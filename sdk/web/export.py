#!/usr/bin/env python3
"""Rebuild a C game as a standalone, static browser export."""

import argparse
import hashlib
import importlib.util
import json
import os
import re
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

WEB = Path(__file__).resolve().parent
SDK = WEB.parent
spec = importlib.util.spec_from_file_location("lcom_compat_builder", SDK / "compat/build_project.py")
compat = importlib.util.module_from_spec(spec)
spec.loader.exec_module(compat)


def run(command, **kwargs):
    subprocess.run([str(value) for value in command], check=True, **kwargs)


def link(objects, archives, runtime, output, project, assets, compiler="em++"):
    command = [compiler, *objects, *archives, *runtime, "-lm", "-O2", "-flto",
               "-sASYNCIFY", "-sASYNCIFY_STACK_SIZE=131072", "-sALLOW_MEMORY_GROWTH",
               "-sSTACK_SIZE=2097152", "-sMODULARIZE", "-sEXPORT_ES6", "-sINVOKE_RUN=0",
               "-sEXIT_RUNTIME", "-sENVIRONMENT=web", "-sFORCE_FILESYSTEM",
               "-sEXPORTED_RUNTIME_METHODS=callMain,ccall,FS",
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
        for archive in archives:
            next_module = self.output / "next.bc"
            run([llvm_link, "--only-needed", merged, archive, "-o", next_module])
            next_module.replace(merged)
        self.binary = self.output / "game.js"
        link([merged], [], runtime, self.binary, self.asset_root, self.web_assets)


def argument_parser():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("project", nargs="?", default=Path("."), type=Path, help="project directory (default: .)")
    parser.add_argument("--output", type=Path, help="export directory (default: PROJECT/.lcom-ng/web)")
    parser.add_argument("--source", action="append", default=[], help="native C source relative to project; repeatable")
    parser.add_argument("--include", action="append", default=[], help="include directory relative to project; repeatable")
    parser.add_argument("--library", action="append", default=[], metavar="NAME=SOURCES", help="rebuild a Minix library from C sources")
    parser.add_argument("--asset", action="append", default=[], help="file/directory to package relative to asset root; repeatable")
    parser.add_argument("--asset-root", type=Path, help="Minix asset root relative to project (default: project)")
    parser.add_argument("--name", help="title displayed by the browser player")
    parser.add_argument("--arg", action="append", default=[], help="argument passed to the game; repeatable")
    return parser


def build(args):
    project = args.project.resolve(strict=True)
    if not project.is_dir():
        raise compat.BuildError("Project must be a source directory, not a compiled executable")
    cache = Path(os.environ.get("XDG_CACHE_HOME", Path.home() / ".cache")) / "lcom-ng/web"
    output = (args.output or project / ".lcom-ng/web").resolve()
    workspace = (project / ".mlab/sdk/libmachinelab.a").is_file() and (project / "proj").is_dir()
    makefile = project / "Makefile"
    minix = makefile.is_file() and not workspace and bool(
        re.search(r"(?m)^\s*(?:PROG\s*[?+:]?=|\.include.*minix)", makefile.read_text()))
    asset_root = (project / (args.asset_root or Path("."))).resolve()
    if not args.asset and (project / "assets").is_dir() and project.is_relative_to(asset_root):
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
            if args.source:
                files = [(project / name).resolve(strict=True) for name in args.source]
            elif workspace:
                files = sorted(path for folder in ("proj", "lib", "labs") for path in (project / folder).rglob("*.c"))
            else:
                files = sorted([*project.glob("*.c"), *(project / "src").rglob("*.c")])
            default_includes = [project / "include", project / "lib", project / "src"]
            if workspace:
                default_includes += sorted((project / "labs").glob("*/include"))
            if not files:
                raise compat.BuildError("No C sources found. Provide --source for each source or a Minix Makefile.")
            objects = []
            for index, source in enumerate(files):
                if source.suffix != ".c":
                    raise compat.BuildError(f"Expected a C source: {source}")
                target = work / f"source-{index}.o"
                includes = [value for directory in [*default_includes, *(project / value for value in args.include)] for value in ("-I", directory)]
                run(["emcc", "-std=gnu11", "-O2", "-I", SDK / "include", "-I", project,
                     *includes, "-c", source, "-o", target])
                sources[str(source)] = hashlib.sha256(source.read_bytes()).hexdigest()
                objects.append(target)
            link(objects, [], runtime, work / "game.js", project, args.asset)
        for source, digest in sources.items():
            if Path(source).is_file() and hashlib.sha256(Path(source).read_bytes()).hexdigest() != digest:
                raise compat.BuildError(f"Source changed during export: {source}")
        config = {"name": args.name or project.name, "arguments": args.arg,
                  "workingDirectory": working_directory}
        (work / "game.json").write_text(json.dumps(config, ensure_ascii=False) + "\n")
        output.mkdir(parents=True, exist_ok=True)
        names = ["game.js", "game.wasm", "game.json"]
        if (work / "game.data").is_file():
            names.append("game.data")
        for name in names:
            shutil.copy2(work / name, output / name)
        for source, name in (("shell.html", "index.html"), ("player.js", "player.js"), ("player.css", "player.css")):
            shutil.copy2(WEB / source, output / name)
            names.append(name)
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
