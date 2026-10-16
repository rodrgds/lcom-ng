#!/usr/bin/env python3
"""Prepare lightweight Arcade images and compressed Cloudflare game modules."""

import argparse
import gzip
import hashlib
import json
from io import BytesIO
from pathlib import Path
import shutil

from PIL import Image


def package_image(source, output, relative, bounds):
    original = (source / relative).read_bytes()
    with Image.open(source / relative) as image:
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
    records, images = [], []
    for game in catalog["games"]:
        for kind, bounds in (("preview", (640, 480)), ("icon", (128, 128))):
            if not game.get(kind):
                continue
            relative, before, after = package_image(source, output, game[kind], bounds)
            game[kind] = relative
            images.append({"path": relative, "bytes": before, "publishedBytes": after})
        relative = Path("games") / game["id"] / "game.wasm"
        original = (source / relative).read_bytes()
        encoded = gzip.compress(original, compresslevel=9, mtime=0)
        assert gzip.decompress(encoded) == original
        (output / relative).write_bytes(encoded)
        records.append({"path": relative.as_posix(), "bytes": len(original),
                        "gzipBytes": len(encoded),
                        "sha256": hashlib.sha256(original).hexdigest()})
    (output / "collection.json").write_text(json.dumps(catalog, indent=2) + "\n")
    # The deployment Worker serves these bytes with manual gzip encoding. A
    # static _headers encoding rule would cause a second compression pass.
    for path in output.rglob("*"):
        if path.is_file() and path.stat().st_size > 25 * 1024 * 1024:
            parser.error(f"asset exceeds 25 MiB: {path}")
    inventory = {"games": len(records), "modules": records, "images": images}
    (output.parent / "arcade-publish-inventory.json").write_text(
        json.dumps(inventory, indent=2) + "\n")
    print(f"Prepared {len(records)} losslessly compressed modules in {output}")
    print(f"Images: {sum(image['bytes'] for image in images):,} to "
          f"{sum(image['publishedBytes'] for image in images):,} bytes")


if __name__ == "__main__":
    main()
