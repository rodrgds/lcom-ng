#!/usr/bin/env python3
"""Rebuild and exercise pinned original LCOM labs and games from a manifest."""

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

from check_minix_projects import checkout, command

ROOT = Path(__file__).resolve().parent.parent


def verify_run(case, run, project, binary, runtime, output):
    stem = output / run["id"]
    log, trace, frame = stem.with_suffix(".log"), stem.with_suffix(".jsonl"), stem.with_suffix(".ppm")
    trace.unlink(missing_ok=True)
    args = [runtime, "run", "--headless", "--max-ticks", run.get("max_ticks", 600),
            "--rtc", "2026-10-13T12:00:00", "--trace", trace]
    if run.get("script"):
        args += ["--script", ROOT / run["script"]]
    if run.get("frame"):
        frame.unlink(missing_ok=True)
        args += ["--dump-frame", frame]
    args += ["--", binary, *run.get("args", [])]
    code = command(args, log, cwd=project, timeout=run.get("timeout", 30), env=case.get("run_env"))
    text = log.read_text()
    guarded = "max virtual ticks reached" in text
    passed = code == run.get("exit", 0) and guarded == run.get("guard", False)
    passed = passed and all(fragment in text for fragment in run.get("stdout_contains", []))
    trace_expectation = run.get("trace_last_tick")
    if trace_expectation is not None or run.get("trace_sequence"):
        records = [json.loads(line) for line in trace.read_text().splitlines()] if trace.is_file() else []
        if trace_expectation is not None:
            ticks = [record["tick"] for record in records if "tick" in record]
            passed = passed and bool(ticks) and max(ticks) == trace_expectation
        position = 0
        for expected in run.get("trace_sequence", []):
            while position < len(records) and not all(records[position].get(key) == value
                                                     for key, value in expected.items()):
                position += 1
            if position == len(records):
                passed = False
                break
            position += 1
    pixels_present = None
    if run.get("frame"):
        valid = False
        if frame.is_file():
            try:
                magic, dimensions, maximum, pixels = frame.read_bytes().split(b"\n", 3)
                width, height = (int(part) for part in dimensions.split())
                valid = magic == b"P6" and maximum == b"255" and len(pixels) == width * height * 3
                pixels_present = any(pixels)
                for probe in run.get("pixel_checks", []):
                    x, y = probe["x"], probe["y"]
                    if not (0 <= x < width and 0 <= y < height):
                        valid = False
                        continue
                    offset = (y * width + x) * 3
                    valid = valid and list(pixels[offset:offset + 3]) == probe["rgb"]
                if run.get("nonblank", True):
                    valid = valid and pixels_present
            except (ValueError, OSError):
                pass
        passed = passed and valid
    return {"id": run["id"], "args": run.get("args", []), "exit": code,
            "guard": guarded, "passed": passed, "log": str(log), "trace": str(trace),
            "frame": str(frame) if run.get("frame") else None,
            "populated_pixels": pixels_present, "scope": run.get("scope", "normal callback exit")}


def original_sources(repository):
    names = subprocess.check_output(["git", "-C", str(repository), "ls-files", "-z"]).decode().split("\0")
    return {name: hashlib.sha256((repository / name).read_bytes()).hexdigest()
            for name in names if name and (repository / name).is_file() and
            (Path(name).suffix in (".c", ".h", ".S", ".s", ".xpm") or Path(name).name == "Makefile")}


def verify_case(case, repository, runtime, evidence):
    before = original_sources(repository)
    output = evidence / case["id"]
    output.mkdir(parents=True, exist_ok=True)
    project = repository / case["project"]
    build = output / "build"
    log = output / "build.log"
    code = command([runtime, "compat", "build", project, "--output", build,
                    *case.get("builder_args", [])], log, timeout=180)
    result = {"id": case["id"], "repo": case["repo"], "sha": case["sha"], "build_exit": code,
              "build_log": str(log), "runs": [], "passed": False}
    if code:
        return result
    manifest = json.loads((build / "build.json").read_text())
    for run in case.get("runs", []):
        result["runs"].append(verify_run(case, run, project, manifest["binary"], runtime, output))
    unchanged = all(hashlib.sha256(Path(name).read_bytes()).hexdigest() == digest
                    for name, digest in manifest["sources"].items())
    # Compare the actual downloaded source bytes before and after execution.
    # Case-colliding Git paths may already differ from HEAD on macOS; we do not
    # rewrite them or misreport that pre-existing filesystem limitation.
    pristine = before == original_sources(repository)
    result["source_hashes_unchanged"] = unchanged and pristine
    result["passed"] = bool(result["runs"]) and all(run["passed"] for run in result["runs"]) and unchanged and pristine
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, default=ROOT / "tests/compat-expanded.json")
    parser.add_argument("--corpus", type=Path, default=ROOT / "build/compat-expanded")
    parser.add_argument("--runtime", type=Path, default=ROOT / "build/lcom-ng")
    parser.add_argument("--fetch", action="store_true")
    parser.add_argument("--id", action="append", help="only run these case IDs; repeatable")
    args = parser.parse_args()
    cases = json.loads(args.manifest.read_text())["cases"]
    if args.id:
        known = {case["id"] for case in cases}
        missing = set(args.id) - known
        if missing:
            parser.error("Unknown case IDs: " + ", ".join(sorted(missing)))
        cases = [case for case in cases if case["id"] in args.id]
    evidence = args.corpus.resolve() / "verification"
    evidence.mkdir(parents=True, exist_ok=True)
    results = []
    for case in cases:
        try:
            repository = args.corpus.resolve() / "corpus" / case["repo"].replace("/", "__")
            checkout(case, repository, args.fetch)
            result = verify_case(case, repository, args.runtime.resolve(), evidence)
        except (OSError, ValueError, subprocess.SubprocessError) as error:
            result = {"id": case["id"], "repo": case["repo"], "passed": False, "error": str(error)}
        results.append(result)
        print(json.dumps(result), flush=True)
    (evidence / "results.json").write_text(json.dumps(results, indent=2) + "\n")
    return 0 if results and all(result["passed"] for result in results) else 1


if __name__ == "__main__":
    sys.exit(main())
