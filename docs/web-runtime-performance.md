# Browser runtime performance, 18 October 2026

The runtime optimization and Arcade interaction fixes are deployed at
https://arcade.rgo.pt from `42e364e8ad2afc4abaace4ba453df30e69432ce6`.
Cloudflare serves version `b983ffad-b9cb-41ff-9a1c-4cb4fa2fdcf2` at 100% traffic.

## What the profiles showed

Chrome CPU profiles sampled real exported games inside the Arcade iframe:
Plants vs. Zombies, Ninjix, Sushi Slash, MINIX Grand Prix and Teclas. Authored
gallery inputs entered their playable surfaces. Additional samples held the
accelerator in Grand Prix and supplied typing input in Teclas. Captures were
inspected to distinguish gameplay from menus and static screens.

Symbolized five-second profiles attributed 33% of active samples in PvZ and
16% in Ninjix to browser-host presentation, including its conversion and upload
children. Those percentages exclude idle/program samples. The games' original
sprite/world drawing remains substantial. Static Teclas frames already avoid
repeat uploads, so a low upload count is not a low-FPS finding.

The expensive host path decoded framebuffer rows into RGB, copied those rows
again into RGBA, then copied RGBA into a separate JavaScript ImageData buffer.

## Changes

- VBE can decode a row directly to opaque RGBA. The browser host removes its
  intermediate RGB row and second per-pixel pass.
- Indexed colors cache four bytes, retaining exact six/eight-bit DAC scaling,
  individual component writes and the live pixel mask.
- RGB555/RGB565 use small constant lookup tables for the existing integer
  expansion rule. All 65,536 input words in each format are checked against
  the original floor-scaled values, for both RGB and RGBA output.
- Canvas ImageData references the presented Wasm storage. Mode changes, allocation
  movement and memory growth replace its cached view before upload. At
  1280×1024 this removes one 5MiB copy per presentation, or 300MiB/s at 60
  presentations per second.

Student game sources, original asset payloads, IRQ delivery, pointer behavior,
virtual clocks and game update rates are preserved by these runtime changes.
The separately requested Escape shortcut changes and Arcade UI fixes are
recorded in [Arcade refinements](arcade-refinements.md).

## Conversion benchmark

Final measurements use Chrome 155.0.8059.80 on macOS arm64, Apple M4, with
three fresh browser runs per version in alternating order. The table reports
the median of each run’s batch median. The maintained benchmark runs the real
browser host in WebAssembly. Each mode
warms up for 40 presentations, then measures five batches of 60 presentations.
Construction and framebuffer initialization are outside the measured interval.
The bridge callback excludes Canvas to isolate conversion. These are local
component timings, not whole-game speedups or FPS guarantees.

| Framebuffer | Before | After | Speedup |
| --- | ---: | ---: | ---: |
| 1280×1024 BGRX32 | 2.063 ms | 1.073 ms | 1.92× |
| 1152×864 BGRX32 | 1.557 ms | 0.813 ms | 1.91× |
| 800×600 BGR24 | 0.747 ms | 0.382 ms | 1.96× |
| 1024×768 BGR24 | 1.205 ms | 0.617 ms | 1.95× |
| 640×480 RGB555 | 0.840 ms | 0.305 ms | 2.75× |
| 640×480 RGB565 | 0.838 ms | 0.303 ms | 2.76× |
| 1024×768 RGB565 | 2.120 ms | 0.778 ms | 2.72× |
| 1280×1024 RGB565 | 3.458 ms | 1.297 ms | 2.67× |
| 1024×768 indexed | 1.245 ms | 0.448 ms | 2.78× |

All nine complete RGBA framebuffer hashes match before/after, including a changed
DAC entry and pixel mask. An earlier separate Canvas-inclusive probe measured 1.9–3.0×
faster conversion plus upload, with matching canvas pixels. Performance samples
come from a shared development machine; host load affects absolute timings.

At 4x Chrome CPU slowdown, high-resolution RGB565 conversion dropped from
12.98 to 4.82 ms per frame. This models CPU pressure, not a particular phone.

