#!/usr/bin/env python3
"""Check crash diagnostics through the single and paired student run commands."""

import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile


def check(runtime):
    with tempfile.TemporaryDirectory(prefix="lcom-ng crash diagnostics ") as temporary:
        root = Path(temporary)
        source, binary = root / "crash.c", root / "crash"
        source.write_text("#include <signal.h>\nint main(void) { raise(SIGSEGV); return 0; }\n")
        subprocess.run([os.environ.get("CC", "cc"), str(source), "-o", str(binary)], check=True)
        for args in (["run", "--headless", "--", str(binary)],
                     ["run-pair", "--headless", str(binary), "--right", str(binary)]):
            result = subprocess.run([runtime, *args], capture_output=True, text=True, timeout=15)
            assert result.returncode == 128 + signal.SIGSEGV, result.stdout + result.stderr
            assert "SIGSEGV, invalid memory access" in result.stdout, result.stdout
            assert "compat build <project> --debug" in result.stdout, result.stdout


if __name__ == "__main__":
    check(sys.argv[1])
