"""Keep unrelated executable build artifacts out of the student distribution."""
from pathlib import Path
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from package_macos import student_examples

with tempfile.TemporaryDirectory(prefix="lcom-ng declared examples ") as directory:
    build = Path(directory).resolve()
    examples = build / "examples"
    examples.mkdir()
    intended = examples / "hello"
    stale = examples / "unregistered-local-probe"
    for path in (intended, stale):
        path.write_text("#!/bin/sh\nexit 0\n")
        path.chmod(0o755)
    manifest = build / "student-examples.txt"
    manifest.write_text(str(intended) + "\n")
    assert student_examples(build) == [intended], "Undeclared executable entered student package"
    manifest.unlink()
    try:
        student_examples(build)
    except ValueError as error:
        assert "configure and build" in str(error)
    else:
        raise AssertionError("Missing build manifest silently accepted")
print("Student package excludes stale executables and requires a declared inventory")
