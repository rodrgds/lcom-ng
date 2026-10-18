#!/usr/bin/env python3
"""Prepare lightweight Arcade images and compressed Cloudflare game payloads."""

import argparse
import base64
import gzip
import hashlib
import json
import re
from io import BytesIO
from pathlib import Path
import shutil
import sys
import subprocess
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "sdk"))
from collection.web import download_metadata

from PIL import Image


def fingerprint(output, name, data):
    name = Path(name)
    digest = hashlib.sha256(data).hexdigest()[:12]
    target = Path('static') / f'{name.stem}-{digest}{name.suffix}'
    (output / target).parent.mkdir(exist_ok=True)
    (output / target).write_bytes(data)
    return target.as_posix()


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
            return fingerprint(output, relative, original), len(original), len(original)
        image.thumbnail(bounds, Image.Resampling.LANCZOS)
        encoded = BytesIO()
        image.save(encoded, format="WEBP", quality=80, method=6)
    data = encoded.getvalue()
    if len(data) >= len(original):
        return fingerprint(output, relative, original), len(original), len(original)
    return fingerprint(output, Path(relative).with_suffix('.webp'), data), len(original), len(data)


def package_shell(source, output, catalog):
    if not (source / 'index.html').is_file():
        return
    shell = (source / 'index.html').read_text()
    replacements = {}
    for name in ('assets/bricolage.woff2', 'assets/cursor.svg', 'assets/arcade.svg', 'icon.png', 'favicon.png'):
        if (source / name).is_file():
            replacements[name] = fingerprint(output, name, (source / name).read_bytes())
            shell = re.sub(r'''(["'])''' + re.escape(name) + r'''(?:\?v=[\w.-]+)?\1''',
                           lambda match: match[1] + replacements[name] + match[1], shell)
    css = (source / 'launcher.css').read_text()
    for name, path in replacements.items():
        css = css.replace(name, Path(path).name)
    css_path = fingerprint(output, 'launcher.css', css.encode())
    # Exported module graphs carry query versions for unbundled SDK hosting.
    # Strip only those versions in temporary inputs; preserve the source export.
    with tempfile.TemporaryDirectory(prefix='arcade-shell-') as directory:
        staging = Path(directory)
        for script in source.glob('*.js'):
            code = re.sub(r'''(\./[\w.-]+\.js)\?v=[a-f0-9]+(?=["'])''', r'\1', script.read_text())
            for name, path in replacements.items():
                code = code.replace(name, path)
            (staging / script.name).write_text(code)
        target = staging / 'bundle.mjs'
        subprocess.run(['bun', 'build', str(staging / 'launcher.js'), '--target', 'browser',
                        '--format', 'esm', '--minify', '--outfile', str(target)],
                       check=True, capture_output=True, text=True)
        bundle_path = fingerprint(output, 'launcher.js', target.read_bytes())
    catalog_path = fingerprint(output, 'collection.json', catalog)
    shell = re.sub(r'launcher\.css(?:\?v=[\w.-]+)?', css_path, shell)
    shell = re.sub(r'launcher\.js(?:\?v=[\w.-]+)?', bundle_path, shell)
    shell = shell.replace('href="collection.json"', f'href="{catalog_path}"')
    (output / 'index.html').write_text(shell)


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
    # Payloads are written compressed below. Copying their decoded form first
    # wastes staging space and can exceed the host's per-file limit.
    def skip_payloads(directory, names):
        return {"game.wasm", "game.data"} if Path(directory).parent == source / "games" else set()
    shutil.copytree(source, output, ignore=skip_payloads)
    records, data_records, images = [], [], []
    for game in catalog["games"]:
        icon_bounds = (512, 512) if game.get("featured") or game.get("featuredPool") else (256, 256)
        original_icon = game.get("icon")
        for kind, bounds in (("preview", (640, 480)), ("icon", icon_bounds)):
            if not game.get(kind):
                continue
            relative, before, after = package_image(source, output, game[kind], bounds)
            game[kind] = relative
            images.append({"path": relative, "bytes": before, "publishedBytes": after})
        if original_icon:
            variants = {}
            for bound in sorted({128, 256, icon_bounds[0]}):
                relative, before, after = package_image(source, output, original_icon, (bound, bound))
                with Image.open(output / relative) as image:
                    variants[image.width] = relative
                images.append({"path": relative, "bytes": before, "publishedBytes": after})
            game["iconSources"] = [{"path": path, "width": width} for width, path in sorted(variants.items())]
            game["iconThumbnail"] = game["iconSources"][0]["path"]
        original_shots = game.get('screenshots', [])
        screenshots=[]
        for screenshot in original_shots:
            relative, before, after = package_image(source, output, screenshot, (640, 480))
            screenshots.append(relative)
            images.append({"path": relative, "bytes": before, "publishedBytes": after})
        game["screenshots"] = screenshots
        # Read the original capture rather than recompressing its display variant.
        if original_shots:
            relative, before, after = package_image(source, output, original_shots[0], (160, 120))
            game['screenshotThumbnail'] = relative
            images.append({'path':relative, 'bytes':before, 'publishedBytes':after})
            with Image.open(source / original_shots[0]) as image:
                image.thumbnail((32, 24), Image.Resampling.LANCZOS)
                encoded = BytesIO()
                image.save(encoded, format='WEBP', quality=25, method=6)
            game['screenshotPlaceholder'] = 'data:image/webp;base64,' + base64.b64encode(encoded.getvalue()).decode()
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
    # Preserve the complete delivery inventory and historical evidence for audits,
    # without parsing it in every visitor's critical browsing path.
    (output / "collection-provenance.json").write_text(json.dumps(catalog, ensure_ascii=False) + "\n")
    library = {**catalog, "provenance": "collection-provenance.json", "games": []}
    for game in catalog["games"]:
        item = {key: value for key, value in game.items() if key not in ("downloadFiles", "downloadBasis")}
        if "academicYear" in item:
            for key in ("year", "yearLabel", "yearSource"):
                item.pop(key, None)
        library["games"].append(item)
    library_bytes = (json.dumps(library, ensure_ascii=False, separators=(",", ":")) + "\n").encode()
    (output / "collection.json").write_bytes(library_bytes)
    package_shell(source, output, library_bytes)
    (output / '_headers').write_text('/static/*\n  Cache-Control: public, max-age=31536000, immutable\n')
    # The deployment Worker serves these bytes with manual gzip encoding. A
    # static _headers encoding rule would cause a second compression pass.
    for path in output.rglob("*"):
        if path.is_file() and path.stat().st_size > 25 * 1024 * 1024:
            parser.error(f"asset exceeds 25 MiB: {path}")
    images = list({image["path"]: image for image in images}.values())
    inventory = {"games": len(records), "modules": records, "data": data_records, "images": images}
    (output.parent / "arcade-publish-inventory.json").write_text(
        json.dumps(inventory, indent=2) + "\n")
    print(f"Prepared {len(records)} losslessly compressed modules and {len(data_records)} data files in {output}")
    print(f"Published images: {len(images)} variants, "
          f"{sum(image['publishedBytes'] for image in images):,} bytes")


if __name__ == "__main__":
    main()
