# Mobile gameplay review, 15 October 2026

This records the initial 53-game review. The subsequent 65-game expansion and
current build evidence are in [the expansion review](arcade-expansion-review.md).

All 53 collection games have representative touch gameplay checks, including
movement, scoring, card selection, drawing, typing, piece movement and local
multiplayer. These are short sessions, not completed campaigns or proof that
every mode works. Each game's demonstrated actions and limits are recorded in
[`examples/community/catalog.json`](../examples/community/catalog.json), under
`verification.mobileGameplay`.

## Source boundary

At this stage, updated all 53 collection profiles and the standalone PvZ/Sushi profiles through
`lcom.json`. All 6,818 other game files match the snapshot before this review,
including their assets, upstream hashes and previously recorded source patches.
No student code or new source-defect patches were added during that stage.

## Shared player improvements

- Home uses a recognizable house icon. The native button has the same fill and
  borderless treatment as the other controls. Both launchers prefer unused
  letterboxing and avoid occupied control targets.
- The touch-mode switch is a 44-unit icon with a persistent active state.
  Top utility keys, including Esc, use their label width instead of stretching
  across the toolbar. Bottom thumb actions keep their larger targets.
- Actions and the utility toolbar can occupy any corner. A shared layout pass
  keeps them apart from movement pads, fullscreen and control disclosure buttons.
  Expanded keyboards can dock left, right or bottom, reserving actual game space.
- Direction pads have larger 52-unit targets and a transparent cross layout.
  Horizontal and vertical presets omit unused directions. WASD and keypad pads
  display their actual keys instead of suggesting they send arrow keys.
- `hold-drag` moves the cursor immediately without selecting on a quick swipe.
  Hold still for 180 ms, drag and lift to place in one gesture. Chess and
  Battleship use it; Bamboozlers also has a verified box drag/drop.
- Direct touch positions the cursor before pressing. A shared released-hover
  sample clears legacy menus' first-packet guards, including Minixsweeper.
  Direct presets require verified initial coordinates, unit gain and bounds.
- Optional `tapDuration` holds synthesized clicks across games' timer ticks.
  Survive the Blast and Memory Match needed 60 ms to register short taps.
  A new gesture, cancellation, focus loss or right-click clears that pulse.
- Move-only switching remains available for click-capable gestures. Profiles
  configured entirely for hover omit an otherwise ineffective switch.

Examples of actual play include Whack-a-Mole hits at the tapped hole; Chess
f2-f4 followed by the AI reply; Sushi score 1 from a real slice; PvZ deck
selection and planting; Minixsweeper reveal/flag; Memory Quest card reveals;
Survive the Blast Start and movement; Battleship ship dragging, rotation and firing;
and Grand Prix name entry, steering, drift and boost. Snake Garden exposes
separately named menu directions and a WASD movement cross. Space Invaders and
other horizontal games show only left/right movement.

Two-player checks include independent movement and releases in Sticky Ball,
Bomber Arena, Minix Legends and Dual Arena, plus a Quick Reflex round result.
Bomber Arena's second player uses real keypad codes. UART modes still require
a peer; extra local buttons do not substitute for one.

## Verification

The shared tests cover gesture edges and cancellation, timed tap release,
independent fingers, keypad and repeated keys, inactive pad axes, corner target
collisions, keyboard recovery, native pixels, relocation and return navigation.

```sh
devenv shell -- env EMCC_CORES=4 lcom-ng-verify
devenv shell -- python3 scripts/check_collection_sources.py
```

Current review evidence is under `build/mobile-ux2/`. Browser gameplay, native
emulator checks and physical-device performance are separate evidence.
Physical-phone performance has not been measured.

## Limits recorded during the initial review

- Minhoca's incorrectly widened mouse counter and PettyCats' inclusive framebuffer
  bounds were fixed in the subsequent source-authorized Arcade expansion. Both
  fixes have failing-before/passing-after compiled regressions, recorded source
  patches, browser gameplay and fresh native Android touch sessions. See
  [issue 11](https://github.com/rodrgds/lcom-ng/issues/11),
  [issue 12](https://github.com/rodrgds/lcom-ng/issues/12) and
  [the expansion review](arcade-expansion-review.md).
- Accelerated, warped or reset guest cursors retain relative movement. Whack's
  direct mapping is verified at the original default mouse sensitivity.
- UART multiplayer and FEUP Hero's external UART music helper were not verified.
- MINIX Pac-Man retains its original small maze within the larger framebuffer.

Configuration is documented in
[`project-config.md`](../docs-site/students/project-config.md).

## Previous published build, before expansion

The initial verified export was published at [arcade.rgo.pt](https://arcade.rgo.pt),
Worker version
`b2ab47f5-570e-4c09-9315-71b8afe3f90a`. All 53 production mobile
launch/render/return flows passed with zero control collisions, browser errors or
failed requests. All 53 decoded game modules, 163 shell/player/config assets and
53 decoded icons match or render correctly. Checks used normal DNS and TLS.

The initial ARM64 collection APK was `dist/lcom-arcade-android/collection.apk`,
315,973,005 bytes, SHA-256:
`2da36bbc58f8217f7a56ca0f7315c3aff68ecd50e157919adea435b7a092d665`.
Both standalone PvZ and Sushi APKs were refreshed too. All three signatures,
16 KB ZIP/ELF alignment and shared-runtime snapshots are verified. All 53
native configurations match their source profiles.

Native input sessions cover Chess, Whack-a-Mole, Survive the Blast, Space Invaders,
Snake Orchard and Sticky Ball. After the last quick-key sizing change, the final
APK was installed and Chess's keyboard, compact controls, continuous move, AI
reply and Home return passed again. Final standalone PvZ planting/zombie checks
and Sushi scoring also passed. The older 53-entry native launch sweep is retained
separately in the catalog; it was not repeated in this pass.

The complete gate passed 37 CTests, 25 browser tests, 11 configuration tests,
three collection tests, SDK relocation, 99 gallery states, docs and Remotion.
The final native sizing change additionally passed a fresh full CTest run and
Android source/relocation checks. Final docs checks passed separately.

Publication used the verified working tree, without a Git commit, tag or push.
Release inputs and coverage are recorded under `build/mobile-ux2/`, including
`deployment.json`, `release-inputs.json` and `native/QA.md`.
