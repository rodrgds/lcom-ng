#!/usr/bin/env python3
"""Package Linux ELF or Windows MSYS2 student runtimes and their desktop libraries."""

import argparse
import json
from pathlib import Path
import platform
import re
import shutil
import subprocess
import tarfile
import tempfile

from package_student import copy_student_payload, student_examples, write_checksum

# Use the student's glibc and loader. Bundling these would bind the whole desktop
# process, including dynamically loaded graphics drivers, to the build machine.
SYSTEM_ELF = re.compile(r"^(ld-linux.*|lib(c|m|dl|pthread|rt|util|resolv)\.so(?:\..*)?)$")


def dependencies(binary, windows):
    completed = subprocess.run(["ldd", str(binary)], capture_output=True, text=True, check=True)
    if "not found" in completed.stdout:
        raise ValueError("Unresolved dependency of " + str(binary) + "\n" + completed.stdout)
    paths = []
    for line in completed.stdout.splitlines():
        match = re.search(r"(?:=>\s*)?(/\S+)\s+\(", line)
        if not match:
            continue
        path = Path(match.group(1))
        if windows:
            if not str(path).startswith("/mingw64/bin/"):
                # Use the installed MSYS runtime/toolchain, not a second copy of
                # msys-2.0.dll that would select a different root/mount namespace.
                # Windows system DLLs likewise belong to the host installation.
                continue
        elif SYSTEM_ELF.match(path.name):
            continue
        paths.append(path)
    return paths


