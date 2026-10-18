# Arcade loading and progressive previews, 18 October 2026

Issue: [#22](https://github.com/rodrgds/lcom-ng/issues/22).

## Production follow-up, issue 23

The controlled comparison below did not establish that production loading was
fixed. A subsequent user capture showed 9.47 seconds to DOMContentLoaded,
23.22 seconds for the catalog, 34.88 seconds for the font, and a 36.25-second
load event. A fresh live browser reproduced a 9.84-second load. Requests were
Cloudflare cache hits, but their delivery times varied substantially.

The decorative font was preloaded alongside the catalog and held the load event
open. The launcher now requests it at low priority after rendering the catalog
and after the page load. System text remains usable while it downloads. The
hosted packager also pins the unused width axis to its normal value, reducing
the font from 169,060 to 100,236 bytes. All characters, default glyph widths,
weight variation and optical sizing are preserved. The original font stays
unchanged in the SDK source.

Three fresh Chrome contexts per version, 1280 by 800, fixed shuffle seed,
400 ms request latency, 64 KiB/s download, 4x CPU slowdown and local HTTP/2.
Medians compare `546368f5` with this follow-up:

| Measure | Before | After |
| --- | ---: | ---: |
| Grid rendered and interactive | 5,491 ms | 3,556 ms |
| All visible game icons decoded | 9,458 ms | 7,537 ms |
| Last initial network response, including background work | 10,987 ms | 9,793 ms |

These are 35%, 20% and 11% improvements respectively. The browser load event
also moved from 8,842 to 1,705 ms, but that event precedes catalog rendering
after this change and must not be presented as the usable-page improvement.
This does not prove that the variable live delivery delay is resolved.

The blocked-font browser regression verifies that the font does not compete
with the catalog, browsing works during its download, the page load completes,
and the font eventually applies. The packaging test preserves its character
map, default glyph metrics and remaining variation ranges. Desktop and phone
renders retain the same typography; the full phone capture is pixel-identical.
All 5,563 per-game files match the previous publish byte for byte.
Raw measurements: `build/arcade-loading/font-benchmark.json`.

The previous deployment (`96d9fab0`) served every static file with
`max-age=0, must-revalidate`. Live requests reproduced 0.5–0.6 second waits even
for cached files. The launcher then discovered nine imported modules, another
nested audio module, and finally the catalog in successive network rounds.

## Changes

- Bundle the hosted launcher into one content-hashed script. The SDK's ordinary
  unbundled exports remain relocatable and do not acquire a Bun requirement.
  The repository's hosted Arcade packager uses Bun from Devenv.
- Preload the browsing catalog alongside the script. Its URL includes its content
  hash; the runtime reuses that request instead of fetching after module loading.
- Store hosted browsing images, fonts, styles, scripts and catalog under `static/`
  with content hashes and `public, max-age=31536000, immutable`. HTML retains the
  host's revalidation policy, so new releases select new URLs safely. Game payload
  headers and the download Worker are unchanged.
  This migration also retains the previous browsing image URLs for already-open
  tabs; those compatibility copies retain normal revalidation.
- Embed a first-screenshot placeholder of at most 32 by 24 pixels. Across all
  306 games these total 45,872 encoded image bytes; the median is 138 bytes.
  A first hover can display a real screenshot without any image request finishing.
- Warm 160-pixel first screenshots for a limited set of visible cards, after their
  launcher icons and the page load. The pending queue holds at most eight, with
  two low-priority downloads at a time. Coarse pointers, Data Saver and 2G skip
  background warming. Keep at most 48 decoded thumbnails, roughly 3.7 MB of RGBA
  pixels at the maximum dimensions, rather than retaining full galleries.
- Load sharp screenshots on interaction and warm only the next frame of the open
  gallery. Closing still releases gallery DOM, animation objects and full slide
  references after the existing transition. Old decodes cannot revive a closed
  preview. Prioritize visible featured icons above offscreen/decorative icons.

The browsing catalog is 469,207 bytes before compression, versus 451,346 bytes
previously. Shorter hashed image paths offset most of the embedded placeholders.
The final catalog compresses to about 127 KB with gzip. This consciously trades
some first-load bytes for immediate screenshot feedback; repeat visits cache it.

## Controlled comparison

Three fresh Chrome contexts per version, 1440 by 900, fixed shuffle seed,
400 ms emulated request latency, 10 Mbit/s download and 4x CPU slowdown. Local
HTTP/2 hosting serves the same 306 games with the corresponding cache headers.
Values are medians. Grid readiness is measured after DOMContentLoaded, without
waiting for every optional background thumbnail. First hover follows network idle.

| Measure | Before | After |
| --- | ---: | ---: |
| Grid usable | 2,494 ms | 1,347 ms |
| First preview image visible | 565 ms | 40 ms |
| Sharp first screenshot visible | 578 ms | 471 ms |
| Largest contentful paint, lab | 3,304 ms | 2,368 ms |
| Launcher JavaScript requests | 11 | 1 |

Grid readiness improved 46%, initial preview visibility 93%, and lab LCP 28%.
The initial preview can be tiny or warmed; sharp image quality still upgrades
as its download finishes. These are controlled desktop measurements, not field
Core Web Vitals or a guarantee for every connection. The local HTTP/2 certificate
is self-signed, so repeat-navigation cache timing is excluded from these claims;
production cache headers and reuse are verified separately on Cloudflare.

The packaged FEUP Hero/Ninjix gameplay smoke test passed. All 306 module hashes,
115 data hashes and 1,836 per-game shell/configuration files match the previously
qualified package. Game behavior, source assets, timing and input are unchanged.

The packaging/cache contracts, blocked-network first-hover regression and bounded
thumbnail cache are covered in `tests/arcade_packaging.py` and
`tests/collection_web.spec.mjs`. Focused browser checks cover search/filtering,
keyboard/touch, reduced motion, closing/reopening, real audio and exported games.
Raw measurements and intermediate comparisons are under ignored
`build/arcade-loading/`. The reported controlled sample uses `h2-before.json`
and `h2-final.json`; earlier HTTP/1.1 runs include different load-event wait
semantics and are not mixed into this table.
