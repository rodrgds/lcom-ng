#!/usr/bin/env python3
"""Verify a relocated release can compile original Minix code and native starters."""

import argparse
import json
import os
from pathlib import Path
import platform
import subprocess
import tarfile
import tempfile


def run(args, env, cwd=None, status=0):
    completed = subprocess.run([str(a) for a in args], env=env, cwd=cwd,
                               capture_output=True, text=True, timeout=60)
    if completed.returncode != status:
        raise AssertionError("Command failed (status " + str(completed.returncode) + "): "
                             + repr(args) + "\n" + completed.stdout + completed.stderr)
    return completed


def extract(archive, root):
    with tarfile.open(archive) as tar:
        for member in tar.getmembers():
            target = (root / member.name).resolve()
            if not target.is_relative_to(root):
                raise ValueError("Unsafe package path: " + member.name)
            if member.issym() or member.islnk():
                link_base = target.parent if member.issym() else root
                link = (link_base / member.linkname).resolve()
                if not link.is_relative_to(root):
                    raise ValueError("Unsafe package link: " + member.linkname)
        tar.extractall(root)
    return next(p for p in root.iterdir() if (p / "bin/lcom-ng").is_file())


def exercise(bundle, root, env, desktop=False):
    runtime = bundle / "bin/lcom-ng"
    # The executable retains development fallback paths. Missing archive resources
    # must fail here even on a CI runner that still has the original source tree.
    for relative in ("sdk/libmachinelab.a", "sdk/liblcom_minix.a", "sdk/compat/build_project.py"):
        assert (bundle / relative).is_file(), "Missing bundled SDK resource: " + relative
    for relative in ("course/labs/templates", "dev/lab-tests", "scripts"):
        assert (bundle / relative).is_dir(), "Missing bundled student resources: " + relative
    metadata = json.loads((bundle / "bundle.json").read_text())
    if metadata.get("platform") in ("linux", "windows-msys2"):
        assert (bundle / "share/fonts/DejaVuSansMono.ttf").is_file(), "Missing bundled desktop font"
    env = dict(env, PATH=str(bundle / "bin") + ":" + env["PATH"])
    try:
        run([runtime, "--help"], env)
    except AssertionError:
        if metadata.get("platform") == "windows-msys2":
            executable = bundle / metadata.get("runtime", "bin/lcom-ng.exe")
            print("Packaged PE header:", executable.read_bytes()[:16], flush=True)
            print("Bundled DLLs:", metadata["dependencies"], flush=True)
            diagnostic = subprocess.run(["ldd", str(executable)], env=env,
                                        capture_output=True, text=True, timeout=30)
            print(diagnostic.stdout + diagnostic.stderr, flush=True)
        raise
    # Invoke through PATH too, exercising installed resource lookup and aliases.
    run(["lcom", "--help"], env)
    project = root / "original legacy lab"
    project.mkdir()
    source = """#include <lcom/lcf.h>
int(timer_test_int)(uint8_t ticks) {
  int hook = 3;
  if (sys_irqsetpolicy(0, IRQ_REENABLE, &hook) != OK) return 1;
  while (ticks) {
    message msg; int ipc;
    if (driver_receive(ANY, &msg, &ipc) != OK) return 2;
    if (is_ipc_notify(ipc) && (msg.m_notify.interrupts & BIT(3))) --ticks;
  }
  return sys_irqrmpolicy(&hook);
}
int main(int argc, char **argv) {
  int result = lcf_start(argc, argv);
  lcf_cleanup();
  return result;
}
"""
    makefile = "PROG=lab2\nSRCS=lab2.c\n.include <minix.lcom.mk>\n"
    (project / "lab2.c").write_text(source)
    (project / "Makefile").write_text(makefile)
    run(["lcom-ng", "compat", "build", project], env)
    run([runtime, "run", "--headless", "--max-ticks", "120", "--",
         project / ".lcom-ng/lab2", "timer_test_int", "3"], env)
    if desktop:
        # This initializes the actual packaged SDL backend, video and audio with
        # deterministic CI drivers. Headless runs cannot catch missing SDL DLLs.
        desktop_env = dict(env, SDL_VIDEODRIVER="dummy", SDL_RENDER_DRIVER="software",
                           SDL_AUDIODRIVER="dummy")
        display = run([runtime, "run", "--display", "sdl", "--audio", "sdl", "--timeout", "10", "--max-ticks", "120", "--",
                       project / ".lcom-ng/lab2", "timer_test_int", "3"], desktop_env)
        if "could not load a TTF font" in display.stdout + display.stderr:
            raise AssertionError("Packaged SDL text could not load its desktop font")
    assert (project / "lab2.c").read_text() == source
    assert (project / "Makefile").read_text() == makefile
    student = root / "native student"
    run(["lcom-ng", "setup", student], env)
    run(["make"], env, cwd=student)
    run([runtime, "run", "--headless", "--", student / "build/student_project"], env)
    check = run([runtime, "test", "rtc", "--project", student], env, status=1)
    if "check failed:" not in check.stdout + check.stderr:
        raise AssertionError("Fresh TODO lab did not reach its predefined assertions")


def smoke(archive, desktop=False):
    # Explicit system-tool PATH prevents a source/build-tree or custom SDK from
    # accidentally making a broken installed archive appear usable.
    env = {"PATH": "/usr/bin:/bin:/usr/sbin:/sbin", "LC_ALL": "C"}
    if platform.system().startswith(("MSYS", "CYGWIN")):
        for variable in ("SYSTEMROOT", "WINDIR", "USERPROFILE", "TEMP", "TMP"):
            if variable in os.environ:
                env[variable] = os.environ[variable]
    with tempfile.TemporaryDirectory(prefix="lcom-ng student machine ") as directory:
        root = Path(directory).resolve()
        bundle = extract(archive, root)
        exercise(bundle, root, env, desktop)
    print("Student archive: relocated legacy IRQ lab, native starter and TODO check passed")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("archive", type=Path)
    parser.add_argument("--desktop", action="store_true")
    args = parser.parse_args()
    smoke(args.archive.resolve(), args.desktop)
