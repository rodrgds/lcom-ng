# Native showcase UI

This guide covers the C framebuffer apps in `examples/`, presented by the SDL
runtime. The current code and captured app output define their appearance.
Ninjix is a benchmark for completeness and polish, not a palette, typography,
or layout template for the other apps.

## Shared drawing and type

- `examples/example_ui.h` owns the shared `UI_*` colors, label measurement,
  text drawing, rounded surfaces, buttons, headings, and modifier handling.
  Reuse these helpers before adding another local implementation.
- Light workspaces use `UI_BG`, `UI_PAPER`, `UI_INK`, `UI_MUTED`, and `UI_LINE`.
  Dark tools use `UI_DARK`, `UI_PANEL`, `UI_LIGHT`, and `UI_DIM`. `UI_ACCENT`
  marks shared actions; app-specific colors still describe ink, tracks, tiles,
  device activity, or the aquarium itself.
- `example_label` draws proportional DejaVu Sans, with native 28px bold headings at scale
  2 and full-size 96px timer numerals. Measure with `example_label_width`, using the same scale as the drawing.
  `example_mono` uses DejaVu Sans Mono with a nine-pixel advance.
- `examples/example_font.h` contains generated, four-bit coverage tables for
  printable ASCII. The apps require no font library at runtime. Unsupported
  characters render as `?`; keep labels within the supported set.
- To regenerate from the repository root, run
  `devenv shell -- python3 scripts/generate_example_font.py /path/to/DejaVuSans.ttf`.
  Generation requires Pillow and sibling `DejaVuSans-Bold.ttf`, `DejaVuSansMono.ttf` and
  `DejaVuSansMono-Bold.ttf` files. Preserve `examples/example-font-LICENSE.txt`
  alongside the generated font data.
- Retain `example_draw_text` and its small pixel glyphs where compact grid
  labels, byte dumps, counters, or game artwork need them. Keep diagnostic
  values and the word processor's editable text monospaced. Do not smooth
  authored sprites, tiles, or pixel art just to match the app chrome.

## Layout and controls

These tools use an 800 by 600 framebuffer. Layout and hit testing use guest
framebuffer pixels, not host-window points. Keep each app's task central:
document, sprite canvas, sequencer, map, shared board, device graph, or clock.
Preserve the task-specific layout rather than forcing every app into one shell.

`example_heading` supplies a 76-pixel header with a 24-pixel left inset.
Editors can use side tools and a bottom status strip; dark monitoring tools can
retain dense graphs and telemetry. Keep labels clear of canvases and controls.

- Draw and hit-test each clickable control with the same rectangle. Update
  hover tests, mouse handlers, grid conversions, and replay coordinates when
  moving it. A label or keyboard hint alone does not make an area clickable.
- `example_button` owns shared idle, hover, active, and disabled rendering,
  with six-pixel corners. Pass actual selection and availability state.
  Disabled rendering must agree with the action handler, including undo/redo.
- Keep changing state visible through labels, selection, status, or progress.
  A selected tool, active playback, unsaved file, and failed transfer have
  different meanings and must follow the owning app's actual state.

## Input and help

Use `example_modifiers_t`, `example_modifier_byte`, and `example_primary` for
primary shortcuts. Primary means Ctrl or Command. The helper tracks both sides
independently and understands PS/2 extended Command bytes. Process modifier
make/break bytes before filtering editing input, including while help is open.
Consume primary chords so Command/Ctrl+C cannot fall through to a bare C clear
action. Keep shortcut labels and implemented actions aligned.

Help must capture editing input and mouse actions. Continue tracking modifier
releases, pointer position, and button releases while it is visible, so closing
it cannot leave a stuck modifier or create a phantom press. F1 toggles help;
some tools also use H. Escape closes help before a later Escape leaves the app.
Check extended keys and undo/redo as well as printable keys beneath the overlay.

`runtime/backends/SdlBackend.cpp` owns host input capture. Command reaches the
guest. Right Ctrl releases the captured mouse; F3 also opens the host debug
overlay and releases it. Do not reserve Command as a host escape key.

## Verification and captures

Run project commands through Devenv. After `devenv shell -- lcom-ng-build`:

```sh
devenv shell -- ctest --test-dir build -R '^(studio_shortcuts|sdl_render)$' --output-on-failure
```

- `tests/studio_shortcuts.py` exercises real guest PS/2 replay paths for save
  and reopen, Command undo/redo, safe chords, help capture, mouse notes, and
  paired whiteboard input. Extend it when an observable input contract changes.
- `tests/sdl_render_tests.cpp` checks SDL dispatch, Command release on focus
  loss, framebuffer conversion, opacity, and palette changes. CTest runs it
  with SDL's dummy video driver; it does not prove real-window usability.
- Open the changed app through `lcom-ng run build/examples/<app>` and check
  actual mouse alignment, shortcuts, help, focus changes, and audio as relevant.
- `scripts/capture_marketing_examples.sh` owns the original app/game captures;
  `scripts/capture_showcase_examples.sh` owns the additional tool screenshots
  and artifact assertions. Inspect resulting images in `docs/assets/`.
- Finish cross-surface changes with `devenv shell -- lcom-ng-verify`. Preserve
  authored replay behavior and verify exported files, not only screenshots.

## Public video ownership

`scripts/render_public_demo.sh` captures inputs, composes audio, and renders
`docs/assets/demo/machine-lab-demo.mp4`. `media/remotion/src/Video.tsx` owns the
film's composition; `Root.tsx` owns its dimensions and duration. The asset sync
script in `media/remotion/scripts/` copies footage, screenshots, and bundled
typefaces into the generated Remotion public directory.

`scripts/capture_launch_clips.sh` derives caption-free clips from the authored
app replays. Keep actual footage readable and free of replay captions inside
the app artwork. Film titles live in Remotion, outside that footage.
`scripts/compose_launch_audio.py` creates the original quiet ambient score with
no sampled audio or percussion; Remotion mutes raw game and sequencer tracks.
After `devenv shell -- sh scripts/render_public_demo.sh`, inspect scene cuts,
readable app content, paired-board captures, and the final audio by listening.

## Host text and persistent state

`lcom_clipboard_set_text` and `lcom_clipboard_get_text` bridge the native host
clipboard through the public SDK. SDL uses the system clipboard. Headless
instances use a private per-guest clipboard so tests never replace the user's
clipboard. The bridge accepts at most 4091 text bytes plus a terminator and
rejects oversized reads without changing the caller's buffer. Notes validates
its supported ASCII character set before pasting.

Notes and Composer compare their content with the saved snapshot when deciding
whether to offer an unsaved-close prompt. Derive this state from the content,
including after undo or reversing a toggle. Atomic saves use an exclusively
created sibling temporary file, check writes and close, then rename. An invalid
or unreadable existing file must not be replaced with a default document.

Every showcase has a dedicated page under `docs-site/examples/`. Its
`ExampleGallery` reads canonical capture metadata. Add useful replay states to
`scripts/showcase-gallery.json`, then regenerate with
`devenv shell -- python3 scripts/capture_gallery.py`. `--app <slug>` focuses a
run; `--check` validates integrity and source freshness using Python's standard
library. Pillow is needed for capturing, and belongs to the Devenv shell.
