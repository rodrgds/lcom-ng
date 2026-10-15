"""Source discovery and CMake export, shared by source-based app exporters."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shlex
import shutil
import config as project_config

SDK = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('lcom_compat_builder', SDK / 'compat/build_project.py')
compat = importlib.util.module_from_spec(spec)
spec.loader.exec_module(compat)


def argument_parser(description):
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('project', nargs='?', default=Path('.'), type=Path, help='project or collection directory (default: .)')
    parser.add_argument('--output', type=Path, help='export directory')
    parser.add_argument('--source', action='append', default=[], help='native C source relative to project; repeatable')
    parser.add_argument('--include', action='append', default=[], help='include directory relative to project; repeatable')
    parser.add_argument('--library', action='append', default=[], metavar='NAME=SOURCES', help='rebuild a Minix library from C sources')
    parser.add_argument('--asset', action='append', default=[], help='file/directory to package relative to asset root; repeatable')
    parser.add_argument('--asset-root', type=Path, help='Minix asset root relative to project (default: project)')
    parser.add_argument('--config', type=Path, help='optional lcom.json path (default: project/lcom.json)')
    parser.add_argument('--icon', type=Path, help='square PNG launcher artwork')
    parser.add_argument('--orientation', choices=['auto','landscape','portrait'], help='mobile orientation, overriding lcom.json')
    parser.add_argument('--name', help='game title, overriding lcom.json')
    parser.add_argument('--arg', action='append', default=[], help='game argument; repeatable')
    return parser


def workspace(project):
    return (project / '.mlab/sdk/libmachinelab.a').is_file() and (project / 'proj').is_dir()


def minix(project):
    makefile = compat.find_makefile(project)
    return makefile is not None and not workspace(project) and bool(
        re.search(r'(?m)^\s*(?:PROG\s*[?+:]?=|\.include.*minix)', makefile.read_text()))


def native_sources(args, project):
    if args.source:
        files = [(project / name).resolve(strict=True) for name in args.source]
    elif workspace(project):
        files = sorted(path for folder in ('proj', 'lib', 'labs') for path in (project / folder).rglob('*.c'))
    else:
        files = sorted([*project.glob('*.c'), *(project / 'src').rglob('*.c')])
    includes = [project, project / 'include', project / 'lib', project / 'src']
    if workspace(project):
        includes += sorted((project / 'labs').glob('*/include'))
    includes += [(project / value).resolve(strict=True) for value in args.include]
    if not files:
        raise compat.BuildError('No C sources found. Provide --source for each source or a Minix Makefile.')
    for source in files:
        if source.suffix != '.c':
            raise compat.BuildError(f'Expected a C source: {source}')
    return files, includes


def cmake_quote(value):
    # Bracket quoting preserves spaces, backslashes, dollars and quotes.
    text = str(value)
    if ';' in text or ']=]' in text:
        raise compat.BuildError('CMake source paths/flags containing semicolons or ]=] are unsupported')
    return '[=[' + text + ']=]'


class CMakeProject(compat.Builder):
    """Translate the existing declarative Makefile traversal into CMake targets."""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.entries = []
        self.groups = {}
        self.selected = []

    def runtime_libraries(self):
        return []  # The app's CMake build supplies its embedded SDK and compatibility library.

    def compile(self, source, makefile):
        if source.parent == self.output and source.name == 'project_context.c':
            source.write_text('const char *lcom_compat_project_root = ".";\n')
        flags = self.compile_flags(source, makefile)
        key = 'source_' + str(len(self.entries))
        self.entries.append((key, source, flags))
        self.manifest['sources'][str(source)] = hashlib.sha256(source.read_bytes()).hexdigest()
        return key

    def local_archive(self, makefile, archive):
        key = (makefile.directory, archive)
        if key in self.archives:
            return self.archives[key]
        declared = self.library_overrides if archive in self.library_overrides else makefile.libraries()
        if archive not in declared:
            raise compat.BuildError(f'No source list for {archive} in {makefile.directory}/Makefile')
        name = 'archive_' + str(len(self.groups))
        self.groups[name] = [self.compile(source, makefile) for source in declared[archive]]
        self.archives[key] = name
        return name

    def link(self, objects, archives, runtime, system_flags, binary):
        self.selected = objects
        self.link_archives = archives


def write_native_project(args, output, target_prefix=""):
    """Export sources, optionally namespace CMake targets for a collection."""
    if target_prefix and not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", target_prefix):
        raise compat.BuildError("Invalid native target prefix")
    project = args.project.resolve(strict=True)
    asset_root = (project / (args.asset_root or Path('.'))).resolve(strict=True)
    if not project.is_relative_to(asset_root):
        raise compat.BuildError('The asset root must contain the project directory')
    is_minix = minix(project) and not args.source
    work = output / 'source-plan'
    work.mkdir()
    plan = CMakeProject(project, SDK, SDK, work, 'cc', args.library, args.include, args.asset_root or Path('.'))
    if is_minix:
        plan.build()
    else:
        if args.library or args.asset_root:
            raise compat.BuildError('--library and --asset-root require a Minix Makefile project')
        files, includes = native_sources(args, project)
        for source in files:
            key = 'source_' + str(len(plan.entries))
            flags = ['-I', str(SDK / 'include')]
            for directory in includes:
                flags += ['-I', str(directory)]
            plan.entries.append((key, source, flags))
            plan.selected.append(key)
        plan.link_archives = []
    native = output / 'native'
    native.mkdir()
    # Collect literal includes recursively. Only selected sources and their headers
    # enter the export, never arbitrary files from the surrounding checkout.
    files = set()
    include_dirs = set()
    pending = []
    seen = set()
    for _, source, flags in plan.entries:
        source = source.resolve()
        files.add(source)
        directories = tuple(Path(flags[i+1]).resolve() for i, flag in enumerate(flags[:-1]) if flag == '-I')
        include_dirs.update(directories)
        pending.append((source, directories))
    while pending:
        source, directories = pending.pop()
        if (source, directories) in seen:
            continue
        seen.add((source, directories))
        includes = re.findall(r'^\s*#\s*include\s*(["<])([^">\n]+)[">]', source.read_text(errors='replace'), re.M)
        for delimiter, name in includes:
            search = (source.parent, *directories) if delimiter == '"' else directories
            for folder in search:
                header = (folder / name).resolve()
                if header.is_file():
                    files.add(header)
                    pending.append((header, directories))
                    break
    import os
    external = [p for p in files if not p.is_relative_to(SDK)]
    snapshot_root = Path(os.path.commonpath([str(project), *[str(p.parent) for p in external],
                                          *[str(p) for p in include_dirs if not p.is_relative_to(SDK)]]))
    def escaped_path(path):
        cmake_quote(path)  # Reject list separators before entering quoted CMake paths.
        return path.as_posix().replace('\\', '\\\\').replace('"', '\\"').replace('$', '\\$')
    def relocated(path):
        path = Path(path).resolve()
        if path.is_relative_to(SDK): return '${LCOM_SDK}/' + escaped_path(path.relative_to(SDK))
        if path.is_relative_to(snapshot_root): return '${CMAKE_CURRENT_LIST_DIR}/sources/' + escaped_path(path.relative_to(snapshot_root))
        raise compat.BuildError(f'Include path outside exported source tree: {path}')
    source_hashes = {str(source): hashlib.sha256(source.read_bytes()).hexdigest() for source in external if not source.is_relative_to(work)}
    for source in external:
        target = native / 'sources' / source.relative_to(snapshot_root)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        if str(source) in source_hashes and hashlib.sha256(target.read_bytes()).hexdigest() != source_hashes[str(source)]:
            raise compat.BuildError(f"Source changed during export: {source}")
    lines = []
    target = lambda key: target_prefix + key
    for key, source, flags in plan.entries:
        key = target(key)
        # CMake variables intentionally expand here; quote path values independently.
        lines.append(f'add_library({key} OBJECT "{relocated(source)}")')
        opts = ['-fcommon', '-Dmain=lcom_student_main']
        i = 0
        while i < len(flags):
            if flags[i] == '-I':
                i += 1
                lines.append(f'target_include_directories({key} PRIVATE "{relocated(flags[i])}")')
            else: opts.append(flags[i])
            i += 1
        # CMake de-duplicates individual options, breaking repeated split flags
        # such as "-D FIRST -D SECOND". Parse the authored sequence as one group.
        lines.append(f'target_compile_options({key} PRIVATE ' + cmake_quote('SHELL:' + shlex.join(opts)) + ')')
    for key, members in plan.groups.items():
        lines.append(f'add_library({target(key)} STATIC ' + ' '.join(f'$<TARGET_OBJECTS:{target(m)}>' for m in members) + ')')
    # The original Makefile links these objects directly. Wrapping them in an
    # archive can discard data-only units when tentative header definitions
    # already satisfy the linker, silently replacing initialized tables by zeros.
    student = target('student')
    lines.append(f'add_library({student} INTERFACE)')
    lines.append(f'target_sources({student} INTERFACE ' + ' '.join(f'$<TARGET_OBJECTS:{target(m)}>' for m in plan.selected) + ')')
    if plan.link_archives:
        libraries=' '.join(target(k) for k in plan.link_archives)
        lines += ['if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR ANDROID)',
                  f'  target_link_libraries({student} INTERFACE "-Wl,--start-group" {libraries} "-Wl,--end-group")',
                  'else()',f'  target_link_libraries({student} INTERFACE {libraries})','endif()']
    (native / 'student.cmake').write_text('\n'.join(lines) + '\n')
    name = args.name or project.name
    working = project.relative_to(asset_root).as_posix() if is_minix else '.'
    (native / 'GameConfig.hpp').write_text('#pragma once\nnamespace game {\ninline const char *name = ' + project_config.cpp_string(name) + ';\ninline const char *workingDirectory = ' + project_config.cpp_string(working) + ';\ninline const char *arguments[] = {' + ','.join(project_config.cpp_string(s) for s in [name,*args.arg]) + '};\n}\n')
    assets = args.asset if getattr(args,'assets_configured',False) else args.asset or ([(project / 'assets').relative_to(asset_root).as_posix()] if (project / 'assets').is_dir() else [])
    asset_dest = output / 'game-assets'
    asset_dest.mkdir()
    for item in assets:
        source = (asset_root / item).resolve(strict=True)
        if not source.is_relative_to(asset_root) or source == asset_root:
            raise compat.BuildError(f'Asset must exist inside the project asset root: {item}')
        target = asset_dest / source.relative_to(asset_root)
        target.parent.mkdir(parents=True, exist_ok=True)
        if source.is_dir(): shutil.copytree(source, target, dirs_exist_ok=True)
        else: shutil.copy2(source, target)
    (asset_dest / working).mkdir(parents=True, exist_ok=True)
    sources = source_hashes
    shutil.rmtree(work)
    return {'name': name, 'sources': sources, 'workingDirectory': working}
