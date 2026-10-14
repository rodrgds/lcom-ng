#!/usr/bin/env python3
"""Build and replay pinned, unchanged LCOM projects outside the tracked tree."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import zipfile


ROOT = Path(__file__).resolve().parent.parent


def command(args, log, cwd=ROOT, timeout=180, env=None):
    with log.open("w") as stream:
        try:
            process = subprocess.Popen([str(a) for a in args], cwd=cwd, stdout=stream,
                                       stderr=subprocess.STDOUT, start_new_session=True,
                                       env={**os.environ, **(env or {})})
            return process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            # The runtime launches a student child. End the test's own process
            # group so a stuck student cannot survive a host timeout.
            os.killpg(process.pid, signal.SIGKILL)
            process.wait()
            stream.write("\nHost process timeout reached\n")
            return 124


def checkout(case, destination, fetch):
    if not destination.exists():
        if not fetch:
            raise ValueError(f"Missing checkout {destination}; use --fetch")
        subprocess.run(["git", "clone", "--depth", "1", "--no-checkout",
                        f"https://github.com/{case['repo']}.git", str(destination)], check=True)
        subprocess.run(["git", "-C", str(destination), "fetch", "--depth", "1", "origin", case["sha"]], check=True)
        subprocess.run(["git", "-C", str(destination), "checkout", "--detach", case["sha"]], check=True)
    current = subprocess.check_output(["git", "-C", str(destination), "rev-parse", "HEAD"], text=True).strip()
    if current != case["sha"]:
        raise ValueError(f"{destination} is at {current}, expected {case['sha']}; checkout preserved")


def unzip_project(archive, destination, project):
    if not destination.exists():
        destination.mkdir(parents=True)
        with zipfile.ZipFile(archive) as source:
            for entry in source.infolist():
                path = Path(entry.filename)
                if path.is_absolute() or ".." in path.parts:
                    raise ValueError(f"Invalid archive path {entry.filename}")
            source.extractall(destination)
    if not (destination / project / "Makefile").is_file():
        raise ValueError(f"Missing original project Makefile under {destination / project}")
    return destination / project


def verify_case(case, project, runtime, evidence):
    output = evidence / case["id"]
    output.mkdir(parents=True, exist_ok=True)
    build_dir = output / "build"
    build_log = output / "build.log"
    # Exercise the public CLI, including source and installed SDK discovery.
    build_command = [runtime, "compat", "build", project, "--output", build_dir] + case.get("builder_args", [])
    build_code = command(build_command, build_log)
    result = {"id": case["id"], "repo": case.get("repo"), "sha": case.get("sha"),
              "build_exit": build_code, "build_log": str(build_log), "project": str(project)}
    expected_failure = case.get("expected_compile_failure")
    if expected_failure:
        result["passed"] = build_code != 0 and expected_failure in build_log.read_text()
        result["verdict"] = "expected incomplete upstream source" if result["passed"] else "unexpected importer result"
        return result
    if build_code:
        result.update(passed=False, verdict="build failed")
        return result
    manifest = json.loads((build_dir / "build.json").read_text())
    frame = output / "exit.ppm"
    frame.unlink(missing_ok=True)
    log = output / "exit.log"
    script = ROOT / case["exit_script"]
    run_code = command([runtime, "run", "--headless", "--max-ticks", "900",
                        "--rtc", "2026-10-13T12:00:00", "--script", script,
                        "--dump-frame", frame, "--trace", output / "trace.jsonl",
                        "--", manifest["binary"]], log, cwd=project, timeout=30,
                       env=case.get("run_env"))
    unchanged = all(hashlib.sha256(Path(name).read_bytes()).hexdigest() == digest
                    for name, digest in manifest["sources"].items())
    guarded = "max virtual ticks reached" in log.read_text()
    result.update(run_exit=run_code, run_log=str(log), frame=str(frame),
                  source_hashes_unchanged=unchanged, virtual_guard_reached=guarded,
                  passed=run_code == 0 and not guarded and unchanged and frame.is_file(),
                  verdict="bounded input replay; visual inspection remains a separate check")
    if case.get("gameplay_script"):
        gameplay_log = output / "gameplay.log"
        gameplay_frame = output / "gameplay.ppm"
        gameplay_frame.unlink(missing_ok=True)
        gameplay_code = command([
            runtime, "run", "--headless", "--max-ticks", case.get("gameplay_exit_max_ticks", 1800),
            "--rtc", "2026-10-13T12:00:00", "--script", ROOT / case["gameplay_script"],
            "--dump-frame", gameplay_frame, "--", manifest["binary"]
        ], gameplay_log, cwd=project, timeout=45, env=case.get("run_env"))
        unchanged = unchanged and all(hashlib.sha256(Path(name).read_bytes()).hexdigest() == digest
                                      for name, digest in manifest["sources"].items())
        result.update(gameplay_exit=gameplay_code, gameplay_log=str(gameplay_log),
                      gameplay_frame=str(gameplay_frame), source_hashes_unchanged=unchanged)
        expected_exit = case.get("gameplay_expected_exit", 0)
        result["gameplay_supported"] = expected_exit == 0 and gameplay_code == 0
        if case.get("known_runtime_failure"):
            result["verdict"] = "working menu; confirmed original gameplay defect: " + case["known_runtime_failure"]
        result["passed"] = (result["passed"] and gameplay_code == expected_exit and unchanged
                            and (expected_exit != 0 or gameplay_frame.is_file())
                            and "max virtual ticks reached" not in gameplay_log.read_text())
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--fetch", action="store_true", help="download missing pinned public checkouts")
    parser.add_argument("--corpus", type=Path, default=ROOT / "build/compat-corpus")
    parser.add_argument("--runtime", type=Path, default=ROOT / "build/lcom-ng")
    parser.add_argument("--pvz", type=Path, help="original PvZ.zip")
    parser.add_argument("--ninjix", type=Path, help="original grupo_2leic09_3-main.zip")
    args = parser.parse_args()
    corpus = args.corpus.resolve()
    evidence = corpus / "verification"
    evidence.mkdir(parents=True, exist_ok=True)
    results = []
    cases = json.loads((ROOT / "tests/compat-projects.json").read_text())["projects"]
    for case in cases:
        try:
            destination = corpus / "github" / case["id"]
            checkout(case, destination, args.fetch)
            result = verify_case(case, destination / case["project"], args.runtime.resolve(), evidence)
        except (OSError, ValueError, subprocess.SubprocessError) as error:
            result = {"id": case["id"], "passed": False, "error": str(error)}
        results.append(result)
        print(json.dumps(result), flush=True)
    for identity, archive, relative, script in (
        ("PvZ", args.pvz, "proj", "scripts/compat-pvz-exit.mlabscript"),
        ("Ninjix", args.ninjix, "grupo_2leic09_3-main/proj", "scripts/ninjix_menu_exit.mlabscript"),
    ):
        if archive is None:
            continue
        try:
            project = unzip_project(archive.resolve(), corpus / "supplied" / identity, relative)
            result = verify_case({"id": identity, "exit_script": script,
                                  "gameplay_script": f"scripts/compat-{identity.lower()}-gameplay.mlabscript",
                                  "gameplay_exit_max_ticks": 1800}, project, args.runtime.resolve(), evidence)
        except (OSError, ValueError, zipfile.BadZipFile) as error:
            result = {"id": identity, "passed": False, "error": str(error)}
        results.append(result)
        print(json.dumps(result), flush=True)
    (evidence / "results.json").write_text(json.dumps(results, indent=2) + "\n")
    return 0 if all(result["passed"] for result in results) else 1


if __name__ == "__main__":
    sys.exit(main())
