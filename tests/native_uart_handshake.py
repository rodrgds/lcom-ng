#!/usr/bin/env python3
"""Start the native UART peers late through their real bus and exec boundary."""

import os
from pathlib import Path
import subprocess
import sys
import tempfile


def check(runtime, sender, receiver):
    with tempfile.TemporaryDirectory(prefix="lcom-ng UART startup ") as temporary:
        root = Path(temporary)
        source, wrapper = root / "late.c", root / "late"
        source.write_text(r'''
#include <lcom/lcom.h>
#include <lcom/uart16550.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv) {
  if (argc != 3 || lcom_init() != LCOM_OK) return 2;
  if (strcmp(argv[1], "ready") == 0) {
    for (unsigned i = 0; i < 5000; ++i) {
      unsigned char status = 0;
      if (lcom_port_read8(COM1_BASE + SER_LSR, &status) != LCOM_OK) return 3;
      if (status & LSR_RX_RDY) {
        lcom_printf("ready byte arrived before sender initialization\n");
        execl(argv[2], argv[2], (char *)0);
        return 4;
      }
      usleep(1000);
    }
    return 5;
  }
  lcom_event_t event;
  if (lcom_delay_ticks(30, &event) != LCOM_OK) return 6;
  execl(argv[2], argv[2], (char *)0);
  return 7;
}
''')
        sdk = Path(__file__).resolve().parents[1] / "sdk" / "include"
        library = Path(runtime).resolve().parent / "libmachinelab.a"
        subprocess.run([os.environ.get("CC", "cc"), "-I", str(sdk), str(source),
                        str(library), "-o", str(wrapper)], check=True)
        for left, right, marker in (
            ([str(wrapper), "ready", sender], [receiver], True),
            ([sender], [str(wrapper), "later", receiver], False),
            ([sender], [receiver], False),
        ):
            result = subprocess.run([runtime, "run-pair", "--headless", "--max-ticks", "80",
                                     *left, "--right", *right], capture_output=True, text=True, timeout=10)
            output = result.stdout + result.stderr
            assert result.returncode == 0, output
            assert "peer sender sent PAIR" in output and "peer receiver got PAIR" in output, output
            assert "max virtual ticks reached" not in output, output
            if marker:
                assert "ready byte arrived before sender initialization" in output, output

        # An absent peer is an ordinary bounded protocol failure, without a runtime guard.
        for program, name in ((sender, "sender"), (receiver, "receiver")):
            result = subprocess.run([runtime, "run", "--headless", "--max-ticks", "400", "--", program],
                                    capture_output=True, text=True, timeout=10)
            output = result.stdout + result.stderr
            assert result.returncode == 1, output
            assert f"peer {name} timed out" in output, output
            assert "max virtual ticks reached" not in output, output


if __name__ == "__main__":
    check(*sys.argv[1:4])
