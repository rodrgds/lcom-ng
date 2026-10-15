#!/usr/bin/env python3
"""Exercise the original PvZ APK's deck, board and drag-to-plant on Android.

Requires a running emulator/device and an exported original game, including its
bundled art. The private student project is not downloaded or modified.
"""
import argparse
import io
import json
from pathlib import Path
import re
import subprocess
import time
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]


def adb(*args):
    return subprocess.check_output(['adb', *map(str, args)])


def sprite_probes(path):
    # Read this game's C XPM assets, including their two-character palette keys.
    rows = re.findall(r'^\s*"(.*)"', path.read_text(), re.M)
    width, height, colors, cpp = map(int, rows[0].split())
    palette = {}
    for line in rows[1:colors + 1]:
        color = line[cpp:].split()[-1]
        palette[line[:cpp]] = None if color == 'None' else tuple(bytes.fromhex(color[1:]))
    data = [[palette[row[x * cpp:(x + 1) * cpp]] for x in range(width)]
            for row in rows[colors + 1:colors + 1 + height]]
    # Flat interiors tolerate nearest-neighbor scaling and Android RGB565 output.
    return [(x, y, data[y][x]) for y in range(1, height - 1, 2) for x in range(1, width - 1, 2)
            if data[y][x] is not None and all(data[yy][xx] == data[y][x]
                for yy in range(y - 1, y + 2) for xx in range(x - 1, x + 2))]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('export', type=Path, help='original PvZ Android export directory')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/pvz-mobile-qa/gameplay')
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    metadata = json.loads((args.export / 'lcom-android.json').read_text())
    package = metadata['applicationId']
    assets = args.export / 'game-assets' / metadata['workingDirectory'] / 'assets'
    mower = sprite_probes(assets / 'ui/lawnmower.xpm')
    flowers = [sprite_probes(path) for path in sorted(
        (assets / 'plants/sunflower/sunflower_idle_animation').glob('*.xpm'))]
    walking = [(sprite_probes(path), offset) for kind, offset in [('regular', -34), ('conehead', -36)]
               for path in sorted((assets / 'zombies' / kind / 'walking').glob('*.xpm'))]
    assert mower and len(flowers) == 6

    def screenshot(name):
        data = adb('exec-out', 'screencap', '-p')
        (args.output / name).write_bytes(data)
        return Image.open(io.BytesIO(data)).convert('RGB')

    def wait_frame(name, expected):
        deadline = time.monotonic() + 20
        while True:
            frame = screenshot(name)
            if expected(frame): return frame
            assert time.monotonic() < deadline, f'Expected PvZ screen missing: {name}'
            time.sleep(0.1)

    adb('install', '-r', args.export / 'game.apk')
    adb('shell', 'am', 'force-stop', package)
    try:
        adb('shell', 'am', 'start', '-W', '-n', package + '/pt.rgo.lcomng.GameActivity')
        frame = wait_frame('menu.png', lambda f: f.width > f.height and
            f.getpixel((f.width // 2, f.height // 3)) == (26, 26, 46))
        # Locate the original menu background, independently of the host's layout.
        pixels = frame.load()
        xs = [x for x in range(frame.width) if pixels[x, frame.height // 2] == (26, 26, 46)]
        ys = [y for y in range(frame.height) if pixels[frame.width // 2, y] == (26, 26, 46)]
        left, top = min(xs), min(ys)
        width, height = max(xs) - left + 1, max(ys) - top + 1
        fit = min(frame.width / 800, frame.height / 600)
        assert abs(width - 800 * fit) <= 2 and abs(height - 600 * fit) <= 2, (
            frame.size, left, top, width, height)
        assert abs(left - (frame.width - width) / 2) <= 1
        assert abs(top - (frame.height - height) / 2) <= 1
        print(f'PvZ fit: display={frame.size}, game={(left, top, width, height)}', flush=True)

        def point(x, y):
            return round(left + width * x / 800), round(top + height * y / 600)

        def tap(x, y):
            adb('shell', 'input', 'tap', *point(x, y))

        def drag(x, y, end_x, end_y):
            adb('shell', 'input', 'swipe', *point(x, y), *point(end_x, end_y), 450)

        def score(image, probes, x, y):
            matched = 0
            for dx, dy, color in probes:
                actual = image.getpixel(point(x + dx + 0.5, y + dy + 0.5))
                matched += max(abs(a - b) for a, b in zip(actual, color)) <= 8
            return matched / len(probes)

        tap(400, 285)
        # The original selection screen requires dragging each card into a slot.
        for slot, (x, y) in enumerate([(260, 140), (360, 140), (460, 140),
                                      (560, 140), (260, 255), (360, 255)]):
            drag(x, y, 185 + 87 * slot, 400)
        screenshot('deck.png')
        tap(400, 490)
        wait_frame('board.png', lambda f: all(score(f, mower, 62, y) > 0.85
            for y in (110, 210, 310, 410, 510)))

        # Release on the first cell, then move the cursor far away. A held ghost
        # cannot satisfy the anchored sunflower check at its grid coordinates.
        drag(135, 45, 170, 150)
        tap(600, 450)
        offsets = [-3, 0, 0, 3, 3, 3]
        planted = lambda f: max(score(f, probes, 140 + offset, 100)
            for probes, offset in zip(flowers, offsets)) > 0.85
        wait_frame('planted.png', planted)
        tap(650, 500)
        wait_frame('planted-after-second-touch.png', planted)

        def zombie_in_lane(image):
            for sprite, offset in walking:
                # The top of a first-row zombie can overlap the HUD. Check its
                # body below that overlap, using the original walking artwork.
                probes = [p for p in sprite if p[1] >= 25]
                max_x = max(p[0] for p in probes)
                for row in (100, 200, 300, 400, 500):
                    for x in range(580, 800 - max_x):
                        if score(image, probes[::max(1, len(probes)//6)], x, row + offset) > 0.95 and \
                                score(image, probes, x, row + offset) > 0.85:
                            print(f'PvZ zombie body matched lane at x={x}, row={row}', flush=True)
                            return True
            return False

        wait_frame('zombies-in-lanes.png', zombie_in_lane)
        print('PvZ deck, mower rows, persistent drag/release planting and zombie lanes passed', flush=True)
    finally:
        adb('shell', 'am', 'force-stop', package)


if __name__ == '__main__':
    main()
