"""Run original RTC/UART inline functions through real single and paired devices."""
from pathlib import Path
import subprocess
import sys
runtime, sdk, libraries, output = map(Path, sys.argv[1:])
fixture = Path(__file__).resolve().parent / 'fixtures/compat-byte-ports'
subprocess.run([sys.executable, str(sdk/'compat/build_project.py'), str(fixture),
                '--sdk', str(sdk), '--runtime-build', str(libraries), '--output', str(output)], check=True)
program = output/'byte_ports'
for command, completions in [([str(runtime),'run','--headless','--max-ticks','40','--',str(program)],1),
                             ([str(runtime),'run-pair','--headless','--max-ticks','40',str(program),'--right',str(program)],2)]:
    result = subprocess.run(command,capture_output=True,text=True,timeout=15)
    if result.returncode or result.stdout.count('ORIGINAL_BYTE_PORTS_OK') != completions:
        raise RuntimeError(result.stdout+result.stderr)
    print(result.stdout,end='')