Real-game five-second samples also measured the JavaScript presentation callback
at the same framebuffer dimensions, including Canvas upload:

| Game | Before mean | After mean |
| --- | ---: | ---: |
| Plants vs. Zombies, 800×600 | 0.150 ms | 0.041 ms |
| Ninjix, 800×600 | 0.150 ms | 0.048 ms |
| MINIX Grand Prix, while accelerating | 0.134 ms | 0.038 ms |

These sampled callback costs are separate from C/C++ game drawing. They are not
whole-game speedup claims. No page errors occurred in those samples.

Reproduce inside Devenv:

```sh
python3 scripts/benchmark_web_frames.py --output build/web-frames/current
python3 scripts/benchmark_web_frames.py --source-root /path/to/baseline \
  --output build/web-frames/baseline
python3 scripts/benchmark_web_frames.py --cpu-rate 4 \
  --output build/web-frames/constrained
```

`--max-ms 1.5` is an optional local budget. It failed on the baseline's larger
RGB565 modes and passed after optimization. It is not enabled in shared CI.
Each output includes raw samples, framebuffer SHA-256 hashes and environment
metadata. Detailed profiles, captures and the original before/after probes are
under ignored `build/web-runtime-perf/`.

## Packaging storage

Publishing now writes compressed game payloads directly, avoiding a preliminary
copy of 1,926,488,647 decoded bytes for this catalog. The existing large-asset
regression also constrains the publishing process to 25MiB per file on POSIX:
the old implementation fails while copying decoded data, and the new one passes
with exact decoded bytes. Runtime archives remain separate from these packaging
changes.

## Release verification

- All 64 native/runtime tests passed.
- All 110 browser cases passed across split runs, including exact pixels for
  nine formats and heap growth in both Chromium and Firefox. Four startup
  assertions were updated for the restored Search button. An initial Firefox
  launch inherited Chrome's channel; the frame suite now launches each engine
  independently.
- Configuration, packaging, Worker delivery, source audits, docs, 99 gallery
  states and the Remotion typecheck passed.
- All 306 games were rebuilt. Every one of the 115 game-data payloads matches
  the previous deployment's decoded SHA-256. The source audit verified 33,173
  files across 305 pinned projects.
- Local disk capacity interrupted parallel building and initial packaging.
  Verification resumed separately. The candidate's old NAS payload links were
  replaced with regular local files using the verified compressed baseline.
  Transparent filesystem compression reduced the ignored SDK payloads from
  1,926,488,647 logical bytes to 558,862,336 physical bytes, with every decoded
  hash rechecked. Published files still use the normal gzip delivery format.
  Redundant raw baseline modules were retired only after verifying the retained
  compressed baseline byte-for-byte.

The final package contains 306 modules and 115 data files. All 421 decoded
payloads match the qualified builds, all 625 responsive-image width descriptors
match their files, and Wrangler's production dry run passes for `lcom-arcade`.

Live checks verified the independent search dialog, preserved query after
returning from a game, 390px/320px filter layouts without horizontal overflow,
ten featured games and six scrolling suggestion columns without pause buttons.
Ninjix entered actual gameplay using its authored gallery input sequence through
the browser keyboard and mouse handlers. Shift+Escape released mouse capture
without leaving the game. The iframe delivered 182 animation callbacks over
three seconds, with no page errors. This is a smoke check, not a gameplay FPS
benchmark. Fifty-five live launcher, catalog, font and sampled game-player assets
match the qualified package's SHA-256 hashes.

The complete live delivery gate passed for all 306 modules and 115 data files:
429 payload responses match the qualified decoded SHA-256 hashes. Additional
HEAD, 304 revalidation and range-request checks passed for the largest module
and data file, over both gzip and identity delivery. Cloudflare's wildcard
encoding normalization is recorded separately from the exact Worker negotiation
tests. The Docs Site CI run for the release revision passed; browser and student
package CI were still running when these live checks finished. Android remains
paused.

Local raw profiles, release logs, live captures and hash inventories are retained
under `build/web-runtime-perf/`.
