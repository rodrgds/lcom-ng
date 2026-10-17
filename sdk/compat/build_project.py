#!/usr/bin/env python3
"""Build declarative Minix LCOM Makefiles without running their recipes.

Source lists, include paths and local archives are inputs. Shell recipes and
Minix system make includes are not host build commands.
"""

import argparse
import hashlib
import importlib.util
import glob
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys


class BuildError(Exception):
    pass


_assembly_spec = importlib.util.spec_from_file_location("lcom_compat_assembly", Path(__file__).with_name("assembly.py"))
assembly = importlib.util.module_from_spec(_assembly_spec)
_assembly_spec.loader.exec_module(assembly)


def words(value):
    return shlex.split(value)


def find_makefile(directory):
    """Honor common make entry filenames, including on case-sensitive hosts."""
    if not Path(directory).is_dir(): return None
    files={path.name:path for path in Path(directory).iterdir() if path.is_file()}
    return next((files[name] for name in ('GNUmakefile','makefile','Makefile') if name in files),None)


class Makefile:
    def __init__(self, directory, sources=None):
        self.directory = Path(directory).resolve()
        # Minix sys.mk is read before authored Makefiles: DBG?=-O2 and
        # CFLAGS?=${DBG}. Seed recursive defaults so explicit assignments,
        # target/recipe flags and the builder's trailing diagnostics still win.
        self.variables = {".CURDIR": shlex.quote(str(self.directory)),
                          "CURDIR": shlex.quote(str(self.directory)),
                          "DBG": "-O2", "CFLAGS": "${DBG}"}
        self.paths = [self.directory]
        self.object_rules = {}
        self.archive_objects = {}
        self.target_flags = {}
        self.target_variables = {}
        if sources is not None:
            # Explicit declarative inputs override the Makefile, including when
            # an archived project has no build file. Keep normal path validation.
            self.variables["SRCS"] = " ".join(shlex.quote(str(source)) for source in sources)
            self.variables["PROG"] = "proj"
            self.project_sources()
            return
        makefile = find_makefile(self.directory)
        if makefile is None:
            raise BuildError(f"No Makefile in {self.directory}")
        text = makefile.read_text().replace("\\\n", " ")
        targets = []
        def declarative_lines(lines):
            iterator = iter(lines)
            for line in iterator:
                match = re.fullmatch(r"\s*\.for\s+(\w+)\s+in\s+(.*)", line)
                if not match:
                    yield line
                    continue
                variable, expression = match.groups()
                body, depth = [], 1
                for nested in iterator:
                    if re.match(r"\s*\.for\b", nested): depth += 1
                    if re.match(r"\s*\.endfor\b", nested): depth -= 1
                    if depth == 0: break
                    body.append(nested)
                if depth:
                    raise BuildError(f"Unterminated .for in {makefile}")
                for value in words(self.expand(expression)):
                    expanded = [part.replace("${" + variable + "}", shlex.quote(value))
                                    .replace("$(" + variable + ")", shlex.quote(value)) for part in body]
                    yield from declarative_lines(expanded)
        for line in declarative_lines(text.splitlines()):
            if line.startswith("\t"):
                # Read compiler options as data. The command itself, automatic
                # variables, shell expressions, and recipes are never executed.
                recipe = line.strip().lstrip("@-")
                match = re.match(r"(?:\$\{CC\}|\$\(CC\))\s+(.*)", recipe)
                if match:
                    for target in targets:
                        self.target_flags.setdefault(target, []).append(match.group(1))
                archive = re.match(r"(?:ar|\$\{AR\}|\$\(AR\))\s+(.*)", recipe)
                if archive:
                    inputs = words(self.expand(archive.group(1)))
                    names = [item for item in inputs if item.endswith(".a")]
                    objects = [item for item in inputs if item.endswith(".o")]
                    if len(names) == 1 and objects:
                        self.archive_objects[Path(names[0]).name] = objects
                continue
            line = line.split("#", 1)[0].strip()
            if re.match(r"\.(?:if|ifdef|ifndef|elif|else|endif|for|endfor)\b", line):
                raise BuildError(f"Unsupported conditional or loop in {makefile}: {line}")
            match = re.match(r"([\w.]+)\s*(\+=|\?=|:=|!=|=)\s*(.*)", line)
            if match:
                key, op, value = match.groups()
                if op == "!=":
                    # Evaluate supported source queries immediately, as BSD
                    # make does. Other shell computations remain opaque until
                    # a required input references them, then expand rejects
                    # them. Object-directory bookkeeping is not a build input.
                    expression = "$(shell " + value + ")"
                    self.variables[key] = (self.expand(expression)
                                           if re.match(r"\s*find(?:\s|$)", value) else expression)
                elif op == "+=":
                    self.variables[key] = self.variables.get(key, "") + " " + value
                elif op != "?=" or key not in self.variables:
                    self.variables[key] = value
            elif re.match(r"\.PATH(?:\.[\w]+)?:", line):
                self.paths.extend(self.directory / p for p in words(self.expand(line.split(":", 1)[1])))
            elif ":" in line and not line.startswith("."):
                left, right = line.split(":", 1)
                targets = [t for t in words(left) if t.endswith(".o") and "$" not in t]
                local = re.match(r"(CFLAGS|CPPFLAGS)\s*(\+=|=|:=)\s*(.*)", right.strip())
                for target in targets:
                    if local:
                        self.target_variables.setdefault(target, []).append(local.groups())
                    else:
                        inputs = [p for p in words(right) if p.endswith((".c", ".cc", ".cpp", ".s", ".S"))]
                        if inputs:
                            self.object_rules[target] = inputs
            elif ":" in line:
                targets = []
        self.paths.extend(self.directory / p for p in self.get("VPATH").replace(":", " ").split())

    def expand(self, value, seen=(), overrides=None):
        variables = self.variables if overrides is None else {**self.variables, **overrides}
        def replace(match):
            key = match.group(1) or match.group(2)
            if key in seen:
                raise BuildError(f"Recursive Makefile variable: {key}")
            # GNU wildcard is declarative source discovery, never a shell command.
            if key.startswith('wildcard '):
                patterns = words(self.expand(key[len('wildcard '):], seen, overrides))
                return ' '.join(shlex.quote(name) for pattern in patterns
                                for name in sorted(glob.glob(pattern, root_dir=self.directory)))
            if key.startswith('notdir '):
                names = words(self.expand(key[len('notdir '):], seen, overrides))
                return ' '.join(shlex.quote(tail) for name in names
                                if (tail := name.rsplit('/', 1)[-1]))
            if key.startswith('shell '):
                # A few GNU Make projects use this fixed find form for SRCS.
                # Interpret it as a path query, never execute a shell command.
                command = words(self.expand(key[len('shell '):], seen, overrides))
                recursive = command[2:] == ['-type', 'f', '-name', '*.c']
                top_level = command[2:] == ['-maxdepth', '1', '-type', 'f', '-name', '*.c']
                if not command or command[0] != 'find' or not (recursive or top_level):
                    raise BuildError('Unsupported shell source discovery; expected find <relative-dir> [-maxdepth 1] -type f -name \"*.c\"')
                relative = Path(command[1])
                directory = (self.directory / relative).resolve()
                if relative.is_absolute() or '..' in relative.parts or not directory.is_relative_to(self.directory):
                    raise BuildError('Makefile find source directory must remain inside the project')
                if not directory.is_dir():
                    raise BuildError(f'Makefile find source directory does not exist: {relative}')
                candidates = directory.glob('*.c') if top_level else directory.rglob('*.c')
                sources = sorted(path for path in candidates if path.is_file())
                if any(not path.resolve().is_relative_to(self.directory) for path in sources):
                    raise BuildError('Makefile find sources must remain inside the project')
                return ' '.join(shlex.quote(str(path.relative_to(self.directory))) for path in sources)
            if re.match(r'\w+\s',key):
                raise BuildError(f"Unsupported Makefile function: {key.split()[0]}")
            # Only the portable input subset is evaluated, never shell expressions.
            if ":" in key:
                name, *modifiers = key.split(":")
                result = words(self.expand(variables.get(name, ""), (*seen, key), overrides))
                for modifier in modifiers:
                    if modifier == "T":
                        result = [tail for word in result if (tail := word.rsplit("/", 1)[-1])]
                    elif "=" in modifier and not modifier.startswith("sh"):
                        before, after = modifier.split("=", 1)
                        result = [word[:-len(before)] + after if before and word.endswith(before) else word
                                  for word in result]
                    else:
                        raise BuildError(f"Unsupported Makefile modifier: {key}")
                return " ".join(shlex.quote(word) for word in result)
            return self.expand(variables.get(key, ""), (*seen, key), overrides)
        pattern = r"\$\{([^${}]+)\}|\$\(([^$()]+)\)"
        while re.search(pattern, value):
            value = re.sub(pattern, replace, value)
        return value

    def get(self, key):
        return self.expand(self.variables.get(key, ""))

    def sources(self, value):
        result = []
        for name in words(value):
            if name.endswith((".h", ".hpp")):
                continue
            if not name.endswith(".c"):
                raise BuildError(f"Unsupported source {name!r} in {self.directory}")
            candidates = [p / name for p in self.paths if (p / name).is_file()]
            if not candidates:
                raise BuildError(f"Source {name!r} not found in {self.directory} or .PATH/VPATH")
            result.append(candidates[0].resolve())
        return result

    def libraries(self):
        result = {}
        for key in self.variables:
            if not key.startswith("LIB") or not key.endswith("_SRCS"):
                continue
            prefix = key[:-5]
            name = self.get(prefix) or (self.get("LIB_NAME") if prefix == "LIB" else "")
            if name.endswith(".a"):
                result[Path(name).name] = self.sources(self.get(key))
        name = self.get("LIB_NAME")
        if name.endswith(".a") and name not in result and self.get("LIB_OBJS"):
            objects = words(self.get("LIB_OBJS"))
            result[Path(name).name] = self.object_sources(objects)
        for archive, objects in self.archive_objects.items():
            if archive not in result:
                result[archive] = self.object_sources(objects)
        name = self.get("LIB")
        if name and not name.endswith(".a"):
            result[f"lib{name}.a"] = self.sources(self.get("SRCS"))
        return result

    def object_sources(self, objects):
        names = []
        for obj in objects:
            names.extend(self.object_rules.get(obj, [obj[:-2] + ".c"]))
        return self.sources(" ".join(shlex.quote(name) for name in names))

    def project_sources(self):
        if self.get("SRCS"):
            names=[]
            for source in words(self.get("SRCS")):
                if source.endswith((".h",".hpp")): continue
                target=str(Path(source).with_suffix('.o'))
                names.extend(self.object_rules.get(target,[source]))
            return self.sources(' '.join(shlex.quote(name) for name in names))
        sources = []
        for names in self.object_rules.values():
            for source in self.sources(" ".join(shlex.quote(name) for name in names)):
                if source not in sources:
                    sources.append(source)
        if not sources:
            raise BuildError("The project Makefile has no SRCS or explicit object source rules")
        return sources

    def flags(self, source=None):
        # Minix's 32-bit ABI and warnings-as-errors are not portable compiler
        # settings. Keep meaningful preprocessing/optimization flags only.
        values = {key: self.get(key) for key in ("CFLAGS", "CPPFLAGS")}
        recipes = []
        if source is not None:
            for target, names in self.object_rules.items():
                # Selected inputs were validated by project_sources. Stale,
                # unselected rules must not prevent a real target from building.
                try:
                    owners=self.sources(" ".join(shlex.quote(name) for name in names))
                except BuildError:
                    continue
                if source in owners:
                    for key, op, setting in self.target_variables.get(target, []):
                        setting = self.expand(setting, overrides=values)
                        values[key] = values.get(key, "") + " " + setting if op == "+=" else setting
                    recipes.extend(self.target_flags.get(target, []))
        value = values["CFLAGS"] + " " + values["CPPFLAGS"]
        value += " " + " ".join(self.expand(f, overrides=values) for f in recipes)
        flags = words(value)
        result = []
        i = 0
        while i < len(flags):
            flag = flags[i]
            if flag in ("-I", "-D", "-U"):
                i += 1
                if i == len(flags) or flags[i].startswith("-"):
                    raise BuildError(f"Missing argument after {flag} in {self.directory}/Makefile")
                value = str((self.directory / flags[i]).resolve()) if flag == "-I" else flags[i]
                result.extend([flag, value])
            elif flag.startswith("-I"):
                result.extend(["-I", str((self.directory / flag[2:]).resolve())])
            elif flag.startswith(("-D", "-U", "-O")) or flag in (
                    "-fcommon", "-fno-common", "-fno-inline", "-fno-unroll-loops",
                    "-fno-builtin", "-funsigned-char", "-fsigned-char", "-fwrapv"):
                result.append(flag)
            i += 1
        return result


