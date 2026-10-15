#!/usr/bin/env python3
"""Check a running arcade Worker against its original exported module bytes."""

import argparse
import gzip
import hashlib
import json
from pathlib import Path
import urllib.error
import urllib.request


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--url", required=True)
    parser.add_argument("--source", type=Path, default=Path("dist/lcom-arcade-web"))
    args = parser.parse_args()
    modules = sorted(args.source.glob("games/*/game.wasm"),
                     key=lambda path: path.stat().st_size, reverse=True)
    assert modules, "no exported modules found"
    opener = urllib.request.build_opener()
    opener.addheaders = [("User-Agent", "lcom-ng-hosting-check/1.0")]
    results = []
    for index, module in enumerate(modules):
        url = args.url.rstrip("/") + "/" + module.relative_to(args.source).as_posix()
        expected = hashlib.sha256(module.read_bytes()).hexdigest()
        # Exercise all modules with normal browser encoding. Also exercise large
        # responses, HEAD, revalidation and identity without browser decompression.
        encodings = ["gzip, br"]
        if index < 2:
            encodings += ["identity"]
        for encoding in encodings:
            request = urllib.request.Request(url, headers={"Accept-Encoding": encoding})
            with opener.open(request, timeout=60) as response:
                body = response.read()
                assert response.headers.get_content_type() == "application/wasm"
                compressed = response.headers.get("Content-Encoding") == "gzip"
                assert compressed == (encoding == "gzip, br"), (url, encoding)
                decoded = gzip.decompress(body) if compressed else body
                assert hashlib.sha256(decoded).hexdigest() == expected, (url, encoding)
                etag = response.headers["ETag"]
            results.append({"path": module.relative_to(args.source).as_posix(),
                            "acceptEncoding": encoding, "sha256": expected})
        if index == 0:
            request = urllib.request.Request(url, method="HEAD", headers={"Accept-Encoding": "gzip"})
            with opener.open(request, timeout=60) as response:
                assert response.headers["Content-Encoding"] == "gzip"
                assert response.read() == b""
            request = urllib.request.Request(url, headers={"Accept-Encoding": "identity", "If-None-Match": etag})
            try:
                opener.open(request, timeout=60)
                raise AssertionError("unchanged module should return 304")
            except urllib.error.HTTPError as error:
                assert error.code == 304
                assert error.read() == b""
    print(json.dumps({"modules": len(modules), "checks": results}, indent=2))


if __name__ == "__main__":
    main()
