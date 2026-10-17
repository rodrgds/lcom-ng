#!/usr/bin/env python3
"""Exercise Minix current-process identity through real standalone and paired clients."""
import argparse
import hashlib
from pathlib import Path
import re
import shutil
import shlex
import os
import subprocess
import sys
import tempfile


def command(arguments):
    result = subprocess.run([str(value) for value in arguments], capture_output=True, text=True, timeout=30)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    return result.stdout + result.stderr


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--runtime-build', type=Path, required=True)
    parser.add_argument('--runtime', type=Path, required=True)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='lcom process identity ') as directory:
        root = Path(directory)
        project = root / 'original'
        shutil.copytree(Path(__file__).parent / 'fixtures/process-identity', project)
        (project / 'lcom.json').write_text('{"name":"A different display title"}\n')
        before = {p: hashlib.sha256(p.read_bytes()).digest() for p in project.iterdir()}
        command([sys.executable, args.sdk / 'compat/build_project.py', project,
                 '--sdk', args.sdk, '--runtime-build', args.runtime_build, '--output', root / 'output'])
        binary = root / 'output/identity-process-long'
        standalone = command([args.runtime, 'run', '--headless', '--max-ticks', '120', '--', binary])
        assert re.search(r'PROCESS_IDENTITY_OK endpoint=\d+ name=identity-proces flags=0', standalone), standalone
        paired = command([args.runtime, 'run-pair', '--headless', '--max-ticks', '120', binary, '--right', binary])
        endpoints = re.findall(r'PROCESS_IDENTITY_OK endpoint=(\d+) name=identity-proces flags=0', paired)
        assert len(endpoints) == 2 and endpoints[0] != endpoints[1], paired
        assert all(hashlib.sha256(p.read_bytes()).digest() == digest for p, digest in before.items())
        # Ordinary process IDs seldom equal Minix selectors. Substitute only the
        # owning unit's POSIX dependency to exercise those real numeric bounds.
        compiler = shlex.split(os.environ.get('CC', 'cc'))
        includes = ['-I', str(args.sdk / 'compat/include'), '-I', str(args.sdk / 'include')]
        adapter = root / 'process-adapter.o'
        command([*compiler, '-std=gnu11', *includes, '-Dgetpid=lcom_test_process_pid', '-c',
                 args.sdk / 'compat/src/compat.c', '-o', adapter])
        boundary = root / 'pid-boundaries'
        command([*compiler, '-std=gnu11', *includes,
                 project / 'pid-boundaries.c', adapter,
                 args.runtime_build / 'liblcom_minix.a', args.runtime_build / 'libmachinelab.a',
                 '-lpthread', '-lm', '-o', boundary])
        result = command([args.runtime, 'run', '--headless', '--max-ticks', '120', '--', boundary])
        assert 'PROCESS_ENDPOINT_BOUNDARIES_OK' in result, result
        print('Minix process identity: standalone, paired and reserved-selector PID bounds passed')


if __name__ == '__main__':
    main()