class IncludeAliases:
    """Declared absolute include prefixes, using Clang's built-in VFS only."""
    def __init__(self, aliases, asset_root):
        self.root = Path(asset_root).resolve()
        self.aliases = sorted(((origin, (self.root / destination).resolve())
                               for origin, destination in aliases.items() if origin.startswith('/')),
                              key=lambda pair: len(pair[0]), reverse=True)
        self.used = set()
        self.mapped_headers = set()
        self.checked_compilers = set()

    def resolve(self, name):
        for origin, destination in self.aliases:
            if name.startswith(origin + '/'):
                suffix = name[len(origin) + 1:]
                if '..' in Path(suffix).parts:
                    raise BuildError('Absolute include aliases cannot traverse parent directories')
                header = (destination / suffix).resolve()
                if not destination.is_relative_to(self.root) or not header.is_relative_to(destination):
                    raise BuildError('Absolute include alias escapes its asset directory')
                self.used.add(origin)
                return header
        return None

    def headers(self, source, directories):
        pending, seen, found = [source], set(), set()
        while pending:
            current = pending.pop()
            if current in seen: continue
            seen.add(current)
            for delimiter, name in re.findall(r'^\s*#\s*include\s*(["<])([^">\n]+)[">]', current.read_text(errors='replace'), re.M):
                mapped = self.resolve(name)
                candidates = [mapped] if mapped is not None else [
                    (folder / name).resolve() for folder in ((current.parent, *directories) if delimiter == '"' else directories)]
                for header in candidates:
                    if header.is_file():
                        if mapped is not None: self.mapped_headers.add(header)
                        found.add(header); pending.append(header)
                        break
        return found

    def overlay(self, relocate=str, relative=False):
        return {'version': 0, 'case-sensitive': True, 'use-external-names': False, 'overlay-relative': relative,
                'roots': [{'type': 'directory-remap', 'name': origin, 'external-contents': relocate(destination)}
                          for origin, destination in self.aliases if origin in self.used]}

    def flags(self, source, flags, compiler, output):
        if not self.aliases: return []
        directories = tuple(Path(flags[i+1]).resolve() for i, flag in enumerate(flags[:-1]) if flag == '-I')
        self.headers(source, directories)
        if not self.used: return []
        overlay = Path(output) / 'include-aliases.json'
        overlay.write_text(json.dumps(self.overlay()) + '\n')
        identity = tuple(compiler)
        if identity not in self.checked_compilers:
            probe = subprocess.run([*compiler, '-ivfsoverlay' + str(overlay), '-E', '-x', 'c', '-'],
                                   input='', text=True, capture_output=True)
            if probe.returncode:
                raise BuildError('Absolute include build.pathAliases require Clang (including Emscripten/Android Clang); '
                                 'select --cc clang. Compiler diagnostic: ' + probe.stderr.strip())
            self.checked_compilers.add(identity)
        return ['-ivfsoverlay' + str(overlay)]


