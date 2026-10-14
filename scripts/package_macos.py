#!/usr/bin/env python3
"""Package the macOS SDL runtime and student SDK with relocatable dependencies."""

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


def run(*args):
    return subprocess.check_output([str(a) for a in args], text=True).strip()


def dependencies(path):
    linked = [line.strip().split(" (compatibility", 1)[0]
              for line in run("/usr/bin/otool", "-L", path).splitlines()[1:]]
    identifiers = run("/usr/bin/otool", "-D", path).splitlines()[1:]
    if identifiers and linked and linked[0] == identifiers[0].strip():
        linked = linked[1:]  # LC_ID_DYLIB is an ID, not a dependency to resolve.
    return linked


def system_library(path):
    return path.startswith(("/usr/lib/", "/System/Library/"))


def rpaths(path):
    return re.findall(r"cmd LC_RPATH\n\s+cmdsize \d+\n\s+path (.*?) \(offset", run("/usr/bin/otool", "-l", path))


def resolve_dependency(name, owner):
    if name.startswith("@loader_path/"):
        path = owner.parent / name[len("@loader_path/"):]
    elif name.startswith("@rpath/"):
        tail = name[len("@rpath/"):]
        candidates = [owner.parent / tail]
        candidates.extend(Path(p.replace("@loader_path", str(owner.parent))) / tail
                          for p in rpaths(owner))
        path = next((p for p in candidates if p.is_file()), None)
        if path is None:
            raise ValueError("Cannot resolve " + name + " from " + str(owner))
    elif name.startswith("@"):
        raise ValueError("Unsupported dependency " + name)
    else:
        path = Path(name)
    return path.resolve(strict=True)


def package(source, build, output):
    source, build, output = (p.resolve() for p in (source, build, output))
    runtime = build / "lcom-ng"
    arch = run("/usr/bin/lipo", "-archs", runtime)
    if arch not in ("arm64", "x86_64"):
        raise ValueError("Expected one supported macOS architecture, got " + arch)
    if not any("SDL3" in name for name in dependencies(runtime)):
        raise ValueError("Student macOS bundles require an SDL-enabled runtime")
    version = json.loads((source / "package.json").read_text())["version"]
    name = "lcom-ng-macos-" + arch
    output.mkdir(parents=True, exist_ok=True)
    archive = output / (name + ".tar.gz")
    with tempfile.TemporaryDirectory(prefix="lcom-ng package ") as directory:
        root = Path(directory) / name
        (root / "bin").mkdir(parents=True)
        (root / "lib").mkdir()
        copy_student_payload(source, build, root)
        shutil.copytree(source / "third-party/licenses/macos-runtime",
                        root / "licenses/macos-runtime")
        shutil.copy2(runtime, root / "bin/lcom-ng")
        originals = {runtime: root / "bin/lcom-ng"}
        (root / "examples").mkdir()
        for example in student_examples(build):
            target = root / "examples" / example.name
            shutil.copy2(example, target)
            originals[example] = target
        pending = list(originals)
        changes = []
        names = {}
        while pending:
            owner = pending.pop()
            target = originals[owner]
            for dependency in dependencies(owner):
                if system_library(dependency):
                    continue
                resolved = resolve_dependency(dependency, owner)
                if resolved == owner:
                    continue  # dylib ID, rather than a dependency
                name_key = resolved.name
                if name_key in names and names[name_key] != resolved:
                    raise ValueError("Dependency basename collision: " + name_key)
                names[name_key] = resolved
                if resolved not in originals:
                    copied = root / "lib" / name_key
                    shutil.copy2(resolved, copied)
                    copied.chmod(0o755)
                    originals[resolved] = copied
                    pending.append(resolved)
                relative = "@loader_path/../lib/" if target.parent.name != "lib" else "@loader_path/"
                changes.append((target, dependency, relative + name_key))
        for target, old, new in changes:
            subprocess.check_call(["/usr/bin/install_name_tool", "-change", old, new, str(target)])
        minimum_macos = (0, 0)
        for original, target in originals.items():
            load_commands = run("/usr/bin/otool", "-l", target)
            versions = re.findall(r"cmd LC_BUILD_VERSION\n.*?minos (\S+)", load_commands, re.S)
            if not versions:
                versions = re.findall(r"cmd LC_VERSION_MIN_MACOSX\n.*?version (\S+)", load_commands, re.S)
            for version_text in versions:
                minimum_macos = max(minimum_macos, tuple(int(v) for v in version_text.split(".")))
            license_root = original.parent.parent / "share/licenses"
            if license_root.is_dir():
                shutil.copytree(license_root, root / "licenses" / original.parent.parent.name,
                                dirs_exist_ok=True)
            if target.parent.name == "lib":
                subprocess.check_call(["/usr/bin/install_name_tool", "-id", "@loader_path/" + target.name, str(target)])
            for path in rpaths(target):
                subprocess.check_call(["/usr/bin/install_name_tool", "-delete_rpath", path, str(target)])
            subprocess.check_call(["/usr/bin/codesign", "--force", "--sign", "-", str(target)])
            unresolved = [p for p in dependencies(target) if not system_library(p) and not p.startswith("@loader_path/")]
            if unresolved:
                raise ValueError("Unrelocated load commands: " + repr(unresolved))
        for alias in ("machinelab", "lcom", "lowlab", "mlab"):
            (root / "bin" / alias).symlink_to("lcom-ng")
        (root / "README.txt").write_text(
            "lcom-ng " + version + " for macOS " + arch + "\n\n"
            "Install Apple's Command Line Tools with: xcode-select --install\n"
            "They provide clang, make, and Python 3. SDL is included.\n"
            "Keep this directory intact; bin/lcom-ng needs lib/ and sdk/.\n\n"
            "Original Minix sources:\n"
            "  bin/lcom-ng compat build /path/to/labs/lab2\n"
            "  bin/lcom-ng run -- /path/to/labs/lab2/.lcom-ng/lab2 timer_test_int 2\n"
            "  bin/lcom-ng compat build /path/to/proj\n"
            "  bin/lcom-ng run -- /path/to/proj/.lcom-ng/proj\n\n"
            "Native course starter:\n"
            "  bin/lcom-ng setup ~/lcom-student\n"
            "  cd ~/lcom-student && make\n"
            "  /path/to/bin/lcom-ng test rtc\n"
            "Fresh TODO labs fail until implemented.\n\n"
            "Installation and usage: docs/macos-student.md\n"
            "Compatibility details: docs-site/developers/from-minix.md\n\n"
            "This trial bundle is ad-hoc signed, not notarized. It is not a Minix OS emulator.\n")
        metadata = {"version": version, "architecture": arch,
                    "binary": "bin/lcom-ng", "sdk": "sdk", "sdl": True,
                    "minimum_macos": ".".join(str(v) for v in minimum_macos),
                    "dependencies": sorted(p.name for p in originals.values() if p.parent.name == "lib")}
        (root / "bundle.json").write_text(json.dumps(metadata, indent=2) + "\n")
        with tarfile.open(archive, "w:gz") as tar:
            tar.add(root, arcname=root.name)
    write_checksum(archive)
    return archive


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument("--build", type=Path, default=Path("build"))
    parser.add_argument("--output", type=Path, default=Path("build/dist"))
    args = parser.parse_args()
    if platform.system() != "Darwin":
        parser.error("This packager runs on macOS")
    package(args.source, args.build, args.output)
