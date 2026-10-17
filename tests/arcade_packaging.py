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
            (game / "game.js").write_bytes(b"loader")
            (game / "player.js").write_bytes(b"player")
            (game / "game.data").write_bytes(b"assets")
            gallery=game/"screenshots";gallery.mkdir()
            Image.new("RGB",(640,480),(24,128,216)).save(gallery/"actual.webp",format="WEBP",quality=80)
            gallery_bytes=(gallery/"actual.webp").read_bytes()
            catalog = {"name": "Fixture", "games": [{"id": "fixture", "preview": "games/fixture/preview.png", "icon": "games/fixture/icon.png", "screenshots":["games/fixture/screenshots/actual.webp"]}]}
            (source / "collection.json").write_text(json.dumps(catalog))
            subprocess.run([sys.executable, str(ROOT / "scripts/package_arcade_web.py"), "--source", str(source), "--output", str(output)], check=True, capture_output=True)
            published = json.loads((output / "collection.json").read_text())["games"][0]
            self.assertNotIn("downloadFiles", published)
            audit = json.loads((output / "collection-provenance.json").read_text())["games"][0]
            self.assertEqual(audit["downloadFiles"][0]["path"], "games/fixture/game.js")
            widths = []
            for variant in published["iconSources"]:
                with Image.open(output / variant["path"]) as image:
                    self.assertEqual(image.width, variant["width"])
                    widths.append(image.width)
            self.assertEqual(widths, [128, 256])
            self.assertEqual((output/published["screenshots"][0]).read_bytes(),gallery_bytes)
            expected=len(gzip.compress(module,compresslevel=9,mtime=0))+len(b"loader")+len(b"player")+len(gzip.compress(b"assets",compresslevel=9,mtime=0))
            self.assertEqual(published["downloadBytes"],expected)
            self.assertEqual(audit["downloadBasis"],"gzip-wasm-data-identity-rest")
            self.assertEqual(sum(item["bytes"] for item in audit["downloadFiles"]),expected)
            for name, maximum in (("preview", (640, 480)), ("icon", (256, 256))):
                path = output / published[name]
                self.assertLess(path.stat().st_size, len(originals[name]) // 2)
                with Image.open(path) as image:
                    self.assertLessEqual(image.width, maximum[0])
                    self.assertLessEqual(image.height, maximum[1])
                    center = image.convert("RGB").getpixel((image.width//2, image.height//2))
                    self.assertTrue(all(abs(actual-expected) < 12 for actual, expected in zip(center, (24, 128, 216))))
                self.assertEqual((game / f"{name}.png").read_bytes(), originals[name])
            self.assertEqual(gzip.decompress((output / "games/fixture/game.wasm").read_bytes()), module)
            self.assertEqual(gzip.decompress((output / "games/fixture/game.data").read_bytes()), b"assets")
            # A tiny original should never grow or be upscaled for publication.
            Image.new("RGBA", (8, 8), (0, 0, 0, 0)).save(game / "icon.png")
            subprocess.run([sys.executable, str(ROOT / "scripts/package_arcade_web.py"), "--source", str(source), "--output", str(output)], check=True, capture_output=True)
            published = json.loads((output / "collection.json").read_text())["games"][0]
            self.assertLessEqual((output / published["icon"]).stat().st_size, (game / "icon.png").stat().st_size)
            with Image.open(output / published["icon"]) as image:
                self.assertEqual(image.size, (8, 8))

    def test_large_game_data_preserves_bytes_and_fits_the_asset_limit(self):
        with tempfile.TemporaryDirectory() as directory:
            source, output = Path(directory)/"export", Path(directory)/"publish"
            game = source/"games/large"
            game.mkdir(parents=True)
            original = bytes(range(256)) * (26 * 1024 * 1024 // 256)
            self.assertGreater(len(original), 25 * 1024 * 1024)
            (game/"game.data").write_bytes(original)
            (game/"game.wasm").write_bytes(b"\0asm\1\0\0\0")
            (game/"game.js").write_bytes(b"loader")
            (source/"collection.json").write_text(json.dumps({"games":[{"id":"large"}]}))
            result = subprocess.run([sys.executable,str(ROOT/"scripts/package_arcade_web.py"),
                                     "--source",str(source),"--output",str(output)],
                                    capture_output=True,text=True)
            self.assertEqual(result.returncode,0,result.stderr)
            stored = (output/"games/large/game.data").read_bytes()
            self.assertLess(len(stored), 25 * 1024 * 1024)
            self.assertEqual(gzip.decompress(stored), original)
            self.assertEqual((game/"game.data").read_bytes(), original)
            game_info = json.loads((output/"collection-provenance.json").read_text())["games"][0]
            data_info = next(item for item in game_info["downloadFiles"] if item["path"].endswith("game.data"))
            self.assertEqual(data_info,{"path":"games/large/game.data","bytes":len(stored),
                                        "encoding":"gzip","decodedBytes":len(original)})
            self.assertEqual(game_info["downloadBytes"],sum(item["bytes"] for item in game_info["downloadFiles"]))


if __name__ == "__main__":
    unittest.main()