class Builder:
    def __init__(self, project, sdk, runtime_build, output, compiler, libraries=(), includes=(), asset_root=None,
                 debug=False, sanitize=None, c_standard=None, path_aliases=None, source_files=None):
        self.project = project.resolve()
        self.sdk = sdk.resolve()
        self.runtime_build = runtime_build.resolve()
        self.output = output.resolve()
        self.compiler = words(compiler)
        self.archiver = "ar"
        self.binary = None
        self.project_no_args = False
        settings = {}
        if c_standard is None or path_aliases is None or source_files is None:
            if (self.project / "lcom.json").is_file():
                spec = importlib.util.spec_from_file_location("lcom_project_config", self.sdk / "export/config.py")
                config = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(config)
                settings = config.load(self.project)["build"]
        self.source_files = source_files if source_files is not None else settings.get("sources") or None
        self.c_standard = c_standard or settings.get("cStandard", "gnu11")
        self.diagnostic_flags = ["-g", "-O0", "-fno-omit-frame-pointer"] if debug or sanitize else []
        if sanitize:
            self.diagnostic_flags.extend(["-fsanitize=" + sanitize, "-fno-sanitize-recover=all"])
        self.sanitize = sanitize
        if asset_root is not None:
            self.asset_root = (self.project / asset_root).resolve()
            if not self.asset_root.is_dir():
                raise BuildError(f"Asset root directory not found: {self.asset_root}")
        else:
            self.asset_root = self.project.parent
            # Student ZIPs often sit inside an unrelated Git checkout. A
            # nearby proj/project boundary takes precedence over that checkout.
            for directory in (self.project, *list(self.project.parents)[:4]):
                if directory.name.lower() in ("proj", "project"):
                    self.asset_root = directory.parent
                    break
                if (directory / ".git").exists():
                    self.asset_root = directory
                    break
        # Pre-LCF projects provide their own VBE/lmlib or text-video declarations. Select
        # this header generation only with positive legacy evidence, and never
        # for projects importing the modern course headers.
        texts = [p.read_text(errors="replace") for p in self.project.rglob("*")
                 if p.suffix.lower() in (".c", ".h") and p.is_file()]
        modern = any(re.search(r'#\s*include\s*[<"]lcom/', text) for text in texts)
        code_texts = [re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S) for text in texts]
        # The original course libvt interface predates LCF. Its header and an
        # actual old memory-range consumer identify this generation together.
        text_video = (any(re.search(r'^\s*#\s*include\s*[<"]vt_info\.h[>"]', text, re.M)
                          for text in code_texts)
                      and any(re.search(r'\bstruct\s+mem_range\b', text) for text in code_texts))
        legacy = text_video or any("struct reg86u" in text or re.search(r'void\s*\*\s*lm_init\s*\(\s*void\s*\)', text)
                                  for text in texts)
        self.legacy_timer_fields = False
        for text in code_texts:
            if re.search(r'\benum\s+timer_status_field\b', text) and all(
                    re.search(r'\b' + name + r'\b', text)
                    for name in ("all", "initial", "mode", "base")):
                self.legacy_timer_fields = True
        self.legacy = legacy and not modern
        self.legacy_lm_status = self.legacy and any(re.search(r"int\s+lm_init\s*\(\s*void\s*\)", text) for text in texts)
        self.manifest = {"project": str(self.project), "sources": {}, "commands": []}
        self.original_sources = {}
        self.manifest["asset_root"] = str(self.asset_root)
        self.archives = {}
        self.active = set()
        self.includes = []
        for directory in includes:
            directory = (self.project / directory).resolve()
            if not directory.is_dir():
                raise BuildError(f"Include directory not found: {directory}")
            self.includes.extend(["-I", str(directory)])
        self.library_overrides = {}
        for specification in libraries:
            if "=" not in specification:
                raise BuildError("Expected --library name=source1.c,source2.c")
            name, source_list = specification.split("=", 1)
            if not re.fullmatch(r"[A-Za-z0-9_+.-]+", name) or not source_list:
                raise BuildError(f"Invalid library override {specification!r}; use name=source1.c,source2.c")
            name = name if name.endswith(".a") else f"lib{name}.a"
            if name in self.library_overrides:
                raise BuildError(f"Repeated library override: {name}")
            sources = []
            for value in source_list.split(","):
                source = (self.project / value).resolve()
                if not value.endswith(".c"):
                    raise BuildError(f"Unsupported source {value!r} in --library {name}")
                if not source.is_file():
                    raise BuildError(f"Library source not found: {source}")
                sources.append(source)
            self.library_overrides[name] = sources
        self.include_aliases = IncludeAliases(path_aliases if path_aliases is not None else settings.get("pathAliases", {}),
                                              self.project / settings.get("assetRoot", ".") if asset_root is None else self.asset_root)
        self.objects = self.output / "objects"
        self.objects.mkdir(parents=True, exist_ok=True)

    def command(self, args, purpose="running build command"):
        self.manifest["commands"].append(args)
        result = subprocess.run(args, cwd=self.project)
        if result.returncode:
            raise BuildError(f"Failed while {purpose} (status {result.returncode}). "
                             "Review the compiler diagnostics above. Original sources were not changed.")

    def project_context(self, root):
        """Immutable launch metadata shared by native, browser and app builds."""
        return ("const char *lcom_compat_project_root = " + json.dumps(str(root)) + ";\n"
                "const char *lcom_compat_project_name = " + json.dumps(self.binary.name) + ";\n")

    def compile_flags(self, source, makefile):
        flags = makefile.flags(source) + self.includes
        flags = [str(self.project_path(value)) if index and flags[index - 1] == "-I" else value
                 for index, value in enumerate(flags)]
        return (["-std=" + self.c_standard, "-fsigned-char", "-fcommon", "-include", "lcom/paths.h", "-Wno-deprecated-non-prototype",
                                "-Wno-incompatible-pointer-types-discards-qualifiers",
                                "-I", str(self.sdk / "compat/include"),
                                "-I", str(self.sdk / "include"), "-I", str(makefile.directory)]
                + (["-DLCOM_COMPAT_LEGACY=1", "-include", "lcom/libc.h", "-Wno-implicit-function-declaration", "-Wno-return-mismatch"] if self.legacy else [])
                + (["-DLCOM_COMPAT_LEGACY_LM_STATUS=1"] if self.legacy_lm_status else [])
                + (["-DLCOM_COMPAT_TIMER_LEGACY_FIELDS=1"] if self.legacy_timer_fields else [])
                + (["-DLCOM_COMPAT_PROJECT_NO_ARGS=1", "-Dproj_main_loop=lcom_compat_project_main_noargs"]
                   if self.project_no_args and source != self.output / "project_entry.c" else [])
                + flags + self.diagnostic_flags)

    def compilation_input(self, source, flags):
        try:
            generated, extra = assembly.lower(source, self.output)
        except assembly.AssemblyError as error:
            raise BuildError(str(error)) from error
        if generated != source:
            self.original_sources[generated] = source
        return generated, flags + extra

    def compile(self, source, makefile):
        self.manifest["sources"][str(source)] = hashlib.sha256(source.read_bytes()).hexdigest()
        flags = self.compile_flags(source, makefile)
        flags += self.include_aliases.flags(source, flags, self.compiler, self.output)
        for header in self.include_aliases.mapped_headers:
            self.manifest["sources"][str(header)] = hashlib.sha256(header.read_bytes()).hexdigest()
        source, flags = self.compilation_input(source, flags)
        identity = str(source) + "\0" + "\0".join(flags)
        target = self.objects / (hashlib.sha256(identity.encode()).hexdigest()[:20] + ".o")
        args = self.compiler + flags + ["-c", str(source), "-o", str(target)]
        self.command(args, "compiling " + source.name)
        return target

    def local_archive(self, makefile, archive):
        key = (makefile.directory, archive)
        if key in self.archives:
            return self.archives[key]
        if key in self.active:
            raise BuildError(f"Cyclic archive dependency: {key}")
        declared = self.library_overrides if archive in self.library_overrides else makefile.libraries()
        if archive not in declared:
            raise BuildError(f"No source list for {archive} in {makefile.directory}/Makefile. "
                             "Provide a source Makefile or --library name=source1.c,source2.c.")
        self.active.add(key)
        target = self.output / (hashlib.sha256(str(key).encode()).hexdigest()[:12] + "-" + archive)
        objects = [self.compile(s, makefile) for s in declared[archive]]
        # Recreate the archive so removed objects cannot remain after a rebuild.
        target.unlink(missing_ok=True)
        self.command([self.archiver, "rcs", str(target)] + [str(o) for o in objects])
        self.active.remove(key)
        self.archives[key] = target
        return target

    def find_archive(self, name, directories):
        if name in self.library_overrides:
            return self.local_archive(Makefile(self.project), name)
        for directory in directories:
            if find_makefile(directory) is None:
                continue
            makefile = Makefile(directory)
            if name in makefile.libraries():
                return self.local_archive(makefile, name)
        # The older FEUP image supplied libvt.a for video-text information
        # (LCOM 2011 lab 1, section 3.2). Graphics games often retain -lvt
        # without referencing it. Prefer declared sources above; unresolved
        # video-text calls still fail at the linker, never become fake stubs.
        # Minix Makefiles also name the LCF/low-memory/XPM dependencies as
        # archives, sometimes alongside their -l spelling. The runtime already
        # owns these APIs. As above, explicit project sources still win.
        if name in ("libvt.a", "liblm.a", "liblcf.a", "libxpm.a"):
            return None
        raise BuildError(f"Cannot rebuild {name}. Searched: {', '.join(str(d) for d in directories)}. "
                         "Provide --library name=source1.c,source2.c or a source Makefile.")

    def project_path(self, name):
        prefix = "/home/lcom/labs/"
        if name.startswith(prefix):
            return (self.asset_root / name[len(prefix):]).resolve()
        return (self.project / name).resolve()

    def rebuild_object(self, name, project_makefile):
        original = self.project_path(name)
        owner = project_makefile
        for directory in (original.parent, *original.parent.parents):
            if find_makefile(directory) is not None:
                owner = Makefile(directory)
                break
        declarations = owner.object_rules.get(name) or owner.object_rules.get(original.name)
        if declarations:
            sources = owner.sources(" ".join(shlex.quote(value) for value in declarations))
        elif original.with_suffix(".c").is_file():
            sources = [original.with_suffix(".c")]
        else:
            sources = owner.sources(original.with_suffix(".c").name)
        if len(sources) != 1:
            raise BuildError(f"Object {name!r} must declare exactly one C source")
        return self.compile(sources[0], owner)

    def build(self):
        previous = self.output / "build.json"
        if previous.is_file():
            try:
                report = json.loads(previous.read_text())
                old_binary = Path(report.get("binary", ""))
                if report.get("project") == str(self.project) and old_binary.parent == self.output:
                    old_binary.unlink(missing_ok=True)
            except (ValueError, TypeError):
                pass
        makefile = Makefile(self.project, self.source_files)
        program = makefile.get("PROG") or "proj"
        target = Path(program)
        if target.is_absolute() or target.name in ("", ".", "..") or "\0" in program:
            raise BuildError(f"Invalid PROG: {program!r}")
        # Minix makefiles may place PROG in ../bin. The importer owns the
        # destination directory, so retain the target name without its path.
        self.binary = self.output / target.name
        self.binary.unlink(missing_ok=True)
        self.manifest["binary"] = str(self.binary)
        if self.sanitize:
            # Compiler support alone does not prove its runtime can initialize
            # on this host. Fail before launching a student program that hangs.
            probe_source = self.output / "sanitizer_probe.c"
            probe_binary = self.output / "sanitizer_probe"
            probe_source.write_text("int main(void) { return 0; }\n")
            self.command(self.compiler + self.diagnostic_flags + [str(probe_source), "-o", str(probe_binary)],
                         "checking compiler sanitizer support")
            try:
                probe = subprocess.run([str(probe_binary)], capture_output=True, text=True, timeout=5)
                if probe.returncode:
                    raise BuildError(f"The {self.sanitize} sanitizer runtime failed to start on this machine. "
                                     "Use --debug without sanitizers or another supported compiler.\n" + probe.stderr)
            except subprocess.TimeoutExpired:
                alternative = "--sanitize undefined or --debug" if "address" in self.sanitize else "--debug"
                raise BuildError(f"The {self.sanitize} sanitizer runtime did not initialize within 5 seconds. "
                                 f"Use {alternative}; this failure occurs before student code runs.")
        sources = makefile.project_sources()
        # Match a definition, not a forward declaration or a comment mentioning
        # the callback. Its ABI is adapted in a separate generated translation unit.
        definitions = []
        for source in sources:
            text = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"', '',
                          source.read_text(errors="replace"), flags=re.S)
            definitions.extend(re.findall(r'\bint\b\s*(?:\(\s*)?proj_main_loop\s*\)?\s*\(([^()]*)\)\s*\{', text))
        self.project_no_args = len(definitions) == 1 and definitions[0].strip() in ("", "void")
        objects = [self.compile(s, makefile) for s in sources]
        if self.project_no_args:
            entry = self.output / "project_entry.c"
            entry.write_text("int lcom_compat_project_main_noargs(void);\n"
                             "int proj_main_loop(int argc, char **argv) {\n"
                             "  (void)argc; (void)argv; return lcom_compat_project_main_noargs();\n}\n")
            objects.append(self.compile(entry, makefile))
        ldflags = words(makefile.get("LDADD") + " " + makefile.get("LDLIBS"))
        directories = [self.project]
        names = []
        system_flags = []
        i = 0
        while i < len(ldflags):
            flag = ldflags[i]
            if flag == "-L":
                i += 1
                if i == len(ldflags):
                    raise BuildError("Missing library directory after -L")
                directories.append(self.project_path(ldflags[i]))
            elif flag.startswith("-L"):
                directories.append(self.project_path(flag[2:]))
            elif flag.startswith("-l") and flag not in ("-llcf", "-llm", "-lxpm", "-ltimers", "-ldriver", "-lsys"):
                if flag in ("-lm", "-lpthread"):
                    system_flags.append(flag)
                else:
                    names.append(f"lib{flag[2:]}.a")
            elif flag.endswith(".a"):
                archive = self.project_path(flag)
                directories.append(archive.parent)
                names.append(archive.name)
            elif flag.endswith(".o"):
                obj = self.rebuild_object(flag, makefile)
                if obj not in objects:
                    objects.append(obj)
            i += 1
        for flag in words(makefile.get("DPADD")):
            if flag.endswith(".a") and Path(flag).name not in ("liblcf.a", "liblm.a", "libxpm.a"):
                archive = self.project_path(flag)
                directories.append(archive.parent)
                if archive.name not in names:
                    names.append(archive.name)
        # An explicit --library also requests linking when the original
        # Makefile forgot that dependency. Matching names still replace its
        # foreign Minix archive instead of linking a second copy.
        names.extend(name for name in self.library_overrides if name not in names)
        archives = []
        for name in names:
            archive = self.find_archive(name, directories)
            if archive is not None:
                archives.append(archive)
        context = self.output / "project_context.c"
        context.write_text(self.project_context(self.asset_root))
        objects.append(self.compile(context, makefile))
        binary = self.binary
        self.link(objects, archives, self.runtime_libraries(), system_flags, binary)
        # Sources are never rewritten. The hashes make that property reviewable.
        for name, digest in self.manifest["sources"].items():
            if hashlib.sha256(Path(name).read_bytes()).hexdigest() != digest:
                raise BuildError(f"Source changed during build: {name}")
        self.manifest["binary"] = str(binary)
        self.manifest["status"] = "complete"
        self.write_report()
        return binary

    def runtime_libraries(self):
        libraries = [self.runtime_build / "liblcom_minix.a", self.runtime_build / "libmachinelab.a"]
        for library in libraries:
            if not library.is_file():
                raise BuildError(f"Missing runtime library {library}. Build lcom-ng first.")
        return libraries

    def link(self, objects, archives, runtime, system_flags, binary):
        grouped = [str(a) for a in archives]
        if archives and sys.platform.startswith('linux'):
            grouped = ['-Wl,--start-group', *grouped, '-Wl,--end-group']
        self.command(self.compiler + [str(o) for o in objects] + grouped
                     + [str(p) for p in runtime] + ["-lm", "-pthread"] + system_flags
                     + self.diagnostic_flags + ["-o", str(binary)], "linking " + binary.name)

    def write_report(self):
        (self.output / "build.json").write_text(json.dumps(self.manifest, indent=2) + "\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("project", type=Path)
    parser.add_argument("--sdk", required=True, type=Path)
    parser.add_argument("--runtime-build", required=True, type=Path)
    parser.add_argument("--output", type=Path)
    parser.add_argument("--cc", default=os.environ.get("CC", "cc"))
    parser.add_argument("--library", action="append", default=[], metavar="NAME=SOURCES",
                        help="rebuild a local archive from comma-separated .c paths relative to PROJECT; repeatable")
    parser.add_argument("--include", action="append", default=[], type=Path, metavar="DIRECTORY",
                        help="add an include directory relative to PROJECT; repeatable")
    parser.add_argument("--asset-root", type=Path, metavar="DIRECTORY",
                        help="workspace root for legacy /home/lcom/labs paths, relative to PROJECT")
    parser.add_argument("--debug", action="store_true", help="build student code with debug symbols and without optimization")
    parser.add_argument("--sanitize", choices=("address", "undefined", "address,undefined"),
                        help="instrument student code for memory or undefined-behavior diagnostics; implies --debug")
    args = parser.parse_args()
    output = args.output or args.project / ".lcom-ng"
    builder = None
    try:
        builder = Builder(args.project, args.sdk, args.runtime_build, output, args.cc,
                          args.library, args.include, args.asset_root, args.debug, args.sanitize)
        print(builder.build())
    except (BuildError, OSError, ValueError) as error:
        print(f"lcom-ng compat: {error}", file=sys.stderr)
        if builder is not None:
            builder.manifest.update(status="failed", error=str(error))
            try:
                if builder.binary is not None:
                    builder.binary.unlink(missing_ok=True)
                builder.write_report()
            except OSError as report_error:
                print(f"lcom-ng compat: Cannot save build report: {report_error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
