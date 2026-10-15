# Arcade expansion review, 15 October 2026

The expansion starts from the 53-game collection and its recorded representative
browser touch gameplay. New imports require a distinct source fingerprint, pinned
provenance, source-derived artwork and actual touch gameplay before admission.
The collection now contains **65 distinct games**, including 12 new imports.
The scout reviewed 37 repositories, admitted 12 and rejected 25, including a
duplicate with identical source and projects without usable gameplay. Detailed
reasons are in
[`arcade-import-decisions.json`](arcade-import-decisions.json).

## Existing game repairs

- **Minhoca, issue 11:** two consumers declared the byte mouse-packet counter as
  `int`. Aligning both with the original `uint8_t` definition restores Local menu
  selection and gameplay pointer packets. Browser and ARM64 Android touch trials
  entered the local arena and steered both snakes.
- **PettyCats, issue 12:** exclusive right/bottom pixel bounds prevent row aliasing
  and buffer overruns. A guarded framebuffer regression protects the final valid
  pixel and all out-of-bounds cases. Browser trials used 32 directional holds.
  Android trials accumulated 35.2 seconds of held movement plus Interact/Pet
  controls without a native crash. A completed petting cycle is not claimed.

Both regressions failed before repair and pass through CTest as
`community_source_regressions`. Original/patched hashes and reversible source
patches remain beside the upstream imports. Detailed sessions are under
`build/arcade-expansion/existing/`.

## Import and distribution checks

Nested upstream ignore rules initially hid 136 required source, Makefile and
service files from Git. Those files are now explicitly tracked. Disposable OS
and VCS metadata is recorded as omitted metadata instead of becoming a required
source dependency. The final repository gate uses
`scripts/check_collection_sources.py --require-tracked`.

The legacy SDK now exports Minix's original integer constants `TRUE`, `FALSE`
and `IRQ_ENABLE`, verified against Minix's headers. The compatibility client
regression failed to compile before the fix and passes for both runtime owners.

Native export regressions also repaired split compiler definitions that CMake
previously deduplicated incorrectly, and a referenced preview image missing from
the relocated Android export. Both failed before their fixes and pass afterward.

VascoGo/ninja-labyrinth was rejected as an exact source duplicate of the existing
Labyrinth Quest import after its enum typo correction. Different repository names
do not count as different games.

## New games and observed touch gameplay

| Game | Demonstrated checkpoint |
| --- | --- |
| Minix Tower Defense | Started a wave, built an archer tower, observed enemies and projectiles. |
| Tennis for Two | Opened a match, served the ball and moved the player. |
| Brick Blitz | Launched the ball and moved the paddle using held controls. |
| Euler's Way | Completed the triangular mission and advanced to the next graph. |
| Castle Clash | Charged and released the cannon, paused and returned to the menu. |
| Minix Surfers: Versus | Moved player one, spawned a train as player two, reached score 56. |
| BreakPong | Moved the paddle, damaged bricks and reached score 1 in the browser and 5 on Android. |
| Bounce Ball | Used ordinary Enter to start and launch, destroyed a brick, reached score 5. |
| Portal 2D | Walked, jumped, placed both portal colors, returned to the menu and restarted twice. |
| Block Destroyer | Dragged the paddle, removed a brick and reached score 005. |
| Leiria | Moved the character, observed advancing monsters, reached game-over and exited. |
| Alien Express | Walked to a package and placed it in the first inventory slot. |

All 12 additions passed representative browser and native Android touch sessions.
Native sessions first used APK `f068c304...`; the final APK `46447b5a...` then
retested both changed games, Portal 2D and BreakPong.

Every new game has a source-derived icon, raw canvas preview, concise description
and its own mobile profile. Controls use the original game's actual keys, useful
pad axes, held/repeated inputs and cursor behavior. Direct touch is used only for
verified cursor contracts; reset or accelerated cursors retain relative input.

BreakPong uses relative hover and a horizontal mouse stick because its paddle
caps each mouse delta instead of following absolute cursor coordinates. Native
taps and continuous swipes established that contract before changing the profile.

New source corrections retain original hashes and reversible patches: Bounce
Ball's ordinary Enter and coalesced-input repaint, Portal 2D's level filename
buffer and sprite cleanup ownership, Minix Surfers' LCF callback signature and
Alien Express' local RTC helper name. The first two additionally have permanent
public CLI gameplay regressions inside `community_source_regressions`, which contains four checks. The Portal
restart regression failed with SIGABRT against the previous cleanup code, with
the filename fix retained, and passes after the ownership repair. A separate
guarded allocator probe checks frame replacement, partial construction and
flipped sprite destruction.

The Android library also has title search with empty-result recovery. Actual
emulator checks caught and repaired the initial container-focus highlight issue;
search, clearing, native launch/return, saved files and crash containment passed
in a separate three-game export before the full collection build.

## Final published artifacts

The web library is live at [arcade.rgo.pt](https://arcade.rgo.pt), Worker version
`e9522664-215f-4b67-a5cd-c207b70845fd`, built from code revision `66c77ff`.
Production checks use normal DNS and TLS:

- 65 touch launch/render/control/Home-return flows, with no browser errors or
  failed requests.
- 130 decoded icons/previews, four responsive widths and four keyboard/loading
  recovery flows.
- All 65 decoded game modules and 526 shell/player/configuration/artwork files
  match the final export exactly.

The final APK is `dist/lcom-arcade-android/collection.apk`, 353,556,768 bytes,
SHA-256 `46447b5a60535304029cb35e3380c600d5daa1fd708dd4dec6183f52ebde2976`.
The downloadable build is attached to the
[Arcade APK release](https://github.com/rodrgds/lcom-ng/releases/tag/arcade-2026-10-15).
Its 65 game libraries and three shared native libraries are ARM64; signature,
16 KB ZIP/ELF alignment, 65 control profiles and 5,795 selected source hashes
are verified. The exact final APK passed all 65 fresh native process launches,
render/return checks, title search, saved-file persistence, forced termination
and launcher crash containment. All native screenshots were visually reviewed.

The source audit verifies 7,757 pinned files, with 20 documented corrections
across 31 source files. The final `lcom-ng-verify` gate passes 38 CTests,
26 browser tests, 11 configuration tests, three collection tests, installed SDK
relocation, 99 gallery states, documentation and Remotion checks.

Detailed local evidence is under `build/arcade-expansion/`: `web-final/live/`,
`web-final/targeted/`, `native-new/evidence/`, `native-existing-final/`,
`native-all65/`, `final-fixed-verify.log`, `final-apk-inventory.json`,
`live-hosting-65.log` and `live-assets-65.json`. The tracked catalog retains
per-game checkpoints, original source provenance and the previous 53-game review.

## Coverage limits

Gameplay sessions are representative checks, not completed campaigns or proof of
every mode, level, power-up or scoring edge. Browser touch emulation, Android
emulator input and physical-device performance are separate evidence. Physical
phones have not been measured. This expansion does not verify UART peer
multiplayer or FEUP Hero's external music helper.
