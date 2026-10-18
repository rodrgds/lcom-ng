# Arcade interaction refinements, 18 October 2026

These interaction changes were verified locally before the runtime follow-up.
See [browser runtime performance](web-runtime-performance.md) for the combined
release and its deployment evidence.

## Changes

- Restored the original Search dialog to launch individual games by name.
  Search covers the complete catalog without changing the mosaic's filters.
  Returning from a searched game restores its query and focused result.
  Cached result rows use lazy thumbnails; names are indexed once.
- Filters remain a separate tile and floating panel. The active-filter bar above
  the grid is removed. Facet selections persist through gameplay and reloads.
  Visible cards animate moving, entering and leaving. Layout reads are batched,
  interrupted moves are canceled, and clearing restores the same highlights
  and variant groups.
- Normal and compact previews use the normal tile height. Featured previews use
  their own larger tile height. All retain the landscape aspect ratio and center
  on their tile, clamped to the viewport. Preview layers sit above search.
- Gameplay suggestions shuffle a bounded 48-game deck from the entire catalog on
  each launch, including after a narrow search. Columns alternate direction and
  loop continuously. Pause buttons are removed; hover, focus, hidden-page and
  reduced-motion behavior remain.
- Games start on their actual first frame, without the artificial two-second
  delay in either the collection or browser player.
- Each shuffle selects ten featured games from 28 curated candidates. The pool
  includes Ninjix, MINIX Grand Prix, Sushi Slash, Teclas, Euler's Way, Gold Miner,
  Tennis for Two, Snook, Plants vs. Zombies, Penaltix, Age of War, Escape the Lab,
  FEUP Hero, Penguin Run and Fireboy & Watergirl. The remaining candidates retain
  varied puzzle, action, racing and strategy mechanics.
- FEUP Hero has new launcher artwork. The generation prompt, processing and
  hashes are recorded in `collections/arcade/icons/generation.json`.
- Shift+Escape releases a captured mouse first; another press returns to the
  library. Delivered Escape stays guest input. Fullscreen Keyboard Lock retains
  capture where supported; browser-owned unlock still forwards Escape once.
- Penaltix's desktop shot was verified with real mouse input: hold the right
  mouse button, drag to aim, then release. The run-up, ball flight and goalkeeper
  reaction were captured. Its description now explains the gesture.

## Local performance measurements

Three fresh Chrome contexts per version and viewport, DPR 1, with 4x CPU
throttling. Each run applies the same 18 query updates, giving 54 samples per
viewport. Both versions use local static hosting on a shared development machine.
These are browsing measurements, not runtime profiles, field INP or deployed
results. The previous inline search repacks the mosaic; the restored dialog
updates its independent result list.

| Median | Previous inline search | Restored search dialog |
| --- | ---: | ---: |
| Desktop input handler | 18.5 ms | 1.0 ms |
| Desktop next-paint opportunity | 40.6 ms | 16.0 ms |
| Mobile input handler | 13.1 ms | 0.9 ms |
| Mobile next-paint opportunity | 25.5 ms | 15.3 ms |

Desktop is 1440x900; mobile viewport is 390x844. The paint measure waits for an
animation frame followed by a task after dispatching input. Search updates do
about 95% less handler work on desktop and 93% less on mobile in this sample.
First opening the dialog, including creating the cached result rows, takes
124.1 ms desktop and 115.2 ms mobile to the next-paint opportunity at 4x CPU.
Separate facet changes, including animated grid layout, take 73.1 ms desktop
and 51.0 ms mobile by the same measure. These costs are not hidden in the search
update figures. The earlier indexing, thumbnails, compact catalog and deferred
preview construction are retained. See the historical
[performance report](arcade-performance.md) for the earlier catalog optimization.

Removing the minimum timer eliminates up to two seconds of forced waiting on
fast loads. Readiness still requires an actual presented frame. The compact
browsing catalog is 451,393 bytes, with the full inventory kept separately.

Local measurements and captures are under `build/arcade-refinements/`, including
`discovery-comparison.json`, `measure-discovery.mjs`, preview captures and Penaltix
shot frames. Raw browsing samples are under `build/arcade-perf/`, named
`previous-inline.json` and `restored-modal.json`.

## Verification

- The earlier player, callback and startup suites passed all 69 cases.
  After restoring separate search, all 39 collection/navigation cases passed
  across the full run and focused corrections. The real audio sampling check
  missed a short Shuffle sound once under load and passed unchanged on rerun.
  Two exported-player collection scenarios were rerun for the restored dialog.
- Packaging and Worker delivery checks, docs checks and 99 gallery states
  passed. The source audit verified all 305 pinned projects and 33,173 files.
- All 306 WASM modules and 115 game data files retain their previous decoded
  SHA-256 hashes. Original game sources and in-game artwork were preserved.
- Rendered desktop, 390px and 320px layouts were inspected. Six side columns fit
  the representative wide player, with no pause buttons or page errors.
- Runtime profiling and optimization were subsequently requested and completed
  separately; see the linked runtime report.
