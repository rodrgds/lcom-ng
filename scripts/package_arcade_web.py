#!/usr/bin/env python3
"""Prepare the exported arcade for Cloudflare's per-file asset limit."""

import argparse
import gzip
import hashlib
import json
from pathlib import Path
import shutil


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
    records = []
    for game in catalog["games"]:
        relative = Path("games") / game["id"] / "game.wasm"
        original = (source / relative).read_bytes()
        encoded = gzip.compress(original, compresslevel=9, mtime=0)
        assert gzip.decompress(encoded) == original
        (output / relative).write_bytes(encoded)
        records.append({"path": relative.as_posix(), "bytes": len(original),
                        "gzipBytes": len(encoded),
                        "sha256": hashlib.sha256(original).hexdigest()})
    # The deployment Worker serves these bytes with manual gzip encoding. A
    # static _headers encoding rule would cause a second compression pass.
    for path in output.rglob("*"):
        if path.is_file() and path.stat().st_size > 25 * 1024 * 1024:
            parser.error(f"asset exceeds 25 MiB: {path}")
    inventory = {"games": len(records), "modules": records}
    (output.parent / "arcade-publish-inventory.json").write_text(
        json.dumps(inventory, indent=2) + "\n")
    print(f"Prepared {len(records)} losslessly compressed modules in {output}")


if __name__ == "__main__":
    main()
