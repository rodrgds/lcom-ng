# Arcade roulette, sounds and keyboard review

Verified on 2026-10-16. Web source revision:
`486bc2c791230f9c7e0f9a44d83b448ab60f0bd4`.

## Delivered behavior

- **Surprise me** accelerates through actual game icons, then slows to a uniformly
  selected catalogue game. Play launches it; Spin again chooses another result.
  Skip, Escape, backdrop dismissal and closing stop the animation safely.
- Reduced motion reveals the result immediately. Resizing preserves the selected
  icon's alignment with the marker. Portrait and landscape keep actions visible.
- Cuelume 0.2.8 is bundled locally with its MIT notice and reproducible provenance.
  UI sounds default off, use volume 0.35, and remember the sound preference.
  Muting stops ringing voices and synchronizes both controls and other tabs.
  Reloading never autoplays. No audio downloads or remote dependencies are needed.
- Search has no heading or close button. Ctrl+K, Command+K and `/` open it.
  Escape or the backdrop dismiss it, preserving the query and restoring focus.
  Shortcuts do not interfere with guest games, typing `/`, or another modal.
- All four arrows navigate the rendered main mosaic and search grid. Featured
  cards use their resting geometry; secondary Repo links keep normal Tab access.
- Featured placement favors unused columns when utility tiles displace targets.
  Shuffle retains varied interior positions rather than repeating two columns.

## Verification

The focused browser suite passed all 13 tests against the isolated release
snapshot. It covers real animation geometry and timing, selection and launch,
return focus, cancellation, actual Web Audio output and immediate mute,
preference persistence, unavailable audio, reduced motion, touch resizing,
spatial arrows, shortcut ownership, previews and repeated shuffles.

```sh
devenv shell -- bunx playwright test tests/collection_web.spec.mjs tests/collection_navigation.spec.mjs
devenv shell -- bunx playwright test tests/web_export.spec.mjs -g 'collection|normal guest exits'
devenv shell -- python3 scripts/check_collection_sources.py --require-tracked
devenv shell -- bun run docs:build
```

The five export checks passed, including the installed SDK payload, guest exit,
loading cancellation/retry, phone icons and failed module recovery. The source
inventory verified 66 games, 7,757 pinned files and 37 recorded corrections across
83 files. The documentation build and owned-diff whitespace checks passed.

A landscape regression first reproduced actions below the dialog's visible
bounds, then passed after the compact layout. A release run also reproduced a
shuffle with only two featured columns; preferring unused columns fixed it.
The final suite checks five further shuffles. The earlier roulette and keyboard
regressions failed against the unchanged launcher before their implementations.

Concurrent Arcade expansion and runtime edits were preserved. The release used
an immutable base SDK plus only this change's files. Two Devenv startup calls
stalled during shared Nix contention; packaging and final verification used the
same resolved tools from `.devenv/profile/bin`.

## Publication

- Published to <https://arcade.rgo.pt/> with Worker version
  `30c394fa-93bb-4e3f-9d36-c333331cf5d8`.
- All 15 checked public launcher, catalogue, module and attribution files match
  the release package byte for byte.
- Live Chrome checks passed compact search, both shortcuts, spatial search arrows,
  query retention, roulette selection, synchronized mute, a real game's first
  frame and return focus. The selected game was Star Defender.
- Live touch checks passed portrait, landscape, reduced motion, visible actions
  and search. No page errors occurred.
- Manual in-app browser review confirmed the actual icons and compact search.
  Right, Down, Left and Up moved between the visible main-grid utility cells.

These changes affect the browser collection launcher. Game binaries and the
previously delivered Android APK retain their existing verification scope.

## Compact controls follow-up

Web source `0530efd9065da2f0b39a97cecd90d0bfb5a6ea8f` supersedes the
three-tile controls and optional sound setting described above:

- One framed square contains the compact brand header and Search, Screen,
  Shuffle and Surprise buttons. The four actions occupy a two-by-two grid.
- Sound is enabled after interaction, with no in-page sound switch or stored
  preference. Browser tab muting controls audio. Page load remains silent.
- Mobile sizing leaves room for three rows of 44px targets. Labels remain on
  one line. Tab order follows the visual order; spatial arrows remain active.

All 14 focused release browser checks passed, plus the updated label and target
checks at widths 320, 390, 584, 768 and 1440. The new layout and sound checks first
failed against the previous launcher. A phone label-wrap regression also failed
before the compact label treatment. Actual Web Audio output was measured after
keyboard and pointer interactions; reload creates no audio context.

