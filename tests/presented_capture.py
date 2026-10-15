"""A screenshot must be able to exclude guest writes made after present()."""
import pathlib
import subprocess
import sys
import tempfile

cli, client, console_client = sys.argv[1:]
with tempfile.TemporaryDirectory(prefix='lcom-presented-') as directory:
    root = pathlib.Path(directory)
    for paired in [False, True]:
        for presented in [False, True]:
            image = root / 'frame.ppm'
            if paired:
                command = [cli, 'run-pair', '--headless', '--max-ticks', '5', '--left-dump-frame', str(image)]
                if presented:
                    command.append('--dump-presented')
                command += [client, '--right', client]
            else:
                command = [cli, 'run', '--headless', '--max-ticks', '5', '--dump-frame', str(image)]
                if presented:
                    command.append('--dump-presented')
                command += ['--', client]
            result = subprocess.run(command, capture_output=True, text=True, timeout=10)
            assert result.returncode == 1 and 'max virtual ticks reached (5)' in result.stderr, result.stderr
            pixel = image.read_bytes().split(b'\n', 3)[3][:3]
            expected = b'\0\0\0' if presented else bytes.fromhex('E14F9A')
            assert pixel == expected, (paired, presented, pixel.hex())

    missing = root / 'never-presented.ppm'
    result = subprocess.run([cli, 'run', '--headless', '--dump-presented', '--dump-frame',
                             str(missing), '--', console_client], capture_output=True, text=True, timeout=10)
    assert result.returncode == 1 and 'no presented frame' in result.stderr, result.stderr
    assert not missing.exists(), 'A never-presented frame must not look like a valid screenshot'
