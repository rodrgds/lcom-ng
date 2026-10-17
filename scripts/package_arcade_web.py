#!/usr/bin/env python3
"""Prepare lightweight Arcade images and compressed Cloudflare game payloads."""

import argparse
import gzip
import hashlib
import json
from io import BytesIO
from pathlib import Path
import shutil
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "sdk"))
from collection.web import download_metadata

from PIL import Image


def package_image(source, output, relative, bounds):
    relative = Path(relative)
    original_path = (source / relative).resolve(strict=True)
    if relative.is_absolute() or '..' in relative.parts or not original_path.is_relative_to(source):
        raise ValueError(f"Image path escapes export: {relative}")
    relative = relative.as_posix()
    original = original_path.read_bytes()
    with Image.open(source / relative) as image:
        if image.format == "WEBP" and image.width <= bounds[0] and image.height <= bounds[1]:
            image.load()
            return relative, len(original), len(original)
        image.thumbnail(bounds, Image.Resampling.LANCZOS)
        encoded = BytesIO()
        image.save(encoded, format="WEBP", quality=80, method=6)
    data = encoded.getvalue()
    if len(data) >= len(original):
        return relative, len(original), len(original)
    digest = hashlib.sha256(data).hexdigest()[:12]
    target = Path(relative).with_name(f"{Path(relative).stem}-{digest}.webp")
    (output / target).write_bytes(data)
    return target.as_posix(), len(original), len(data)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=Path("dist/lcom-arcade-web"))
    parser.add_argument("--output", type=Path, default=Path("build/arcade-publish"))
    args = parser.parse_args()
    source, output = args.source.resolve(), args.output.resolve()
    if source == output or source in output.parents or output in source.parents:
        parser.error("source and output must be separate directories")
    catalog = json.loads((source / "collection.json").read_text())
    if output.exists():
        shutil.rmtree(output)
    shutil.copytree(source, output)
    records, data_records, images = [], [], []
    for game in catalog["games"]:
        icon_bounds = (512, 512) if game.get("featured") else (256, 256)
        for kind, bounds in (("preview", (640, 480)), ("icon", icon_bounds)):
            if not game.get(kind):
                continue
            relative, before, after = package_image(source, output, game[kind], bounds)
            game[kind] = relative
            images.append({"path": relative, "bytes": before, "publishedBytes": after})
        screenshots=[]
        for screenshot in game.get("screenshots", []):
            relative, before, after = package_image(source, output, screenshot, (640, 480))
            screenshots.append(relative)
            images.append({"path": relative, "bytes": before, "publishedBytes": after})
        game["screenshots"] = screenshots
        relative = Path("games") / game["id"] / "game.wasm"
        original = (source / relative).read_bytes()
        encoded = gzip.compress(original, compresslevel=9, mtime=0)
        assert gzip.decompress(encoded) == original
        (output / relative).write_bytes(encoded)
        records.append({"path": relative.as_posix(), "bytes": len(original),
                        "gzipBytes": len(encoded),
                        "sha256": hashlib.sha256(original).hexdigest()})
        data_relative = relative.with_name("game.data")
        data_bytes = None
        if (source / data_relative).is_file():
            original_data = (source / data_relative).read_bytes()
            encoded_data = gzip.compress(original_data, compresslevel=9, mtime=0)
            assert gzip.decompress(encoded_data) == original_data
            (output / data_relative).write_bytes(encoded_data)
            data_bytes = len(encoded_data)
            data_records.append({"path": data_relative.as_posix(), "bytes": len(original_data),
                                 "gzipBytes": data_bytes,
                                 "sha256": hashlib.sha256(original_data).hexdigest()})
        game.update(download_metadata(source / "games" / game["id"], f"games/{game['id']}/",
                                      len(encoded), data_bytes=data_bytes))
    (output / "collection.json").write_text(json.dumps(catalog, indent=2) + "\n")
    # The deployment Worker serves these bytes with manual gzip encoding. A
    # static _headers encoding rule would cause a second compression pass.
    for path in output.rglob("*"):
        if path.is_file() and path.stat().st_size > 25 * 1024 * 1024:
            parser.error(f"asset exceeds 25 MiB: {path}")
    inventory = {"games": len(records), "modules": records, "data": data_records, "images": images}
    (output.parent / "arcade-publish-inventory.json").write_text(
        json.dumps(inventory, indent=2) + "\n")
    print(f"Prepared {len(records)} losslessly compressed modules and {len(data_records)} data files in {output}")
    print(f"Images: {sum(image['bytes'] for image in images):,} to "
          f"{sum(image['publishedBytes'] for image in images):,} bytes")


if __name__ == "__main__":
    main()