The real 66-game catalogue passed responsive checks at 320, 390, 768 and 1440.
The documentation build, design scan and owned-diff checks passed. The pinned
66-game source inventory verified 7,757 files and 37 recorded corrections.
Concurrent imports, grouping and runtime work were preserved; the deployment
uses the previous published catalogue and only this change's launcher files.

Published Worker version: `bb1d766b-76e4-4db4-bd8e-b75c5b39566b`.
All 15 checked public assets match the release package. Live desktop and touch
checks passed the compact search, both shortcuts, spatial search navigation,
roulette result, sound after interaction, a real Portal 2D first frame and return
focus. Phone portrait, landscape and reduced motion passed, with no page errors.
The in-app browser also confirmed the compact control tile visually.

## Separate tiles and phone density follow-up

Web source `b63a9cbb` replaces the combined control tile with a separate logo
tile and a four-button tile. Phones keep these in two columns above a distinct
three-column game grid. Featured games retain their larger spans; desktop
continues packing the two utility tiles into the full mosaic. Sounds retain the
interaction-only behavior from the preceding update.

The updated layout regression first failed against the combined tile. The denser
grid then reproduced five pixels of horizontal overflow at 320px from hidden
preview metadata. Containing inactive overlays fixed it while preserving opened
previews and their Repo links. All 14 release browser checks passed. Responsive
checks with the real catalogue passed at 320, 390, 768 and 1440; the layout test
also covers 584px, 44px controls, unclipped labels, Tab order and spatial arrows.
The docs build, design scan, whitespace checks and pinned 66-game source audit
passed. Concurrent expansion and runtime work were preserved.

Published Worker version: `6c14a8e5-4507-43c7-a1fc-974765103c88`.
All 15 checked public assets match the release package. Live phone assertions
confirmed two separate utility columns and three game columns. Desktop search,
shortcuts, roulette, sound after interaction, a Not Cuphead first frame and
return focus passed. Touch portrait, landscape and reduced motion also passed,
with no page errors. Proof screenshots are under `build/arcade-two-tiles/live/`.

## Search motion, desktop density and clean roulette follow-up

Web sources `7700130c` and `ed4b2594` add:

- Search enters with a 320ms lift and scale, with a fading backdrop and a faster
  180ms exit. Native dialog transitions reverse on reopening without delaying
  typing, focus or launch. Reduced motion keeps opening and closing immediate.
- Desktop tiles use a denser grid, including nine columns at 1440px rather than
  eight. Regular tiles measure about 147px there. The phone layout is unchanged.
- Surprise me shows only the reel while choosing. It accelerates, slows over
  8.8 seconds, then pauses briefly before opening the result. Every crossed card
  triggers a quiet mechanical Cuelume tick. The marker lands 8–92% into the
  selected card, preserving that point through resizing and expansion.
- The result grows into a screenshot with icon, title, description, available
  year and download size, public Repo link and Play. Middle gallery frames favor
  gameplay over an early quiet frame. Outside click and Escape dismiss it.
  Heading, close, skip and spin-again controls have been removed from the reel.

The motion and density regressions first failed against the preceding launcher.
The new roulette checks first reproduced its extra controls and centered result.
All 17 focused release browser checks passed. The final roulette and rotation
checks were rerun after correcting an immediate reduced-motion re-open geometry
race. Frame samples verify acceleration, slowdown, intermediate reveal sizes,
noncentral landings and audible attacks for the passing cards. Cancellation
leaves no later ticks or game launch. Both edges, touch, resize, unavailable audio
and reduced motion are covered.

The real 66-game catalogue passed responsive checks, plus desktop, phone portrait
and landscape result reviews. Play stays visible and reachable; images load and
there is no horizontal overflow. The docs build, owned whitespace checks and
pinned 7,757-file source inventory passed. Concurrent imports, grouping and
runtime changes remain separate from this release.

Published Worker version: `3b54e260-56c0-4f82-8042-1cef9afed902`.
All 15 checked public assets match the release package for `ed4b2594`. Live Chrome
confirmed search, the denser desktop grid, screenshot results and a Sushi Slash
first frame launched from the result, followed by return focus. Phone portrait
and landscape passed with reduced motion and no page errors. The in-app browser
also opened search with Command+K and completed a real random spin to Euler's
Way. Proof is under `build/arcade-search-motion/live/`.

This update changes the web launcher. The previously delivered native Android
APK and the 66 pinned game binaries are unchanged.

## Random highlights, clover and viewport loading

