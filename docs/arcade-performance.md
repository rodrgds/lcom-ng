# Arcade performance, 17 October 2026

Published at <https://arcade.rgo.pt/>. Worker version: `03d48dc2-6eec-4c27-b753-f63cd2309b0a`.
Implementation: `ed79de40`, issue [#19](https://github.com/rodrgds/lcom-ng/issues/19).

## Measured improvement

Three fresh Chromium sessions at each viewport, with a fixed shuffle seed, 4× CPU
slowdown and an unthrottled network. Both versions contain the same 306 games.
Values are medians, using the live production site before and after deployment.

| Measure | Desktop before | Desktop after | Mobile viewport before | Mobile viewport after |
| --- | ---: | ---: | ---: | ---: |
| Search input handler | 57.3 ms | 7.7 ms | 54.1 ms | 8.2 ms |
| Search through next paint opportunity | 90.5 ms | 21.6 ms | 72.0 ms | 17.4 ms |
| Initial asset bodies | 583,169 B | 470,912 B | 445,267 B | 312,975 B |
| Largest contentful paint | 1,152 ms | 968 ms | 1,044 ms | 812 ms |
| Initial DOM elements | 6,712 | 1,521 | 6,712 | 1,521 |
| Initial image bodies | 181,461 B | 178,952 B | 43,549 B | 20,962 B |

Search handler work fell 85–87%; search-to-paint time fell 76%. Initial asset
bodies fell 19% on desktop and 30% at the mobile viewport. Initial DOM size fell
77%. The published browsing catalog shrank from 968,098 to 450,714 bytes before
HTTP compression, 53% smaller. Layout shift was unchanged, at 0.0141 desktop and
0.0016 mobile viewport.

Desktop is 1440×900 and mobile viewport is 390×844, with DPR 1. Asset sizes sum
encoded resource bodies, excluding HTML and HTTP headers. Search samples dispatch
the same 18 query updates and wait for an animation frame followed by a task;
these are lab interaction timings, not field INP. LCP is also a lab measurement.
The changed shuffle arrangement can change which artwork appears first. These
runs are not a physical low-end phone benchmark or a Lighthouse score.

Raw measurements, screenshots and the measurement harness are retained locally
under `build/arcade-perf/`: `live-before.json`, `live-after.json`,
`comparison.json`, `measure.mjs` and `live-visual.json`.

## Changes

- Index searchable names, tag labels and school years once. Count facets in three
  passes, rather than rebuilding search text for every button and every key.
- Preserve result order while typing. Remove the grid reshuffle, movement
  animations and unnecessary geometry reads from each search update. Shuffle
  remains an explicit action; responsive layout and keyboard order are retained.
- Create screenshot tracks, captions, repository links and progress bars when a
  preview opens, instead of constructing them for every game on startup.
- Generate 128/256-pixel icons, plus 512-pixel artwork for featured candidates.
  Use responsive sources and actual tile sizes; startup uses the small thumbnail.
  Retain near-viewport loading, screenshot loading on interaction and compressed
  game payloads on launch. Browsing fetched no game modules, game data, gallery
  screenshots or provenance catalog in either version.
- Load a Latin/punctuation WOFF2 subset of Bricolage. Preserve the original TTF,
  variable weights and font license.
- Keep the full delivery inventory and historical evidence in
  `collection-provenance.json`. The browsing catalog keeps the fields the UI uses,
  including school-year evidence tooltips.
- Fill available gameplay gutters with one to four continuous columns per side.
  Adjacent columns move in opposite directions using CSS transforms and a bounded
  48-game thumbnail deck. Pause on hover, keyboard focus or explicit control;
  stop drift for reduced motion and hidden pages. Narrow gutters preview inside
  their tiles. Occupied game/control regions remain clear.

## Verification

All 306 Wasm modules and 115 game-data files have the same decoded hashes as the
previous verified deployment. All 618 responsive icon descriptors match their
actual image widths. Live shell, catalog, provenance, changed modules and font
match the publish package byte-for-byte. Desktop and mobile browser checks cover
search, filters, keyboard focus, hover previews, loading, Home, game launch,
column direction, pause, loop continuity and reduced motion.

The hosting check also exposed a pre-existing wildcard-encoding assertion that
cannot be enforced after ingress normalization. A temporary diagnostic observed
`gzip;q=0, *;q=1` arriving with `request.cf.clientAcceptEncoding` equal to `gzip`.
Cloudflare documents that it can replace `Accept-Encoding` and expose the
client encoding through [incoming request properties](https://developers.cloudflare.com/workers/runtime-apis/request/#incomingrequestcfproperties).
The direct Worker test checks exact quality handling and payload bytes. The live
check still enforces normal gzip, explicit identity, content type, decoded
hashes, HEAD, revalidation and range handling. For the normalized wildcard case,
it records the delivered encoding and independently validates decoded bytes.
The production Worker implementation is unchanged, and the temporary diagnostic
was removed.
