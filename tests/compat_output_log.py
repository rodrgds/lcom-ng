"""Verify complete child stdout logging and resource cleanup through both servers."""
from pathlib import Path
import subprocess
import sys
import tempfile

runtime, client = map(lambda value: Path(value).resolve(), sys.argv[1:])
first = "printf first\nputs first\nfprintf first\nElapsed time: 1 seconds\nwrite first\nconsole first\nafter failed configuration\n"
with tempfile.TemporaryDirectory(prefix="lcom-ng output log ") as temporary:
    root = Path(temporary)
    def directory(name):
        path = root / name
        (path / "lab").mkdir(parents=True)
        (path / "b.log").write_text("stale contents\n")
        return path
    single = directory("single")
    result = subprocess.run([runtime, "run", "--headless", "--max-ticks", "30", "--", client, single], capture_output=True, text=True, timeout=15)
    text = result.stdout + result.stderr
    assert result.returncode == 0, text
    assert (single / "lab/a.log").read_text() == first
    assert (single / "b.log").read_text() == "printf third\n"
    assert not (single / "missing").exists() and not (single / "calls.log").exists()
    for line in ["before logging", "printf first", "puts first", "fprintf first", "Elapsed time: 1 seconds", "write first", "console first", "after failed configuration", "printf second", "printf third", "after cleanup", "stderr stays separate"]:
        assert line in text, (line, text)
    left, right = directory("left"), directory("right")
    pair = subprocess.run([runtime, "run-pair", "--headless", "--max-ticks", "30", client, left, "--right", client, right], capture_output=True, text=True, timeout=15)
    pair_text = pair.stdout + pair.stderr
    assert pair.returncode == 0, pair_text
    for path in [left, right]:
        assert (path / "lab/a.log").read_text() == first
        assert (path / "b.log").read_text() == "printf third\n"
    automatic = directory("automatic")
    result = subprocess.run([runtime, "run", "--headless", "--", client, automatic, "--exit"], capture_output=True, text=True, timeout=15)
    assert result.returncode == 0, result.stdout + result.stderr
    assert (automatic / "lab/a.log").read_text() == "flushed automatically on exit"
    failure = directory("failure")
    result = subprocess.run([runtime, "run", "--headless", "--", client, failure, "--write-failure"], capture_output=True, text=True, timeout=15)
    assert result.returncode == 0, result.stdout + result.stderr
    assert "console survives log write failure" in result.stdout
    assert (failure / "lab/a.log").read_bytes() == b""
print("LCF stdout logs and console tee verified through single and paired runtimes")
