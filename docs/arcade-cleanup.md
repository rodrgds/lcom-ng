# Arcade preview cleanup, 18 October 2026

## Findings and fix

Closed card previews retained their screenshot elements, slide caches and CSS
progress animations with forwards fill. Visiting more distinct cards accumulated
resources on the library even though each game iframe was removed correctly.

The launcher now scopes progress animations to open previews and removes preview
markup and cached slide references after the existing 340 ms closing transition.
A decode finishing after cleanup cannot repopulate an obsolete slide cache.
Reopening a card rebuilds its preview; the browser still supplies HTTP image caching.

Game sources, WebAssembly, runtime timing, input and audio behavior are unchanged.

## Local comparison

Baseline: `a8b65c34`. The same 306-game publish package, fixed shuffle seed, Chrome,
1440 by 900 viewport, 40 distinct card visits, then all previews closed. Both
samples wait 3.5 seconds and collect garbage before Chrome DevTools counters.

| Resource after 40 visits | Before | After |
| --- | ---: | ---: |
| Retained progress animations | 40 | 0 |
| Attached screenshot elements | 40 | 0 |
| DOM nodes, including browser-internal nodes | 4,420 | 3,463 |
| DevTools embedder heap | 14,358,440 B | 8,154,480 B |

This sample has 22% fewer nodes and 43% less embedder heap. Embedder heap is the
browser-managed heap reported separately from JavaScript. It is not total tab
memory, GPU memory, or a measured frame-rate improvement. JavaScript heap usage
was approximately unchanged, 2.93 MB before and 2.92 MB after.

Eight repeated FEUP Hero/Ninjix launches and returns, including FEUP Hero song
selection and gameplay, left only the parent frame. After the first launch,
listener count remained at 2,570 rather than increasing on each return. Document
count stayed at three, including browser-owned documents. Scroll frame intervals
remained approximately 16.7 ms. This desktop sample does not rule out unrelated
slowdowns on other devices or browsers.

A real exported-game check opens audio on three successive launches, retains the
contexts for observation, and verifies each is closed after returning. This check
also passed on the baseline, so it verifies existing browser cleanup rather than
claiming an audio leak was repaired. One collection sound context persists for UI
sounds during the page lifetime.

The retention regression failed before the fix and passed afterwards. Additional
checks cover a screenshot finishing after dismissal, reopening, filter/search
navigation, touch, reduced motion, side rails, and real exported-game startup.
Raw profiles, screenshots and focused check logs are under ignored
`build/arcade-cleanup/`; the lasting regressions are in
`tests/collection_web.spec.mjs` and `tests/web_startup.spec.mjs`.

## Featured selection

Papa's Pizzeria was already eligible and remains in the pool. Minix Surfers:
Versus remains playable but is no longer featured. Ten games are selected from
these 28 candidates on each visit or Shuffle:

- Age of War: Ancient Front
- Bamboozlers
- Castle Clash
- Escape the Lab
- Euler's Way
- FEUP Hero
- Fireboy & Watergirl
- Getaway Car
- Gold Miner
- HumanoId
- Legend of LCOM
- MINIX Grand Prix
- Ninjix
- Papa's Pizzeria
- Penaltix
- Penguin Run
- PettyCats
- PixelMind
- Plants vs. Zombies
- Plunder
- Portal 2D
- Snook: Eight Ball Pool
- Super Cold
- Sushi Slash
- Teclas
- Tennis for Two
- Thin Ice
- Watt: Circuit Escape

Packaging validation compares all 306 module and 115 data hashes against the
previous qualified package, plus 1,530 per-game JavaScript, HTML and configuration
files. All match. The only game delivery change is removing the unused 512-pixel
featured icon variant for Minix Surfers; its original artwork remains intact.
