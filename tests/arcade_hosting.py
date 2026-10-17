#!/usr/bin/env python3
"""Check a running arcade Worker against original module and game-data bytes."""

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
    modules = sorted(args.source.glob("games/*/game.wasm"), key=lambda path: path.stat().st_size, reverse=True)
    data_files = sorted(args.source.glob("games/*/game.data"), key=lambda path: path.stat().st_size, reverse=True)
    assert modules, "no exported modules found"
    opener = urllib.request.build_opener()
    opener.addheaders = [("User-Agent", "lcom-ng-hosting-check/1.0")]
    results = []
    for payload in modules + data_files:
        siblings = modules if payload.name == "game.wasm" else data_files
        index = siblings.index(payload)
        content_type = "application/wasm" if payload.name == "game.wasm" else "application/octet-stream"
        url = args.url.rstrip("/") + "/" + payload.relative_to(args.source).as_posix()
        expected = hashlib.sha256(payload.read_bytes()).hexdigest()
        # Exercise all modules with normal browser encoding. Also exercise large
        # responses, HEAD, revalidation and identity without browser decompression.
        encodings = ["gzip, br"]
        if index < 2:
            encodings += ["identity", "gzip;q=0, *;q=1"]
        for encoding in encodings:
            request = urllib.request.Request(url, headers={"Accept-Encoding": encoding})
            with opener.open(request, timeout=60) as response:
                body = response.read()
                assert response.headers.get_content_type() == content_type, (url, response.headers.get_content_type(), content_type)
                compressed = response.headers.get("Content-Encoding") == "gzip"
                assert compressed == (encoding == "gzip, br"), (url, encoding)
                decoded = gzip.decompress(body) if compressed else body
                assert hashlib.sha256(decoded).hexdigest() == expected, (url, encoding)
                etag = response.headers["ETag"]
                assert "accept-encoding" in response.headers.get("Vary", "").lower()
                assert "no-transform" in response.headers.get("Cache-Control", "")
            results.append({"path": payload.relative_to(args.source).as_posix(),
                            "acceptEncoding": encoding, "sha256": expected})
        if index == 0:
            for encoding in ("gzip", "identity"):
                request = urllib.request.Request(url, method="HEAD", headers={"Accept-Encoding": encoding})
                with opener.open(request, timeout=60) as response:
                    assert (response.headers.get("Content-Encoding") == "gzip") == (encoding == "gzip")
                    assert response.headers.get_content_type() == content_type
                    assert response.read() == b""
                request = urllib.request.Request(url, headers={"Accept-Encoding": encoding, "If-None-Match": etag})
                try:
                    opener.open(request, timeout=60)
                    raise AssertionError("unchanged payload should return 304")
                except urllib.error.HTTPError as error:
                    assert error.code == 304
                    assert error.read() == b""
                request = urllib.request.Request(url, headers={"Accept-Encoding": encoding, "Range": "bytes=0-3"})
                with opener.open(request, timeout=60) as response:
                    assert response.status == 200
                    body = response.read()
                    decoded = gzip.decompress(body) if response.headers.get("Content-Encoding") == "gzip" else body
                    assert hashlib.sha256(decoded).hexdigest() == expected
    print(json.dumps({"modules": len(modules), "dataFiles": len(data_files), "checks": results}, indent=2))


if __name__ == "__main__":
    main()
