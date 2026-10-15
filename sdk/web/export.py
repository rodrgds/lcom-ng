#!/usr/bin/env python3
"""Rebuild a C game as a standalone, static browser export."""

import hashlib
import html
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

WEB = Path(__file__).resolve().parent
SDK = WEB.parent
sys.path.insert(0, str(SDK / "export"))
import project as source_project
compat = source_project.compat


def run(command, **kwargs):
    subprocess.run([str(value) for value in command], check=True, **kwargs)


def link(objects, archives, runtime, output, project, assets, compiler="em++"):
    command = [compiler, *objects, *archives, *runtime, "-lm", "-O2", "-flto",
               "-sASYNCIFY", "-sASYNCIFY_STACK_SIZE=131072", "-sALLOW_MEMORY_GROWTH",
               "-sSTACK_SIZE=2097152", "-sMODULARIZE", "-sEXPORT_ES6", "-sINVOKE_RUN=0",
               "-sEXIT_RUNTIME", "-sENVIRONMENT=web", "-sFORCE_FILESYSTEM",
               "-sEXPORTED_RUNTIME_METHODS=callMain,ccall,FS,ENV",
               "--js-library", WEB / "library.js", "-o", output]
    # An explicit list keeps source, build artifacts and private files out of exports.
    for item in assets:
        source = (project / item).resolve()
        if not source.is_relative_to(project) or not source.exists():
            raise compat.BuildError(f"Asset must exist inside the project: {item}")
        if "@" in str(source):
            raise compat.BuildError("Asset paths containing @ are not supported by Emscripten")
        # Preserve source-relative paths, including legacy /home/lcom/labs rebasing.
        command += ["--preload-file", f"{source}@/project/{source.relative_to(project).as_posix()}"]
    run(command)


class WebBuilder(compat.Builder):
    def __init__(self, *args, assets=(), **kwargs):
        super().__init__(*args, **kwargs)
        self.archiver = "emar"
        self.web_assets = assets

    def link(self, objects, archives, runtime, system_flags, binary):
        if "-lpthread" in system_flags:
            raise compat.BuildError("Threaded student games are not supported by the browser exporter")
        # The native builder generates the asset-root constant as build context.
        # Set it to the virtual root, without changing any project source file.
        context = self.output / "project_context.c"
        context.write_text('const char *lcom_compat_project_root = "/project";\n')
        makefile = compat.Makefile(self.project)
        context_object = self.compile(context, makefile)
        objects = [obj for obj in objects if obj != context_object] + [context_object]
        llvm_root = Path(subprocess.check_output(["em-config", "LLVM_ROOT"], text=True).strip())
        llvm_link = llvm_root / ("llvm-link.exe" if os.name == "nt" else "llvm-link")
        if not llvm_link.is_file():
            raise compat.BuildError("Minix exports need llvm-link beside Emscripten's clang. Use the project's Devenv toolchain.")
        # Resolve tentative C definitions together in IR. wasm-ld does not merge
        # duplicate commons, and suppressing all duplicate symbols would hide
        # actual conflicting definitions. llvm-link preserves C common semantics.
        merged = self.output / "student.bc"
        run([llvm_link, *objects, "-o", merged])
        if archives:
            llvm_nm = llvm_root / ("llvm-nm.exe" if os.name == "nt" else "llvm-nm")
            if not llvm_nm.is_file():
                raise compat.BuildError("Minix archives need llvm-nm beside Emscripten's clang.")

            def unresolved_symbols():
                symbols = subprocess.check_output(
                    [llvm_nm, "--undefined-only", "--format=posix", merged], text=True)
                return frozenset(line.split()[0] for line in symbols.splitlines() if line.strip())

            # A later archive can introduce references to an earlier one. Repeat
            # until no unresolved symbols change, as a native archive group does.
            # Two archives can alternate across arbitrarily many object members.
            unresolved = unresolved_symbols()
            while True:
                for archive in archives:
                    next_module = self.output / "next.bc"
                    run([llvm_link, "--only-needed", merged, archive, "-o", next_module])
                    next_module.replace(merged)
                remaining = unresolved_symbols()
                if remaining == unresolved:
                    break
                unresolved = remaining
        self.binary = self.output / "game.js"
        link([merged], [], runtime, self.binary, self.asset_root, self.web_assets)


