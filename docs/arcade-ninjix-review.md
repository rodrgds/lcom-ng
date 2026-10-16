# Arcade Ninjix and image follow-up

The Arcade now contains 66 games: 65 pinned community imports and the repository's
Ninjix showcase. Ninjix keeps its original game art and logic. Its new icon uses
the original ninja sprite and logo; its preview is the existing battle capture.

Ninjix uses relative touch movement, tap selection and a separate control panel.
Named tower shortcuts, Start, Pause and Cancel leave the board clear. Native
`toolbar: auto` keeps the pointer switch inside that reserved panel too.

The website displays each game's icon on its card and loading screen. The smallest
cards have enough height for a long title and icon. Interrupted player-module
downloads now offer Retry in the collection and Reload in standalone exports.
This fixes a real loading-script interruption found during live verification.

Hosted image copies preserve the source PNGs and game assets:

| Images | Original bytes | Published bytes | Reduction |
| --- | ---: | ---: | ---: |
| 66 previews | 8,550,249 | 1,377,808 | 83.89% |
| 66 icons | 2,360,297 | 111,958 | 95.26% |
| Total | 10,910,546 | 1,489,766 | 86.35% |

The packager uses WebP previews up to 640 × 480 and icons up to 128 × 128, retaining an
original whenever it is smaller. Content-based filenames refresh browser caches.
Game Wasm modules remain unchanged by image packaging and the recovery fix.

Verification completed:

- 28 browser tests, including long phone titles, correct card/loading icons,
  blocked-module recovery in both export modes, cancellation and retry.
- 66 live launches, nonblank first frames and returns, plus 132 decoded images,
  four responsive layouts and search/loading/recovery flows, with no final failures.
- 534 active hosted files match the final publication package byte for byte.
  All 66 hosted Wasm module hashes, gzip/identity delivery and cache checks passed;
  their bytes were preserved through the subsequent player-shell recovery update.
- A forced loading-script failure on the final live Ninjix player offered Retry,
  then rendered the game and returned to the library successfully.
- Android emulator touch gameplay placed Normal and Smoke defenders, spent
  coins 230→140→15, ran a real enemy wave and paused/resumed the same battlefield.
  Native search, relaunch, Home, isolated-process exit and saved-file checks passed.
- The final APK matches 66 profiles and 5,960 source hashes. All 69 ARM64 libraries,
  signing and 16 KB alignment passed. An actual previous 65-game APK update preserved
  private game data and retained the same package and signing certificate.
- Mixed-source and image-packaging regressions passed after reproducing the failures.
  Strict provenance checked 7,757 pinned files and 20 documented source corrections
  across 31 files. Docs checks and all 99 gallery states passed.

The [browser CI run](https://github.com/rodrgds/lcom-ng/actions/runs/38865168454)
passed for the published web code `7a236c7`, including the source and image
packaging checks.

The short sessions establish playable objectives and wrapper behavior. They do
not claim complete campaigns, UART multiplayer or physical-phone performance.
The other 65 games retain their prior native gameplay evidence in the
[expansion review](arcade-expansion-review.md).

Published web: [arcade.rgo.pt](https://arcade.rgo.pt), code `7a236c7`, Worker version
`5fc2f2a4-8a4b-4d86-b7a8-868b89899a0d`.

Published APK: [Ninjix Arcade release](https://github.com/rodrgds/lcom-ng/releases/tag/arcade-2026-10-16-ninjix),
354,709,528 bytes, ARM64, native source `c8c2252417f7898b3e69d2f59ad5b1033032d8b2`.
SHA256: `69f49f4e132516f9d7bccc522ef7e3a09d01e51e58cf9ae5a40b184095d8e9df`.
GitHub's uploaded-asset digest matches the locally verified APK, and its public
APK URL serves the expected file length. Earlier Arcade releases are unchanged;
the stable SDK release remains v0.2.0.

Detailed local evidence is under `build/arcade-ninjix/` and
`build/arcade-expansion/ninjix/`, including the final live launch ledger and
reviewed native gameplay captures.
