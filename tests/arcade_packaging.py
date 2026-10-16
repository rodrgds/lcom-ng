#!/usr/bin/env python3
"""Exercise the hosted Arcade's image and module delivery contracts."""

import gzip
import json
from pathlib import Path
import random
import subprocess
import sys
import tempfile
import unittest

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]


class ArcadePackagingTest(unittest.TestCase):
    def test_display_images_are_small_and_originals_are_preserved(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "export"
            output = Path(directory) / "publish"
            game = source / "games" / "fixture"
            game.mkdir(parents=True)
            originals = {}
            for name, size in (("preview", (1024, 768)), ("icon", (512, 512))):
                image = Image.frombytes("RGB", size, random.Random(42).randbytes(size[0] * size[1] * 3))
                ImageDraw.Draw(image).rectangle((size[0]//4, size[1]//4, size[0]*3//4, size[1]*3//4), fill=(24, 128, 216))
                image.save(game / f"{name}.png")
                originals[name] = (game / f"{name}.png").read_bytes()
            module = b"\x00asm\x01\x00\x00\x00"
            (game / "game.wasm").write_bytes(module)
            catalog = {"name": "Fixture", "games": [{"id": "fixture", "preview": "games/fixture/preview.png", "icon": "games/fixture/icon.png"}]}
            (source / "collection.json").write_text(json.dumps(catalog))
            subprocess.run([sys.executable, str(ROOT / "scripts/package_arcade_web.py"), "--source", str(source), "--output", str(output)], check=True, capture_output=True)
            published = json.loads((output / "collection.json").read_text())["games"][0]
            for name, maximum in (("preview", (640, 480)), ("icon", (128, 128))):
                path = output / published[name]
                self.assertLess(path.stat().st_size, len(originals[name]) // 2)
                with Image.open(path) as image:
                    self.assertLessEqual(image.width, maximum[0])
                    self.assertLessEqual(image.height, maximum[1])
                    center = image.convert("RGB").getpixel((image.width//2, image.height//2))
                    self.assertTrue(all(abs(actual-expected) < 12 for actual, expected in zip(center, (24, 128, 216))))
                self.assertEqual((game / f"{name}.png").read_bytes(), originals[name])
            self.assertEqual(gzip.decompress((output / "games/fixture/game.wasm").read_bytes()), module)
            # A tiny original should never grow or be upscaled for publication.
            Image.new("RGBA", (8, 8), (0, 0, 0, 0)).save(game / "icon.png")
            subprocess.run([sys.executable, str(ROOT / "scripts/package_arcade_web.py"), "--source", str(source), "--output", str(output)], check=True, capture_output=True)
            published = json.loads((output / "collection.json").read_text())["games"][0]
            self.assertLessEqual((output / published["icon"]).stat().st_size, (game / "icon.png").stat().st_size)
            with Image.open(output / published["icon"]) as image:
                self.assertEqual(image.size, (8, 8))


if __name__ == "__main__":
    unittest.main()