def argument_parser():
    parser = source_project.argument_parser(__doc__)
    return parser


def write_game_metadata(output, settings, arguments, working_directory):
    """Refresh presentation metadata without recompiling an unchanged game."""
    output = Path(output)
    downloads = {name: (output / name).stat().st_size for name in ('game.wasm', 'game.data')
                 if (output / name).is_file()}
    config = {**source_project.project_config.public(settings), 'arguments': arguments,
              'workingDirectory': working_directory, 'downloads': downloads}
    (output / 'game.json').write_text(json.dumps(config, ensure_ascii=False) + '\n')
    for key in ('icon', 'preview'):
        destination = output / f'{key}.png'
        if settings[key]: shutil.copy2(settings[key], destination)
        else: destination.unlink(missing_ok=True)


def write_player(output, settings):
    """Package and version the shared host independently of the game binary."""
    output = Path(output)
    names = ['player.js', 'player.css', 'loading.js']
    for name in names: shutil.copy2(WEB / name, output / name)
    player = (output / 'player.js').read_text()
    for name in ('loading.js', 'game.js'):
        digest = hashlib.sha256((output / name).read_bytes()).hexdigest()[:12]
        player = player.replace(f"'./{name}'", f"'./{name}?v={digest}'")
    (output / 'player.js').write_text(player)
    shell = (WEB / 'shell.html').read_text().replace('<title>lcom-ng game</title>',
        '<title>' + html.escape(settings['name']) + ' · lcom-ng</title>')
    for name in ('player.js', 'player.css'):
        digest = hashlib.sha256((output / name).read_bytes()).hexdigest()[:12]
        shell = shell.replace(f'"./{name}"', f'"./{name}?v={digest}"')
    if settings['icon']:
        shell = shell.replace('</head>', '<link rel="icon" href="./icon.png"><link rel="apple-touch-icon" href="./icon.png"></head>')
    (output / 'index.html').write_text(shell)
    return ['index.html', *names]


