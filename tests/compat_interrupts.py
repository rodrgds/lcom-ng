#!/usr/bin/env python3
"""Compile actual basic assembly through the importer, then exercise two guests."""
from pathlib import Path
import subprocess
import sys

runtime, sdk, libraries, output = map(Path, sys.argv[1:])
fixture = Path(__file__).resolve().parent / 'fixtures/compat-interrupts'
subprocess.run([sys.executable, str(sdk / 'compat/build_project.py'), str(fixture),
                '--sdk', str(sdk), '--runtime-build', str(libraries), '--output', str(output)], check=True)
program = output / 'interrupts'
for command in ([str(runtime), 'run', '--headless', '--max-ticks', '40', '--', str(program)],
                [str(runtime), 'run-pair', '--headless', '--max-ticks', '40', str(program),
                 '--right', str(program), 'right']):
    result = subprocess.run(command, capture_output=True, text=True, timeout=15)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    expected = ['interrupt flag enabled after CLI/CLI/STI']
    if 'run-pair' in command: expected.append('independent right guest received timer IRQs')
    for message in expected:
        if message not in result.stdout:
            raise RuntimeError('Missing expected guest completion: ' + message + '\n' + result.stdout + result.stderr)
    print(result.stdout, end='')
