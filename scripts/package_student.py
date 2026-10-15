#!/usr/bin/env python3
"""Shared contents of the installed student SDK, independent of binary format."""

import hashlib
import os
from pathlib import Path
import shutil


def student_examples(build):
    manifest = build / "student-examples.txt"
    if not manifest.is_file():
        raise ValueError("Missing CMake example manifest; configure and build lcom-ng first")
    examples = []
    for name in manifest.read_text().splitlines():
        example = Path(name).resolve(strict=True)
        if not example.is_file() or not os.access(example, os.X_OK):
            raise ValueError("Declared example is not executable: " + name)
        examples.append(example)
    return examples


def copy_student_payload(source, build, root):
    (root / "sdk").mkdir(parents=True)
    (root / "licenses").mkdir()
    shutil.copy2(source / "LICENSE", root / "licenses/lcom-ng-LICENSE")
    for owner in ("sdk/include", "sdk/compat", "sdk/client", "sdk/web", "sdk/export", "sdk/android", "sdk/collection", "course", "dev/lab-tests", "scripts", "docs"):
        shutil.copytree(source / owner, root / owner,
                        ignore=shutil.ignore_patterns("__pycache__", "*.pyc"))
    # Browser and Android exports rebuild the shared device core for their target.
    for owner in ("core", "devices", "backends", "web", "embedded"):
        shutil.copytree(source / "runtime" / owner, root / "sdk/runtime" / owner)
    for library in ("libmachinelab.a", "liblcom_minix.a"):
        shutil.copy2(build / library, root / "sdk" / library)
    for page in (source / "docs-site").rglob("*.md"):
        relative = page.relative_to(source)
        if ".vitepress" in relative.parts or "public" in relative.parts:
            continue
        target = root / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(page, target)
    shutil.copy2(source / "README.md", root / "README.md")


def write_checksum(archive):
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    archive.with_name(archive.name + ".sha256").write_text(digest + "  " + archive.name + "\n")
    print(archive)
    print("SHA256 " + digest)