def package(source, build, output, windows=False):
    source, build, output = (p.resolve() for p in (source, build, output))
    architecture = {"AMD64": "x86_64", "aarch64": "arm64", "arm64": "arm64",
                    "x86_64": "x86_64"}.get(platform.machine())
    if architecture is None or (windows and architecture != "x86_64"):
        raise ValueError("Unsupported desktop package architecture: " + platform.machine())
    target = "windows-msys2" if windows else "linux"
    name = "lcom-ng-" + target + "-" + architecture
    suffix = ".exe" if windows else ""
    runtime = build / ("lcom-ng" + suffix)
    linked = dependencies(runtime, windows)
    if not any("sdl3" in path.name.lower() for path in linked):
        raise ValueError("Student bundles require an SDL-enabled runtime")
    output.mkdir(parents=True, exist_ok=True)
    archive = output / (name + ".tar.gz")
    with tempfile.TemporaryDirectory(prefix="lcom-ng package ") as directory:
        root = Path(directory) / name
        copy_student_payload(source, build, root)
        (root / "bin").mkdir()
        (root / "lib").mkdir()
        (root / "examples").mkdir()
        shutil.copytree(source / "third-party/licenses/macos-runtime", root / "licenses/desktop",
                        ignore=shutil.ignore_patterns("NOTICE.txt", "SOURCES.txt"))
        (root / "licenses/desktop/NOTICE.txt").write_text(
            "Desktop libraries are listed in bundle.json and remain dynamically linked.\n"
            "Compatible replacement libraries can be installed in the bundled library directory.\n"
            "Linux load paths were adjusted; library implementations were not modified.\n"
            "SDL: https://github.com/libsdl-org/SDL\n"
            "SDL_ttf: https://github.com/libsdl-org/SDL_ttf\n"
            "Linux dependency sources: https://archive.ubuntu.com/ubuntu/pool/\n"
            "MSYS2 dependency sources: https://mirror.msys2.org/msys/sources/ and "
            "https://mirror.msys2.org/mingw/sources/\n")
        font_candidates = ([Path("/mingw64/share/fonts/TTF/DejaVuSansMono.ttf"),
                            Path("/mingw64/share/fonts/dejavu/DejaVuSansMono.ttf")]
                           if windows else [Path("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf")])
        font = next((p for p in font_candidates if p.is_file()), None)
        if font is None:
            raise ValueError("DejaVuSansMono.ttf is required for the desktop package")
        (root / "share/fonts").mkdir(parents=True)
        shutil.copy2(font, root / "share/fonts/DejaVuSansMono.ttf")
        for notice in (Path("/usr/share/doc/fonts-dejavu-core/copyright"),
                       Path("/mingw64/share/licenses/ttf-dejavu/LICENSE")):
            if notice.is_file():
                shutil.copy2(notice, root / "licenses/DejaVu-copyright")
        runtime_target = root / "bin" / ("lcom-ng.exe" if windows else "lcom-ng")
        shutil.copy2(runtime, runtime_target)
        pending = [runtime]
        originals = {runtime: runtime_target}
        for example in student_examples(build):
            copied = root / "examples" / example.name
            shutil.copy2(example, copied)
            originals[example] = copied
            pending.append(example)
        libraries = {}
        while pending:
            owner = pending.pop()
            for library in dependencies(owner, windows):
                resolved = library.resolve(strict=True)
                if library.name in libraries and libraries[library.name] != resolved:
                    raise ValueError("Dependency basename collision: " + library.name)
                libraries[library.name] = resolved
                if resolved not in originals:
                    copied = root / ("bin" if windows else "lib") / library.name
                    shutil.copy2(resolved, copied)
                    originals[resolved] = copied
                    pending.append(resolved)
        if windows:
            # The POSIX process/socket runtime and compiler are MSYS2. Keep the
            # executable's DLL directory in PATH for child processes as well.
            (root / "bin/lcom-ng").write_text(
                '#!/usr/bin/env sh\nset -eu\n'
                'directory=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)\n'
                'export PATH="$directory:$PATH"\n'
                'exec "$directory/lcom-ng.exe" "$@"\n')
            (root / "bin/lcom-ng").chmod(0o755)
            for alias in ("machinelab", "lcom", "lowlab", "mlab"):
                shutil.copy2(root / "bin/lcom-ng", root / "bin" / alias)
        else:
            for copied in originals.values():
                subprocess.run(["patchelf", "--set-rpath", "$ORIGIN/../lib:$ORIGIN", str(copied)], check=True)
            for alias in ("machinelab", "lcom", "lowlab", "mlab"):
                (root / "bin" / alias).symlink_to("lcom-ng")
        metadata = {"version": json.loads((source / "package.json").read_text())["version"],
                    "platform": target, "architecture": architecture,
                    "binary": "bin/lcom-ng", "sdk": "sdk", "sdl": True,
                    "dependencies": sorted(libraries)}
        if not windows:
            metadata["minimum_glibc"] = "2.39"
        else:
            metadata["requires"] = "MSYS2 MSYS shell, gcc, make, python"
        (root / "bundle.json").write_text(json.dumps(metadata, indent=2) + "\n")
        (root / "README.txt").write_text(
            "lcom-ng " + metadata["version"] + " for " + target + " " + architecture + "\n\n"
            "Keep this directory intact. The CLI needs sdk/, course/, dev/, and its desktop libraries.\n"
            + ("Use the MSYS2 MSYS terminal, not MINGW64 or PowerShell.\n"
               "Update MSYS2 first: pacman -Syu (restart the shell if requested).\n"
               "Install host tools: pacman -S --needed gcc make python\n" if windows else
               "Ubuntu 24.04 or a compatible Linux desktop with glibc 2.39 or newer.\n"
               "Install host tools: sudo apt install build-essential python3 fonts-dejavu-core\n")
            + "Add the archive's bin directory to PATH. SDL is bundled.\n\n"
            "From an existing project's Makefile directory:\n"
            "  lcom-ng compat build .\n"
            "  lcom-ng run -- .lcom-ng/proj\n"
            "Use the binary name printed by compat build if PROG is not proj.\n\n"
            "Native starter:\n"
            "  lcom-ng setup ~/lcom-student\n"
            "  cd ~/lcom-student && make\n"
            "  lcom-ng run -- build/student_project\n"
            "  lcom-ng test rtc\n"
            "Fresh TODO labs fail until implemented. More: docs/from-minix.md\n")
        # Preserve third-party notices available from dependency installations.
        if windows:
            for prefix in (Path("/usr/share/licenses"), Path("/mingw64/share/licenses")):
                if prefix.is_dir():
                    shutil.copytree(prefix, root / "licenses" / prefix.parent.parent.name,
                                    dirs_exist_ok=True)
        else:
            for package_name in ("libfreetype6", "libharfbuzz0b", "libpng16-16t64", "zlib1g",
                                 "libstdc++6", "libgcc-s1"):
                copyright_file = Path("/usr/share/doc") / package_name / "copyright"
                if copyright_file.is_file():
                    shutil.copy2(copyright_file, root / "licenses" / (package_name + "-copyright"))
        with tarfile.open(archive, "w:gz") as tar:
            tar.add(root, arcname=root.name)
    write_checksum(archive)
    return archive


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument("--build", type=Path, default=Path("build"))
    parser.add_argument("--output", type=Path, default=Path("build/dist"))
    parser.add_argument("--windows-msys2", action="store_true")
    args = parser.parse_args()
    system = platform.system()
    if args.windows_msys2:
        if not system.startswith(("MSYS", "CYGWIN")):
            parser.error("Windows bundles must be built with the MSYS2 MSYS toolchain")
    elif system != "Linux":
        parser.error("Use package_macos.py for macOS")
    package(args.source, args.build, args.output, args.windows_msys2)
