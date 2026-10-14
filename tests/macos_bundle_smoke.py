#!/usr/bin/env python3
"""Exercise a packaged macOS student SDK with Apple's host tools and a clean PATH."""

import argparse
import os
from pathlib import Path
import subprocess
import tempfile

from student_bundle_smoke import extract, exercise


def run(args, env, cwd=None, status=0):
    completed = subprocess.run([str(a) for a in args], env=env, cwd=cwd,
                               capture_output=True, text=True, timeout=30)
    if completed.returncode != status:
        raise AssertionError("Command failed: " + repr(args) + "\n" + completed.stdout + completed.stderr)
    return completed


def smoke(archive):
    env = {"PATH": "/usr/bin:/bin:/usr/sbin:/sbin", "LC_ALL": "C"}
    with tempfile.TemporaryDirectory(prefix="lcom-ng student machine ") as directory:
        root = Path(directory).resolve()
        bundle = extract(archive, root)
        for guide in ("docs/macos-student.md", "docs/from-minix.md",
                      "docs-site/developers/from-minix.md"):
            assert (bundle / guide).is_file(), "Missing bundled student guide: " + guide
        runtime = bundle / "bin/lcom-ng"
        run(["/usr/bin/codesign", "--verify", runtime], env)
        for binary in [runtime, *list((bundle / "lib").glob("*.dylib"))]:
            output = run(["/usr/bin/otool", "-L", binary], env).stdout
            if "/nix/store/" in output or "/opt/homebrew/" in output or "/usr/local/" in output:
                raise AssertionError("Unrelocated binary dependency:\n" + output)
        exercise(bundle, root, env, desktop=True)
    print("macOS package: relocated SDK, legacy IRQ lab, native starter and TODO check passed")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("archive", type=Path)
    args = parser.parse_args()
    smoke(args.archive.resolve())
