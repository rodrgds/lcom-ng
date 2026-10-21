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
size and a public Repo link when available. Full sets of nine variants in a game
family share a compact 3×3 group, with each game individually playable.
Search filters the visible grid by name or tag and stays beside one filter panel
for genres, player modes and
academic school years. The query and tags persist within the tab; filtered results use
regular tiles and keep their order while typing. Five random featured picks come from a curated pool. Year labels stay plain and unknowns are omitted.
Loading opens a compact dismissible modal with the selected game's icon,
description, category tags and measured progress. The modal stays for at least
two seconds without running gameplay behind it. Empty side gutters show brighter clickable
suggestions in one to four slowly scrolling columns per side, alternating up and down.
Hover or focus pauses a side; its pause button stops it until resumed. Reduced motion
keeps suggestions still. They retain the same gameplay previews. On touchscreens, the Home button prefers unused
space outside the game and returns to the collection. Home stays visible for keyboard and touch players. After mouse capture, a centered
Shift + Esc hint appears after one second and fades away four seconds later.
Shift+Escape also returns to the library.
Click the game to capture the mouse; Escape releases it
and reaches the game once. The upper-right button toggles fullscreen.
Normal in-game Quit actions return to the library automatically.
On a mouse or trackpad, the upper-right **Share** button copies a clean
per-game address such as `https://arcade.rgo.pt/share/afg-ninjix`. Those
initials come from each participant's first name, in alphabetical order, and
only from evidenced author lists in `game-metadata.json`; games without
evidence use a name-only slug. Touch layouts keep that corner for the game's
own controls, so phones open shared addresses without the button. The Worker
answers `/share/<slug>` with the same launcher, a `<base href="/">`, the
game's title and preview for link unfurls, and no side suggestion rails, so a
shared link opens one game on a plain background. Slugs are assigned by
`sdk/collection/share.py` and checked by `tests/collection_export.py`.

The checked-in gallery publishes real gameplay captures with per-image provenance. See
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
compresses Wasm modules and game data, and generates smaller WebP previews (up to 640 × 480)
and responsive icons (128, 256 and, for featured candidates, 512 pixels wide).
The browser chooses an icon for the tile's size and screen density. Startup uses
the small thumbnail. Original captures stay unchanged. Content-based
filenames refresh image caches when the artwork changes; an original is kept
when it is already smaller. A small Worker serves their encoding correctly and
streams decompression for clients requesting identity encoding. This keeps large
payloads within the host's asset limit. Other files use static serving. The regular
export stays directly usable on other static servers. The published `collection.json`
contains browsing data; `collection-provenance.json` retains the complete delivery
inventory and historical evidence without loading it on every visit.
The launcher creates hover-preview markup only when opened. Check modules and game data with
browser gzip encoding, plus large identity responses and cache revalidation,
then test the live player:

```sh
devenv shell -- python3 tests/arcade_hosting.py --url https://arcade.rgo.pt
```

The launcher font uses a Latin and punctuation WOFF2 subset of the original
variable font. Keep `assets/OFL.txt` and the original TTF alongside it. Regenerate
with a one-off Nix environment:

```sh
nix shell --impure --expr 'let pkgs = import <nixpkgs> {}; in pkgs.python3.withPackages (ps: [ ps.fonttools ps.brotli ])' -c pyftsubset sdk/collection/web-template/assets/bricolage.ttf --unicodes=U+0000-024F,U+2000-206F,U+20AC --flavor=woff2 --output-file=sdk/collection/web-template/assets/bricolage.woff2 --layout-features='*'
```

See the [performance measurements and verification](../../docs/arcade-performance.md).
