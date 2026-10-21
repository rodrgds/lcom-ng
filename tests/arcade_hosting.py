#!/usr/bin/env python3
"""Check a running arcade Worker against original module and game-data bytes."""

import argparse
import gzip
import hashlib
import json
from pathlib import Path
import urllib.error
import urllib.request


def check_share_pages(opener, url, source):
    """A shared address must carry its own game metadata and root-relative assets."""
    catalog = json.loads((source / "collection.json").read_text())
    games = [game for game in catalog.get("games", []) if game.get("slug")]
    assert games, "no slug in the exported catalog"
    for game in (games[0], games[len(games) // 2]):
        share = url.rstrip("/") + "/share/" + game["slug"]
        request = urllib.request.Request(share, headers={"Accept-Encoding": "identity"})
        with opener.open(request, timeout=60) as response:
            assert response.headers.get_content_type() == "text/html", (share, response.headers.get_content_type())
            assert "max-age" in response.headers.get("Cache-Control", ""), (share, response.headers.get("Cache-Control"))
            html = response.read().decode()
        assert '<base href="/">' in html, share
        assert f'<link rel="canonical" href="{share}">' in html, share
        assert f'Play {game["name"]}' in html, share
        assert 'property="og:title"' in html, share
    unknown = url.rstrip("/") + "/share/not-a-real-slug"
    with opener.open(urllib.request.Request(unknown, headers={"Accept-Encoding": "identity"}), timeout=60) as response:
        assert response.status == 200 and '<base href="/">' in response.read().decode(), unknown


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
                # The edge normalizes wildcard negotiation before invoking the
                # Worker. Its cf.clientAcceptEncoding can report "gzip" for
                # "gzip;q=0, *;q=1". Verify those delivered bytes independently;
                # exact quality handling is covered at the Worker boundary.
                if encoding != "gzip;q=0, *;q=1":
                    assert compressed == (encoding == "gzip, br"), (url, encoding)
                decoded = gzip.decompress(body) if compressed else body
                assert hashlib.sha256(decoded).hexdigest() == expected, (url, encoding)
                etag = response.headers["ETag"]
                assert "accept-encoding" in response.headers.get("Vary", "").lower()
                assert "no-transform" in response.headers.get("Cache-Control", "")
            results.append({"path": payload.relative_to(args.source).as_posix(),
                            "acceptEncoding": encoding, "contentEncoding": "gzip" if compressed else "identity",
                            "sha256": expected})
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
    check_share_pages(opener, args.url, args.source)
    print(json.dumps({"modules": len(modules), "dataFiles": len(data_files), "checks": results}, indent=2))


if __name__ == "__main__":
    main()
