# Arcade mosaic and Android launcher release

APK source: `a3697659922a759193688c973d028b5523c363e5`.
Web metadata source: `6bc0e7d5130b560268418de7152df7d899160f28`.
Web: <https://arcade.rgo.pt/>.
Worker version: `66fc0525-db53-4b93-8e4c-0d4ddd154313`.
Release: <https://github.com/rodrgds/lcom-ng/releases/tag/arcade-2026-10-16-mosaic>.

## Result

- Square icon mosaic, separate featured rows, shuffled order, embedded utility tiles and modal search.
- Wider hover previews with real horizontal slide transitions, repository creation year/size facts and independent Repo links for 65 public projects. Creation years are verified from GitHub's `createdAt` field; supplied PvZ has no public repository date.
- 196 real gameplay captures across 66 games, 4,924,502 bytes. Memory Quest and LabJack use two useful frames; other games use three. Inputs, crop bounds and provenance are in the checked-in gallery manifests.
- Three generated launcher icons, including the final simple pixel Ninjix requested after review. Exact prompts and final hashes are in `collections/arcade/icons/generation.json`. Original in-game artwork is unchanged.
- Shared web host returns to the library on normal guest exit. Standalone exports keep restart behavior; detectable error exits retain the player. Android's existing SDL completion returns to its surviving native launcher.
- Larger adaptive Android artwork cards, persistent search, clear/random actions, compact landscape header, keyboard handling and restored browsing position.

## Verification

- Six collection UI scenarios passed, including actual two-frame push geometry, keyboard focus after scrolling, one-tap touch, reduced motion and Repo navigation without launching a game.
- The 39 shared player/export browser scenarios passed across the full run and isolated confirmation. A concurrent Playwright output-directory collision required an isolated rerun; no input-runtime change was needed.
- All 66 final web games passed first-render, mobile control geometry, fresh iframe launch and Home return checks. Their Wasm modules match the previous published build byte-for-byte.
- Six metadata/export tests, hosted image packaging, Android exporter relocation/native checks, docs build and `git diff --check` passed.
- Strict source audit passed: 66 games, 7,757 pinned files and the existing 36 recorded source corrections across 80 files.
- Native emulator checks covered phone/tablet portrait and landscape, 1.8 font scale, search/clear/empty state, sticky header and keyboard dismissal.
- The final APK passed six fresh launches across PvZ, Grand Prix, Memory Quest, LCOMaze and Ninjix, including saves, Home/Back, forced termination recovery and an update installation. All 69 native libraries match the previous verified APK. The actual original Ninjix Exit menu returned to the same launcher process.
- A fresh design review accepted the final simple Ninjix icon and the reviewed web/native layouts.

## Published bytes

The Android ARM64 APK is 355,376,812 bytes:

```text
92ec6d128be1b9c20e1b194a06e7ed8ce872331cdb94ea612f279df7fd5292e3
```

Its public download was streamed and independently hashed. The installed APK and final Ninjix icon also match their packaging receipts.

The live web shell, modules and catalog match the packaged build. All 328 hosted icons, previews and gallery frames match their local bytes. All 66 Wasm modules passed live hosting checks, including 68 gzip/identity responses, plus HEAD and cache revalidation.

The live Ninjix Exit button was clicked in the desktop browser. It removed the game iframe and restored the library's search context. Physical Android-device performance is outside the emulator evidence above.

The final creation-year correction fetched all 65 public repositories through GitHub's repository `createdAt` API field. Metadata/export regression and docs checks passed, and the republished live catalog matches those creation timestamps. This web-only metadata correction leaves the published APK unchanged.
