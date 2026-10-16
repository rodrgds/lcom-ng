# Arcade steering and control follow-up

The collection contains 66 games, including Ninjix. MINIX Grand Prix now has a
rotating steering wheel, separate accelerator and brake/reverse pedals, and
independent Drift and Boost buttons. Releasing the wheel centres it and releases
steering. Its digital guest steering remains unchanged. The S pedal slows through
zero and then reverses. Getaway Car keeps its lane controls and automatic throttle.

`lcom.json` supports `style: "wheel"`, degree-based `maxAngle` and `deadZone`,
pedal action icons, and `touchClick: false`. The latter starts the pointer switch
in Move only mode; named mouse actions and the switch remain available.

The four reviewed Move only defaults are Minixsweeper, Memory Match, Fruit Defense
and Knight Quest. Minixsweeper has separate Flag and Discover actions. Fruit
Defense's right action is labelled Build / cancel, matching its two functions.
Drawing, petting and drag-based boards retain their appropriate direct gestures.

Fixes verified at their observable input boundaries:

- Pac-Man's menus accepted arrows but gameplay accepted WASD. The mobile pad now
  sends WASD. The same rendered-game touch test failed before the correction
  (Left moved x398→422) and passed afterward (Left x398→382, Right x382→395).
- Grand Prix discarded releases while paused. All six driving releases now work
  through both ESC and mouse Resume. Original and patched source hashes and a
  reversible patch remain in the pinned import's provenance.
- Fast relative mouse moves lost travel beyond 96 pixels. The embedded pointer
  now retains a bounded motion backlog and emits complete PS/2-sized packets.
  A browser regression originally delivered 96 of 275 requested vertical pixels;
  it now delivers all 275, with button edges ordered after their travel.
- Ninjix's mouse startup ACK entered its packet decoder, and screen transitions
  reset its cursor. It now consumes command replies and preserves cursor position
  across screen initialization. Actual rendered menu and gameplay tests place
  the cursor at x100,y25, matching the desktop browser pointer. The gameplay test
  reproduced y105 before the transition correction.
- Configured mouse-action taps now honour the minimum `tapDuration`, measured
  from contact down. Cancellation, hidden controls and backgrounding release
  input, while independent fingers and physical buttons keep their ownership.
  Instant Memory Match Flip taps now reveal the targeted card in the real game.

Six previews now show clearer gameplay: Not Cuphead, LCOMaze, Snake Garden,
Teclas, Brain Training and Dr. Minix. LCOMaze uses a closer crop of its genuine
flashlight view. Snake Garden shows a grown snake and score eight. Original game
art remains unchanged. Published thumbnails total 1,517,894 bytes, compared with
11,120,730 source-image bytes. Original PNGs remain available in the repository.

Local evidence is retained under `build/arcade-wheel/`, `build/arcade-click/` and
`build/arcade-wheel/preview-polish/`. Short sessions establish the stated controls
and objectives, rather than completed campaigns or physical-phone performance.

The follow-up cursor audit covered the actual loaded artwork, transparency key,
packet handling, motion scale and framebuffer writes of all 66 games. Memory
Match now consumes complete packets, reaches its bottom row without losing the
cursor and repaints its last row correctly. Shared delivered-packet tracking
handles configured desktop mouse gain, fractional guest coordinates, re-entry
and mixed touch/mouse use without changing native touch gain.

Other confirmed corrections keep visible cursor artwork inside the frame while
preserving aiming, building and drawing targets where required. Star Defender's
out-of-bounds target rendering previously crashed its Android game process;
clipped rendering passes the same firing and long-drag sequence. Euler's Way now
consumes startup/reset command replies before decoding movement packets. Portal
2D draws and restores its cursor at the same inset position; the rendered edge
strip went from 9,344 stale pixels to zero after moving back to the centre.

The strict provenance audit verifies 7,757 pinned files across 65 imported games
and one repository game. Across the collection, 36 imported games have documented
source corrections affecting 80 files. Each cumulative source patch reverses to
its pinned originals byte for byte.

Verification for the final shared runtime and control configuration:

- 38 CTest checks and 38 browser tests passed, including actual packet travel,
  multi-touch ownership, configured gain and cursor re-entry.
- 40 compiled guest-source regressions passed, with failing-before evidence for
  the repaired boundaries.
