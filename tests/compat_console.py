"""A student's printf output must survive stopping a blocked lab at the runtime guard."""
from pathlib import Path
import subprocess
import sys
import tempfile

runtime = Path(sys.argv[1]).resolve()
with tempfile.TemporaryDirectory(prefix="lcom-ng console ") as temporary:
    project = Path(temporary)
    (project / "Makefile").write_text("PROG=lab\nSRCS=lab.c\n.include <minix.lcom.mk>\n")
    (project / "lab.c").write_text('''#include <lcom/lcf.h>
int proj_main_loop(int argc, char **argv) {
  (void)argc; (void)argv;
  if (!freopen("/dev/console", "w", stdout)) return 3;
  printf("Student output before waiting for input\\n");
  int hook = 0;
  if (sys_irqsetpolicy(0, IRQ_REENABLE, &hook)) return 1;
  for (;;) { message msg; int status; if (driver_receive(ANY, &msg, &status)) return 2; }
}
int main(int argc, char **argv) { return lcf_start(argc, argv); }
''')
    build = subprocess.run([runtime, "compat", "build", project], capture_output=True, text=True, timeout=15)
    assert build.returncode == 0, build.stdout + build.stderr
    result = subprocess.run([runtime, "run", "--headless", "--max-ticks", "20", "--", project / ".lcom-ng/lab"],
                            capture_output=True, text=True, timeout=15)
    text = result.stdout + result.stderr
    assert result.returncode != 0 and "max virtual ticks reached" in text, text
    assert "Student output before waiting for input" in text, text
print("Student console output is visible during blocked labs")
