"""Observe instruction transport failures as process failures, not invented bytes."""
import os
from pathlib import Path
import subprocess
import sys
program = Path(sys.argv[1])
env = dict(os.environ, LCOM_COMPAT_MOUSE_REPORTING='0')
for args, diagnostic in [([], 'instruction byte I/O and exact16-bit port passed'),
                         (['read-failure'], 'INB failed on port 0x1234'),
                         (['write-failure'], 'OUTB failed on port 0x1234')]:
    run = subprocess.run([str(program), *args], capture_output=True, text=True, env=env)
    assert (run.returncode == 0) == (not args), (args, run.returncode, run.stderr)
    assert diagnostic in run.stdout + run.stderr, (args, run.stdout, run.stderr)
    assert 'UNEXPECTED_RETURN' not in run.stdout
print('Byte I/O succeeds with exact data; read and write transport failures stop visibly.')
