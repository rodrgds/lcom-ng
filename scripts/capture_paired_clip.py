#!/usr/bin/env python3
"""Capture real paired whiteboard motion from deterministic presented frames.

The paired runtime has no video recorder. Replaying to each sample tick keeps
both sides in their actual runtime, including serial delivery. No UI is drawn
by this script. Temporary framebuffers are removed as each PNG is encoded.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

from PIL import Image


def capture(root, output, fps=10, start=30, end=480):
    if fps < 1 or 60 % fps or start < 1 or end <= start:
        raise ValueError('Choose an FPS that divides 60 and a positive tick range')
    step = 60 // fps
    ticks = list(range(start, end + 1, step))
    if len(ticks) > 100:
        raise ValueError('Paired capture is limited to 100 deterministic samples')
    output.mkdir(parents=True, exist_ok=True)
    proof = []
    with tempfile.TemporaryDirectory(prefix='whiteboard-', dir=output) as temporary:
        work = Path(temporary)
        scripts = []
        for side in ('left', 'right'):
            original = root / f'scripts/serial_whiteboard_{side}.mlabscript'
            script = work / f'{side}.mlabscript'
            script.write_text('at 0 rtc 2026-08-12T14:35:42\n' + original.read_text())
            scripts.append(script)
        for index, tick in enumerate(ticks):
            left, right = work / 'left.ppm', work / 'right.ppm'
            command = [str(root / 'build/machinelab'), 'run-pair', '--headless',
                       '--dump-presented', '--left-script', str(scripts[0]),
                       '--right-script', str(scripts[1]), '--left-dump-frame', str(left),
                       '--right-dump-frame', str(right), '--max-ticks', str(tick),
                       str(root / 'build/examples/serial_whiteboard'), 'left', '--right',
                       str(root / 'build/examples/serial_whiteboard'), 'right']
            result = subprocess.run(command, cwd=work, capture_output=True, text=True, timeout=40)
            log = result.stdout + result.stderr
            if result.returncode != 1 or f'max virtual ticks reached ({tick})' not in log:
                raise RuntimeError(f'Paired sample at {tick} did not reach its guard:\n{log}')
            with Image.open(left) as left_image, Image.open(right) as right_image:
                if left_image.size != (800, 600) or right_image.size != left_image.size:
                    raise ValueError('Unexpected whiteboard framebuffer size')
                combined = Image.new('RGB', (1632, 600), '#e8eeec')
                combined.paste(left_image, (0, 0))
                combined.paste(right_image, (832, 0))
                combined.save(work / f'{index:04d}.png')
                proof.append({'tick': tick, 'pixel_sha256': hashlib.sha256(combined.tobytes()).hexdigest()})
            left.unlink()
            right.unlink()
        if len({sample['pixel_sha256'] for sample in proof}) < 10:
            raise ValueError('Paired footage did not contain meaningful motion')
        subprocess.run(['ffmpeg', '-y', '-loglevel', 'error', '-framerate', str(fps),
                        '-i', str(work / '%04d.png'), '-c:v', 'libx264', '-crf', '18',
                        '-pix_fmt', 'yuv420p', '-movflags', '+faststart',
                        str(output / 'serial_whiteboard.mp4')], check=True)
    (output / 'serial_whiteboard.capture.json').write_text(json.dumps({
        'fps': fps, 'source': 'real paired runtime, bounded presented frames',
        'samples': proof}, indent=2) + '\n')
    print(f'Captured {len(ticks)} paired samples at {fps} fps')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--fps', type=int, default=10)
    parser.add_argument('--start-tick', type=int, default=30)
    parser.add_argument('--end-tick', type=int, default=480)
    options = parser.parse_args()
    project = options.root.resolve()
    capture(project, (options.output or project / 'build/launch-clips').resolve(),
            options.fps, options.start_tick, options.end_tick)
