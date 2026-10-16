# LCOM Arcade

A collection of original student games and the Ninjix showcase. `lcom.collection.json` lists their
folders; each game's `lcom.json` owns its name, icon, description, preview screenshot and mobile controls.

```sh
lcom-ng web collections/arcade
lcom-ng export web collections/arcade --output dist/lcom-arcade-web
lcom-ng export android collections/arcade --output dist/lcom-arcade-android
```

The Android launcher is native and fullscreen, with columns adapting to screen
width and text size. The web library uses a shuffled square icon mosaic, with
larger featured games spread down the grid. Hover or keyboard focus expands a
tile into sliding gameplay screenshots, its name, evidenced year and download
size and a public Repo link when available. Search opens a modal. Loading shows the selected game's icon, description
and measured download progress. The Games button prefers unused
space outside the game and returns to the collection.
Normal in-game Quit actions return to the library automatically.

The checked-in gallery has 196 real captures across 66 games. See
[`galleries/README.md`](galleries/README.md) for capture and validation commands.
Three authored launcher illustrations replace weak icons; exact image-generation
prompts and source hashes are in [`icons/generation.json`](icons/generation.json).

See [the collection guide](../../docs-site/instructors/game-collections.md) for
creating a class collection, Android prerequisites, per-game settings and limits.
Source provenance, verification and licensing are in
[`examples/community/catalog.json`](../../examples/community/catalog.json).

Publish the verified web export to `arcade.rgo.pt` through the repository's
Cloudflare account:

```sh
devenv shell -- python3 scripts/package_arcade_web.py
devenv shell -- bunx wrangler deploy --config collections/arcade/wrangler.jsonc
```

The packaging step copies the export into `build/arcade-publish`, losslessly
compresses Wasm modules and generates smaller WebP previews (up to 640 × 480)
and icons (256 × 256, or 512 × 512 for featured tiles). Original captures stay unchanged. Content-based
filenames refresh image caches when the artwork changes; an original is kept
when it is already smaller. A small Worker serves their encoding correctly and
streams decompression for clients requesting identity encoding. This keeps large
modules within the host's asset limit. Other files use static serving. The regular
export stays directly usable on other static servers. Check all modules with
browser gzip encoding, plus large identity responses and cache revalidation,
then test the live player:

```sh
devenv shell -- python3 tests/arcade_hosting.py --url https://arcade.rgo.pt
```