- Configuration, collection, source-inventory fixtures, image packaging, Android
  source export/relocation, 99 gallery states and Remotion typechecking passed.
- Web checks cover all 66 launch/first-frame/Home paths, 132 decoded images,
  four responsive layouts and loading/retry/search/cancellation flows.
- Actual rendered desktop checks cover the reported Memory Match/Ninjix cases,
  corrected edge artwork, menu/game transitions, and Euler's first movement.

Evidence remains under `build/arcade-wheel/`, `build/arcade-cursor-audit/` and
`build/arcade-cursor-secondhalf/`. Android evidence uses an API 35 ARM64 emulator.
The checks establish short gameplay objectives and controls; they do not establish
completed campaigns or physical-phone performance.

Tower Defense's authored profile now enables its existing mouse-reporting
prerequisite. The actual-game browser regression failed with a static cursor
before the profile fix, then passed physical motion, whole-cursor corner drawing,
direct touch and touch-to-Play using the identical WebAssembly module. A separate
66-game reporting audit confirms reachable initialization and reply handling for
all 57 enabled pointer profiles; the nine pointer-off profiles retain their
intended controls. Gameplay checks use the authored environment and record extra
overrides, rather than accepting a renderer-only test as pointer-input proof.

## Publication

Source: `40beb860e6f30500905138b9a8d9f1ed2c130e24`, committed and pushed.

[arcade.rgo.pt](https://arcade.rgo.pt) is deployed with Worker version
`79ce85fb-e601-454e-b73a-6dbc6421a172`. The web gate covers all 66
launch/first-frame/Home paths, 132 decoded images, four responsive layouts and
four wrapper flows. The final Tower-only configuration update also passes actual
live physical mouse, direct touch, Play, slot selection and Archer placement:
coins fall from 300 to 250 and a projectile is visible. All 600 active static
files match the final publication package byte for byte, and all 66 game modules
pass decoded-hash/delivery checks.

The live focused cursor suite passed 44 checkpoints with no page errors,
including Memory Match's bottom edge, pause/resume, LCOMaze's menu return and
PettyCats' complete corner artwork. Ninjix's published menu and gameplay cursor
align at x100,y25. Portal's edge-to-centre strip contains zero stale pixels, with
all 584 sampled cursor pixels matching. Euler's first 64-pixel movement and
physical-pointer exit/re-entry pass. These unchanged game modules retain their
verified hashes in the final publication.

[Android release](https://github.com/rodrgds/lcom-ng/releases/tag/arcade-2026-10-16-steering):
ARM64, 354,749,208 bytes, same application ID and signing certificate.
APK SHA-256: `7535f0a8f3e373d7cefb6e8156c90a4979357b1994e8efa58d11434fb10bef01`.
Local artifact: `dist/lcom-arcade-android-20261016-release-ready/collection.apk`.

Android coverage comprises a full 66-game sweep in 67 fresh isolated game
processes, with search, saved-file relaunch, forced-process-exit containment and
Home/launcher isolation. Every first frame was visually reviewed. The last
profile-only update changes Tower Defense; its final APK passes actual touch
Play, slot selection, Archer placement, both cursor corners and Home return.
Independent comparison verifies unchanged profiles and native code/data for the
other 65 games and three host/dependency libraries, excluding GNU build IDs.
A second full sweep of the final package is not claimed.

Focused gameplay also verifies the exact former Star Defender crash gesture,
Euler mission completion, Portal paired portals and repeated level return/restart,
and Teclas' full cursor at both corners. The final package verifies all 66
profiles, 5,960 selected source hashes, 69 ARM64 libraries, 16 KiB ELF/ZIP alignment,
its signature and an actual in-place update retaining private saved files.

Final detailed Android evidence:
`build/arcade-wheel/release-ready-android/final-release-qa.json` and
`unchanged65-binary-and-profile-proof.json`. Generated runtime source copies were
reclaimed after successful linking to fit the disk budget. Frozen Git runtime
inputs and archived generated headers were verified; no post-cleanup hash check
of removed runtime source copies is claimed. Earlier APKs and test evidence are
preserved. Temporarily removed generated raw web modules were restored from
independently verified lossless gzip copies.

The public APK download was streamed in full and matched the stated SHA-256 and
byte count. The release tag resolves to the stated source revision. The stable
student SDK latest release remains `v0.2.0`.
