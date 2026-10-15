# Student game sources

These imported LCOM student projects are pinned imports for compatibility examples
and the [LCOM Arcade collection](../../collections/arcade/README.md). Build from the
folder named by each entry's `path` in [catalog.json](./catalog.json).

Each project retains its source/assets and upstream attribution. Added `lcom.json`
profiles select mobile controls, export assets, names and game-derived icons.
`UPSTREAM.json` records the source URL, revision, hashes and any deliberate patches.
Confirmed student defects have separately recorded patches, including video-memory
cleanup and menu/cursor ordering. Other imported game C and header files are unchanged.

The root MIT license does not relicense these projects or their artwork. Upstream
license files are retained when present. Most imports have no explicit upstream
license; check the recorded status and obtain permission before redistribution.
Generated binaries, bundled Minix headers and unrelated authoring/build artifacts
are omitted. Original Makefiles and dependencies used by each game are retained.

Verify pinned originals and reconstruct every documented source correction with:

```sh
devenv shell -- python3 scripts/check_collection_sources.py
```
