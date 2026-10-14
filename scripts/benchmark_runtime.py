#!/usr/bin/env python3
"""Run via devenv shell. Timings are opt-in diagnostics, not shared CI limits."""

import argparse
import json
import os
from pathlib import Path
import re
import signal
import statistics
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, default=Path("build"))
    parser.add_argument("--runtime", type=Path)
    parser.add_argument("--sdl", action="store_true", help="Also open real SDL windows")
    parser.add_argument("--runs", type=int, default=3)
    parser.add_argument("--max-port-ms", type=float, help="Optional local regression limit")
    args = parser.parse_args()
    if args.runs < 1:
        parser.error("--runs must be positive")
    root = Path(__file__).resolve().parent.parent
    build = args.build_dir.resolve()
    runtime = (args.runtime or build / "machinelab").resolve()
    measurements = {}
    failed = False
    with tempfile.TemporaryDirectory(prefix="runtime-benchmark-", dir=build) as tmp:
        guest = Path(tmp) / "guest"
        subprocess.run([
            os.environ.get("CC", "cc"), "-O2", "-I", str(root / "sdk/include"),
            str(root / "tests/runtime_benchmark.c"), str(build / "libmachinelab.a"),
            "-o", str(guest),
        ], check=True)
        cases = [("ports_headless", "ports", "headless", False)]
        if args.sdl:
            cases += [("ports_sdl", "ports", "sdl", False),
                      ("ports_pair_sdl", "ports", "sdl", True),
                      ("console_sdl", "logs", "sdl", False)]
        cases += [("capture_800x600", "0x115", "headless", False),
                  ("capture_1280x1024", "0x200", "headless", False)]
        for name, mode, display, paired in cases:
            capture = mode not in {"ports", "logs"}
            times = []
            for _ in range(args.runs):
                with tempfile.TemporaryDirectory(dir=tmp) as frames:
                    command = [str(runtime), "run-pair" if paired else "run",
                               "--display", display]
                    if capture:
                        command += ["--frame-dir", frames]
                    if paired:
                        command += [str(guest), mode, "--right", str(guest), mode]
                    else:
                        command += ["--", str(guest), mode]
                    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                               text=True, start_new_session=True)
                    try:
                        stdout, stderr = process.communicate(timeout=60)
                    except subprocess.TimeoutExpired:
                        os.killpg(process.pid, signal.SIGKILL)
                        process.communicate()
                        raise
                    result = subprocess.CompletedProcess(command, process.returncode, stdout, stderr)
                    if result.returncode:
                        raise RuntimeError(result.stdout + result.stderr)
                    elapsed = [float(x) for x in re.findall(r"benchmark_ms=([0-9.]+)", result.stdout)]
                    if len(elapsed) != (2 if paired else 1):
                        raise RuntimeError("Guest measurement missing: " + result.stdout)
                    if capture and len(list(Path(frames).glob("frame_*.ppm"))) != 60:
                        raise RuntimeError("Expected all 60 captured frames")
                    times.append(max(elapsed))
            median = statistics.median(times)
            measurements[name] = {"median_ms": median, "runs_ms": times}
            if capture:
                measurements[name]["ms_per_frame"] = round(median / 60, 3)
            if mode == "ports" and args.max_port_ms is not None and median > args.max_port_ms:
                failed = True
            print(json.dumps({name: measurements[name]}), flush=True)
    return int(failed)


if __name__ == "__main__":
    raise SystemExit(main())