Web sources `6244e981` and `621ca705` preserve the four fixed highlights and add
two distinct ordinary games to each visit or Shuffle. Resizing and returning
from play retain the arrangement. Extra selections update only the current
mosaic, leaving authored featured metadata unchanged. Previous extra picks
return to ordinary sizes on the next shuffle.

Surprise now uses the Lucide clover, with its ISC notice bundled locally.
Desktop spacing fits nine normal columns at 1280px. Smaller brand padding and
28px display type keep the complete Arcade name visible; phones retain their
two utility columns above three game columns.

Icons receive sources within 160px of the viewport, using the final shuffled
positions. Scrolling, resizing and Shuffle load newly visible games. Decode
completion starts a short fade and scale reveal. Reduced motion reveals them
immediately. Missing art loads the existing fallback. Hover screenshots remain
on demand, and keyboard focus can request an icon before the observer runs.

All 21 focused browser checks passed across the release suite and corrected
layout check. The new regressions first failed for eight desktop columns,
excessive offscreen requests and absent loading transitions. A 120-game fixture
covers deferral, scrolling and Shuffle. Held image responses prove the real fade,
fixed tile dimensions, reduced motion and a missing-image fallback. The brand
regression inspects actual line fragments so font ink extending beyond a line
box cannot be mistaken for truncated text. It first reproduced the ellipsis,
then passed after the compact brand treatment. The docs build, owned whitespace
checks and pinned 7,757-file source inventory passed.

Cold Chrome samples used the same catalogue and random seed, with cache disabled:

| View | Initial icon requests | Encoded icon bytes | LCP sample |
| --- | --- | --- | --- |
| 1280 × 720 | 66 → 48 | 279,651 → 209,524 | 252 → 688ms |
| 390 × 844 | 45 → 15 | 196,208 → 76,429 | 224 → 636ms |

The reveal contributes to the later paint timing. These fast-network lab samples
establish reduced initial artwork requests and bytes, rather than a field speed
score. All visible icons loaded; phone CLS was zero and desktop CLS was 0.0046.
The before and public after measurements live under `build/arcade-loading-polish/`.

Published Worker version: `80c07f30-e905-4b1f-8b23-75a6f6792110`.
All 16 checked public launcher and attribution assets match `621ca705`'s package.
Live desktop and touch checks confirmed four staples plus two extra highlights,
stable resize selection, new random inputs on Shuffle, 66 unique playable tiles,
no overlapping cells or horizontal overflow, and no page errors. The in-app
browser confirmed nine desktop columns, the clover and 18 deferred icons.
Proof screenshots are under `build/arcade-loading-polish/live/` and `live-perf/`.
Concurrent expansion and grouping changes were preserved. Game binaries and the
previous native Android APK are unchanged by this web-only update.

## Game surroundings, 2026-10-16

Published browser source `48a5fcf925a2b23140a2fccfa8ca9fd2e5e2a18c` with Worker version
`3d796f2f-4be3-4664-af76-b2ea60d805ac` at <https://arcade.rgo.pt/>.

- Unused game margins use the library's `#101021` background. Two faint columns
  of game icons drift in opposite directions behind the embedded player.
- Columns require at least 112px of side space and disappear when touch controls
  occupy their area. They never capture pointer input or cover the canvas.
- Artwork borrows decoded library icons first. Direct game links need at most
  eight extra icons, fetched only after the first game frame and only where a
  column fits. Reduced motion makes them static; hidden tabs pause animation;
  returning to the library removes the decoration images.
- Only collection players expose their background. Standalone game exports keep
  their existing black background. Game binaries, assets and settings are unchanged.

Verification: the new margin regression first failed on the black background,
then passed with the repair. All 22 collection/navigation checks passed together;
its additional direct-link image-budget check passed separately, for 23 distinct
checks. The documentation build and the pinned-source audit passed: 66 games,
7,757 files, 37 documented source corrections across 83 files.

Real Memory Match exports were inspected locally and on the public site at
1440×810 desktop, 390×844 portrait and 844×390 touch landscape. Desktop had two
columns outside the canvas; touch layouts kept the controls clear. First-frame
loading, reduced motion and return cleanup passed without browser errors.
Native in-app browser inspection confirmed the published background and artwork.
All 215 checked public assets matched the package, including HTML, JavaScript
and CSS for each of the 66 players. This verifies the shared player deployment,
not another complete gameplay pass through the catalogue.

Evidence: `build/arcade-surroundings/` contains the frozen SDK, red/green browser
results, unchanged-game hashes, desktop/phone captures and live byte receipt.
Concurrent catalogue and runtime work was preserved and excluded from publication.
This browser-only release does not change the Android APK.
