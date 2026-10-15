#!/usr/bin/env python3
"""Capture and validate the public example galleries from real runtime replays."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import struct

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / 'scripts/showcase-gallery.json'
OUTPUT = ROOT / 'docs/assets/screenshots/gallery'
BUILD = ROOT / 'build'


def fingerprint(app, state):
    digest = hashlib.sha256(json.dumps([app, state], sort_keys=True).encode())
    digest.update(Path(__file__).read_bytes())
    sources = set()
    for pattern in app['sources']:
        sources.update(path for path in ROOT.glob(pattern) if path.is_file())
    if not sources:
        raise ValueError(f"No source files for {app['slug']}")
    for path in sorted(sources):
        digest.update(str(path.relative_to(ROOT)).encode())
        digest.update(path.read_bytes())
    return digest.hexdigest()


def inspect_image(path):
    from PIL import Image, ImageStat
    with Image.open(path) as original:
        image = original.convert('RGB')
        if image.width < 320 or image.height < 200 or max(ImageStat.Stat(image).stddev) < 5:
            raise ValueError(f'Blank or undersized capture: {path}')
        return {'width': image.width, 'height': image.height,
                'pixels_sha256': hashlib.sha256(image.tobytes()).hexdigest()}


def capture(app, state):
    work = BUILD / 'gallery-captures' / app['slug'] / state['slug']
    work.mkdir(parents=True, exist_ok=True)
    # A fresh fixture directory prevents files from previous runs changing a replay.
    fixture_dir = work / 'files'
    if fixture_dir.exists():
        import shutil
        shutil.rmtree(fixture_dir)
    fixture_dir.mkdir()
    for name, contents in (app.get('fixtures', {}) | state.get('fixtures', {})).items():
        path = fixture_dir / name
        if not path.resolve().is_relative_to(fixture_dir.resolve()):
            raise ValueError(f'Fixture escapes capture directory: {name}')
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(contents)
    script = work / 'input.mlabscript'
    script.write_text(state.get('script', ''))
    frame = work / 'frame.ppm'
    frame.unlink(missing_ok=True)
    ticks = str(state['ticks'])
    cli = [str(BUILD / 'machinelab')]
    expand = lambda value: value.replace('{files}', str(fixture_dir)).replace('{root}', str(ROOT))
    args = [expand(value) for value in state.get('args', app.get('args', []))]
    paired = app.get('paired', False)
    if paired:
        right_script = work / 'right.mlabscript'
        right_script.write_text(state.get('right_script', ''))
        side = state.get('capture_side', 'left')
        command = cli + ['run-pair', '--headless', '--dump-presented', '--left-script', str(script),
                         '--right-script', str(right_script), f'--{side}-dump-frame', str(frame),
                         '--max-ticks', ticks, str(BUILD / 'examples' / app['executable'])] + args
        command += ['--right', str(BUILD / 'examples' / app['executable'])]
        command += [expand(value) for value in state.get('right_args', app.get('right_args', []))]
    else:
        command = cli + ['run', '--headless', '--dump-presented', '--rtc', state.get('rtc', '2026-08-12T14:35:42'),
                         '--script', str(script), '--dump-frame', str(frame), '--max-ticks', ticks,
                         '--', str(BUILD / 'examples' / app['executable'])] + args
    result = subprocess.run(command, cwd=fixture_dir, text=True, capture_output=True, timeout=45)
    log = result.stdout + result.stderr
    (work / 'runtime.log').write_text(log)
    guarded = result.returncode == 1 and f'max virtual ticks reached ({ticks})' in log
    expected_failure = (state.get('expected_exit', 0) != 0 and
                        result.returncode == state['expected_exit'] and bool(state.get('expect')))
    if result.returncode != 0 and not guarded and not expected_failure:
        raise RuntimeError(f"{app['slug']}/{state['slug']} failed ({result.returncode}):\n{log[-3000:]}")
    for expected in state.get('expect', []):
        if expected not in log:
            raise ValueError(f"{app['slug']}/{state['slug']}: missing output {expected!r}")
    metadata = inspect_image(frame)
    destination = OUTPUT / app['slug'] / (state['slug'] + '.png')
    destination.parent.mkdir(parents=True, exist_ok=True)
    from PIL import Image
    with Image.open(frame) as image:
        image.save(destination, optimize=True)
    return metadata | {'png_sha256': hashlib.sha256(destination.read_bytes()).hexdigest(),
                       'fingerprint': fingerprint(app, state),
                       'exit': 'bounded snapshot' if guarded else 'expected failure' if expected_failure else 'normal',
                       'order': app['states'].index(state),
                       'title': state['title'], 'caption': state['caption'],
                       'src': '/assets/screenshots/gallery/' + str(destination.relative_to(OUTPUT))}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app', action='append', help='Capture only this app slug (repeatable)')
    parser.add_argument('--state', help='Capture only this state slug')
    parser.add_argument('--list', action='store_true', help='List apps and available states')
    parser.add_argument('--check', action='store_true', help='Check image integrity and source freshness without running apps')
    parser.add_argument('--no-build', action='store_true', help='Use already built executables')
    options = parser.parse_args()
    apps = json.loads(MANIFEST.read_text())['apps']
    if options.app and set(options.app) - {app['slug'] for app in apps}:
        parser.error('Unknown app slug; use --list')
    selected = [(app, state) for app in apps if not options.app or app['slug'] in options.app
                for state in app['states'] if not options.state or options.state == state['slug']]
    if not selected:
        parser.error('No matching gallery states')
    if options.list:
        for app, state in selected:
            print(f"{app['slug']}/{state['slug']}: {state['title']}")
        return
    index_path = OUTPUT / 'index.json'
    index = json.loads(index_path.read_text()) if index_path.exists() else {}
    if not options.check and not options.no_build:
        targets = sorted({app['executable'] for app, _ in selected})
        subprocess.run(['cmake', '-S', str(ROOT), '-B', str(BUILD), '-DMACHINE_LAB_WITH_SDL=ON'], check=True)
        subprocess.run(['cmake', '--build', str(BUILD), '--target', 'machinelab', *targets, '-j4'], check=True)
    errors = []
    for app, state in selected:
        key = f"{app['slug']}/{state['slug']}"
        try:
            if options.check:
                entry = index[key]
                if entry['fingerprint'] != fingerprint(app, state):
                    raise ValueError('source or replay changed; regenerate this app')
                image_path = OUTPUT / app['slug'] / (state['slug'] + '.png')
                encoded = image_path.read_bytes()
                if encoded[:16] != b'\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR':
                    raise ValueError('invalid PNG header')
                width, height = struct.unpack('>II', encoded[16:24])
                if (width, height) != (entry['width'], entry['height']) or hashlib.sha256(encoded).hexdigest() != entry['png_sha256']:
                    raise ValueError('image differs from recorded capture')
                page = ROOT / 'docs-site/examples' / (app['slug'] + '.md')
                if f'<ExampleGallery app="{app["slug"]}"' not in page.read_text():
                    raise ValueError('missing dedicated documentation gallery')
            else:
                index[key] = capture(app, state)
                index_path.write_text(json.dumps(index, indent=2, sort_keys=True) + '\n')
            print(('checked ' if options.check else 'captured ') + key, flush=True)
        except (KeyError, OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
            errors.append(f'{key}: {error}')
    if errors:
        print('\n'.join(errors), file=sys.stderr)
        raise SystemExit(1)
    print(f'{len(selected)} gallery states verified' if options.check else f'{len(selected)} gallery states captured')


if __name__ == '__main__':
    main()
