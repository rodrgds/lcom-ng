#!/usr/bin/env python3
"""Exercise host deadlines and numeric validation through the public student CLI."""

import os
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import time


def check(runtime):
    with tempfile.TemporaryDirectory(prefix="lcom-ng host timeout ") as temporary:
        root = Path(temporary)
        source, binary = root / "loop.c", root / "loop"
        source.write_text(r'''
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#ifdef GRAPHICS
#include <lcom/lcom.h>
#endif
int main(int argc, char **argv) {
  signal(SIGTERM, SIG_IGN);
  FILE *pid = fopen(argv[1], "w");
  if (!pid) return 2;
  fprintf(pid, "%ld\n", (long)getpid());
  fclose(pid);
  puts("student output before timeout");
  fflush(stdout);
  if (argc > 2) return 0;
#ifdef GRAPHICS
  lcom_vbe_mode_info_t info;
  unsigned char *frame = 0;
  if (lcom_init() || lcom_vbe_get_mode_info(0x115, &info) ||
      lcom_vbe_set_mode(0x115) ||
      lcom_phys_map(info.framebuffer_phys, info.framebuffer_size, (void **)&frame)) return 3;
  frame[0] = 51; frame[1] = 34; frame[2] = 17;
  lcom_port_write8(0x61, 0x42);
#endif
  for (;;) {}
}
''')
        subprocess.run([os.environ.get("CC", "cc"), str(source), "-o", str(binary)], check=True)
        script = root / "empty.mlabscript"
        script.write_text("# The guest deliberately never waits for an event.\n")
        for command in ("run", "replay", "run-pair"):
            left_pid, right_pid = root / "left.pid", root / "right.pid"
            left_pid.unlink(missing_ok=True)
            right_pid.unlink(missing_ok=True)
            prefix = [command, *([str(script)] if command == "replay" else []),
                      "--headless", "--timeout", "1.25", "--max-ticks", "1"]
            args = [*prefix, str(binary), str(left_pid)]
            if command == "run-pair":
                args += ["--right", str(binary), str(right_pid)]
            started = time.monotonic()
            result = subprocess.run([runtime, *args], capture_output=True, text=True, timeout=5)
            output = result.stdout + result.stderr
            assert result.returncode == 124, output
            assert "host timeout" in output and "1.25 seconds" in output, output
            assert "max virtual ticks reached" not in output, output
            assert "student output before timeout" in output, output
            assert time.monotonic() - started < 3, output
            for pid_file in ([left_pid, right_pid] if command == "run-pair" else [left_pid]):
                pid = int(pid_file.read_text())
                try:
                    os.kill(pid, 0)
                except ProcessLookupError:
                    pass
                else:
                    raise AssertionError(f"timed out student {pid} is still alive")

            # Exiting normally before the deadline retains the student's exit status.
            normal = [*prefix, str(binary), str(left_pid), "exit"]
            if command == "run-pair":
                normal += ["--right", str(binary), str(right_pid), "exit"]
            result = subprocess.run([runtime, *normal], capture_output=True, text=True, timeout=5)
            assert result.returncode == 0, result.stdout + result.stderr
            assert "host timeout" not in result.stderr, result.stderr

        # The shared framebuffer and already-recorded trace survive a forced stop.
        graphics = root / "graphics-loop"
        sdk = Path(__file__).resolve().parents[1] / "sdk" / "include"
        library = Path(runtime).resolve().parent / "libmachinelab.a"
        subprocess.run([os.environ.get("CC", "cc"), "-DGRAPHICS", "-I", str(sdk),
                        str(source), str(library), "-o", str(graphics)], check=True)
        for command in ("run", "run-pair"):
            left_frame, right_frame, trace = root / "left.ppm", root / "right.ppm", root / "trace.jsonl"
            args = [command, "--headless", "--timeout", "1"]
            if command == "run":
                args += ["--dump-frame", str(left_frame), "--trace", str(trace),
                         str(graphics), str(root / "left.pid")]
            else:
                args += ["--left-dump-frame", str(left_frame), "--right-dump-frame", str(right_frame),
                         str(graphics), str(root / "left.pid"), "--right",
                         str(graphics), str(root / "right.pid")]
            result = subprocess.run([runtime, *args], capture_output=True, text=True, timeout=5)
            assert result.returncode == 124, result.stdout + result.stderr
            for frame in ([left_frame] if command == "run" else [left_frame, right_frame]):
                data = frame.read_bytes()
                header, pixels = data.split(b"\n255\n", 1)
                assert header == b"P6\n800 600", header
                assert pixels[:3] == bytes([17, 34, 51]), pixels[:3]
            if command == "run":
                assert any(row.get("event") == "port_write8" and row.get("port") == "0x0061"
                           for row in map(json.loads, trace.read_text().splitlines())), trace.read_text()

        for command in ("run", "replay", "run-pair"):
            prefix = [command, *([str(script)] if command == "replay" else [])]
            suffix = [str(binary), str(root / "unexpected.pid"), "exit"]
            if command == "run-pair":
                suffix += ["--right", *suffix]
            options = {"--timeout": ["0", "-1", "nan", "inf", "1s", "1e999", "1e-999"],
                       "--max-ticks": ["0", "-1", "2x", "18446744073709551616"],
                       "--scale": ["0", "-1", "2x", "2147483648"]}
            if command != "run-pair":
                options["--video-fps"] = ["0", "-1", "60x", "4294967296"]
            for option, values in options.items():
                for value in values:
                    result = subprocess.run([runtime, *prefix, option, value, *suffix],
                                            capture_output=True, text=True, timeout=5)
                    assert result.returncode != 0 and option in result.stderr, result.stdout + result.stderr
                    assert "positive" in result.stderr, result.stderr
                    assert not (root / "unexpected.pid").exists(), result.stdout
                result = subprocess.run([runtime, *prefix, option], capture_output=True,
                                        text=True, timeout=5)
                assert result.returncode != 0 and "requires a value" in result.stderr, result.stderr


if __name__ == "__main__":
    check(sys.argv[1])
