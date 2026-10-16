# Arcade cursor and preview refinements

Game/APK source: `2d6862f7526474dd38f5b59b0becb649c51a36fc`.
Web source: `3690db050da40abbeadf0ca7acef9f25579d6723`.
Web: <https://arcade.rgo.pt/>.
Worker version: `f63d2d4f-5293-41e0-8ec6-5f637aa71dad`.
APK release: <https://github.com/rodrgds/lcom-ng/releases/tag/arcade-2026-10-16-reaction>.

## Changes

- Quick Reflex now calibrates the desktop pointer from its real `(40,40)` start, unit movement and sprite-safe `(784,573)` bounds. Mobile relative movement and separate player actions are retained.
- Its original reaction and guess modes now update cursor position while playing. Results preserve that position instead of warping it to `(40,40)`. The three source changes are independently reversible to the pinned upstream hashes in `SOURCE_PATCH.diff`.
- Long game names wrap within their available width, preventing hidden resting labels from widening the phone or desktop page.
- The collection Home icon is explicitly white on dark gray, including when a game uses a light control theme.
- Featured games use shuffled horizontal bands with jittered targets, including interior columns. Targets stay stable during responsive reflow until Shuffle is pressed. Distinct vertical start rows and square packing are retained.
- Every hover starts its first screenshot progress bar from empty, including revisits. Slides advance after 2.4 seconds, preserving the horizontal push transition and reduced-motion behavior.
- Focused Repo actions remain reachable when scrolling or crossing another tile with the mouse. Ordinary hover switching after Home restores the primary game link remains supported.
- Quick Reflex's three real gameplay gallery frames were recaptured against the corrected module. All 196 gallery frames retain embedded provenance.

## Verification

- Actual browser regression failed before calibration: zero cursor pixels at the physical pointer near the bottom. Calibration alone then exposed a second failure after a round. The complete fix passed bottom alignment and completed React/Guess rounds with green signals and aligned result-menu cursors.
- All 40 shared browser export/player scenarios passed. Collection UI checks cover Home contrast, theme messages, keyboard focus, actual bar reset/fill and slide timing, Repo navigation, hover switching and return to the search modal. Featured interior distribution passed ten randomized layouts after a failing edge-only baseline.
- All six collection UI scenarios passed with the final sources. The actual 66-game catalogue passed responsive geometry at 320, 390, 768 and 1440 pixels; desktop and phone renders were inspected. Hidden labels are excluded from visible-copy checks. The mechanical design scan returned no findings.
- Native headless and SDL scripted gameplay replays exited normally with status 0. The SDL result frame was visually checked separately from the earlier guarded capture.
- Six collection export tests and 12 project configuration tests passed. Source verification passed for 66 games, 7,757 pinned files and 37 recorded corrections across 83 files.
- Updated Android APK: actual touch reaction round, green signal, 433 ms result, retained cursor, Play Again and original Exit passed. Ninjix also launched and its original Exit returned to the surviving launcher. Upgrade installation retained private saved data and the signing certificate.
- Only Quick Reflex's native library changed. Other 68 libraries and all game assets are byte-identical to the preceding mosaic APK. All 5,960 selected source contents and the profile were checked against the game commit, with legacy include-case aliases recorded.

## Android artifact

`dist/lcom-arcade-android-20261016-reaction/collection.apk`, ARM64, 355,376,812 bytes.

SHA-256: `e6bd255c114b6e2956fcd5c350a3877c9e6796bfcf189b0906385fdb561b7565`.

The public APK was downloaded as a stream and hashed, matching the packaged and installed APK. Sixteen changed shell, game and gallery resources match their published bytes. The original mouse repro passes against the live site before and after a round; the in-app browser also verified bottom cursor alignment, white Home contrast and return to the library.

Local detailed evidence: `build/arcade-reaction/`, `build/arcade-reaction-android/` and `build/arcade-home-work/`.
Physical-device performance was not tested. The prior all-66 launch checks and broader mobile layout review remain documented in `arcade-mosaic-review.md`; this follow-up verifies the changed game and shared navigation.