def build(args):
    if (args.project / 'lcom.collection.json').is_file():
        sys.path.insert(0, str(SDK))
        from collection import manifest
        return manifest.build(args, 'web')
    settings = source_project.project_config.resolve(args)
    project = args.project.resolve(strict=True)
    if not project.is_dir():
        raise compat.BuildError("Project must be a source directory, not a compiled executable")
    cache = Path(os.environ.get("XDG_CACHE_HOME", Path.home() / ".cache")) / "lcom-ng/web"
    output = (args.output or project / ".lcom-ng/web").resolve()
    workspace = source_project.workspace(project)
    minix = source_project.minix(project)
    asset_root = (project / (args.asset_root or Path("."))).resolve()
    if not args.assets_configured and (project / "assets").is_dir() and project.is_relative_to(asset_root):
        args.asset = [(project / "assets").relative_to(asset_root).as_posix()]
    if output == project or project.is_relative_to(output):
        raise compat.BuildError("Output must be a separate directory, not the project or its parent")
    if output.exists() and any(output.iterdir()) and not (output / "lcom-web.json").is_file():
        raise compat.BuildError("Output is not an existing browser export; choose an empty directory")
    for tool in ("emcc", "em++", "emar", "emcmake", "cmake", "ninja"):
        if not shutil.which(tool):
            raise compat.BuildError(f"Missing {tool}. Run inside the project's Devenv shell or install Emscripten and CMake/Ninja.")
    # Keep the Nix toolchain's immutable cache read-only and compile locally.
    cache.mkdir(parents=True, exist_ok=True)
    os.environ.setdefault("EM_CACHE", str(cache / "emscripten"))
    identity = hashlib.sha256(str(SDK).encode()).hexdigest()[:12]
    build = cache / identity
    run(["emcmake", "cmake", "-S", WEB, "-B", build, "-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release"])
    run(["cmake", "--build", build, "--parallel", "4"])
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix=".lcom-web-", dir=output.parent) as temporary:
        work = Path(temporary)
        runtime = [build / "liblcom_minix.a", build / "libmachinelab.a"]
        working_directory = "/project"
        if minix and not args.source:
            # LTO retains legacy common symbols until the final Wasm link.
            # LLVM's Wasm object writer can crash on -fcommon definitions.
            builder = WebBuilder(project, SDK, build, work, "emcc -O2 -flto", args.library,
                                 args.include, args.asset_root or Path("."), assets=args.asset)
            if not project.is_relative_to(builder.asset_root):
                raise compat.BuildError("The Minix asset root must contain the project directory")
            builder.build()
            sources = builder.manifest["sources"]
            working_directory = "/project/" + project.relative_to(builder.asset_root).as_posix()
        else:
            if args.library or args.asset_root:
                raise compat.BuildError("--library and --asset-root require a Minix Makefile project")
            sources = {}
            files, default_includes = source_project.native_sources(args, project)
            objects = []
            for index, source in enumerate(files):
                if source.suffix != ".c":
                    raise compat.BuildError(f"Expected a C source: {source}")
                target = work / f"source-{index}.o"
                includes = [value for directory in default_includes for value in ("-I", directory)]
                run(["emcc", "-std=gnu11", "-O2", "-I", SDK / "include", "-I", project,
                     *includes, "-c", source, "-o", target])
                sources[str(source)] = hashlib.sha256(source.read_bytes()).hexdigest()
                objects.append(target)
            link(objects, [], runtime, work / "game.js", project, args.asset)
        for source, digest in sources.items():
            if Path(source).is_file() and hashlib.sha256(Path(source).read_bytes()).hexdigest() != digest:
                raise compat.BuildError(f"Source changed during export: {source}")
        write_game_metadata(work, settings, args.arg, working_directory)
        manifest = {'name': args.name, 'short_name': args.name, 'start_url': './', 'scope': './',
                    'display': 'fullscreen' if settings['mobile']['fullscreen'] else 'standalone',
                    'background_color': '#000000', 'theme_color': settings['mobile']['theme']['accent']}
        if args.orientation != 'auto': manifest['orientation'] = args.orientation
        if settings['icon']:
            icon_data = settings['icon'].read_bytes()
            size = int.from_bytes(icon_data[16:20],'big')
            manifest['icons'] = [{'src':'icon.png','sizes':f'{size}x{size}','type':'image/png','purpose':'any'}]
        (work / 'manifest.webmanifest').write_text(json.dumps(manifest, ensure_ascii=False) + '\n')
        output.mkdir(parents=True, exist_ok=True)
        names = ["game.js", "game.wasm", "game.json", "manifest.webmanifest"]
        if settings["icon"]: names.append("icon.png")
        if settings["preview"]: names.append("preview.png")
        if (work / "game.data").is_file():
            names.append("game.data")
        for name in names:
            shutil.copy2(work / name, output / name)
        names.extend(write_player(output, settings))
        # Remove stale identity assets when re-exporting without a configured icon.
        for key in ('icon', 'preview'):
            if not settings[key]: (output / f'{key}.png').unlink(missing_ok=True)
        (output / "lcom-web.json").write_text(json.dumps({"format": 1, "files": names,
            "sources": {name: digest for name, digest in sources.items() if not name.startswith(str(work))}}, indent=2) + "\n")
    return output


def main():
    args = argument_parser().parse_args()
    try:
        output = build(args)
        print(f"Created browser export at {output}\nServe this directory over HTTP or HTTPS; open index.html.")
        return 0
    except (compat.BuildError, OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f"lcom-ng web: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
