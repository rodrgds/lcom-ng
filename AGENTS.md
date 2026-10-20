# lcom-ng Agent Guide

Treat this as a concise operating map for recurring work. Prefer the named
commands and source-of-truth rules below over rediscovering the repo on every
task.

## Start Here

Current delivery focus is the web Arcade. Android builds, emulators, publication
and CI are paused until the user resumes Android work. Qualify web imports in
the browser; Android qualification is not a web admission or deployment gate.
Preserve Android implementation and signing keys for future builds.

For non-trivial work, run this once:

```sh
devenv shell -- agent-preflight
```

It is read-only. It reports the branch/worktree, tool availability, dependency
state, versions, and the canonical verification command. Treat every existing
change as user-owned and do not rerun the preflight unless the environment
changes.

Use the Devenv shell for project commands. Do not diagnose missing CMake, SDL,
FFmpeg, Node, or Bun from the host shell until the same command has been tried
through `devenv shell -- ...`.

## Stable Sources Of Truth

- Public product/course name and CLI: **lcom-ng**. New public docs use
  `lcom-ng`. Keep `machinelab`, `lcom`, `lowlab`, and `mlab` aliases, existing
  canonical asset filenames and C API identifiers compatible. The canonical public
  website is `https://lcom.rgo.pt`; the existing Cloudflare Pages resource keeps
  its internal `machine-lab-docs` project ID.
- Lab catalog and generated-workspace metadata:
  `course/labs/function-requests.json`. CMake generates the runtime lab catalog
  from it. Do not restore a second hand-maintained catalog.
- Public C API: `sdk/include/lcom/`; host implementation:
  `runtime/`; C client/protocol boundary: `sdk/client/`.
- Browser exports: read `docs-site/students/browser.md`. Keep the web SDK source
  payload relocatable through `scripts/package_student.py`; browser regressions
  run through `lcom-ng-web-test`, including real multi-touch keyboard input.
- Optional project identity and mobile controls: `lcom.json`, loaded by
  `sdk/export/config.py` for both exporters and desktop bundles. CLI options
  override file values. Read `docs-site/students/project-config.md`; keep
  `sdk/export/lcom.schema.json` aligned with validation. Native control glyphs
  in `runtime/embedded/ControlFont.hpp` regenerate with the example font tables. Mobile settings do not
  change the normal desktop runtime. Example profiles live beside their source
  folders, including wrapper folders for the flat C examples.
- Game collections: `lcom.collection.json` lists project folders relative to its
  directory. `sdk/collection/` owns validation and native Android/responsive web
  launchers; `collections/arcade/` is the example manifest. Browser gameplay
  previews live under `collections/arcade/previews/`, with per-game `preview`
  paths and `description` in `lcom.json`. Keep original game assets unchanged. Use shared `archetype`
  names for variants of the same game family; keep each variant’s pinned source
  identity and individual launch controls. Per-game settings stay
  in each game's `lcom.json`. Optional collection `web.json` points to web-only
  `game-metadata.json` and `gallery.json`; year evidence and featured selection
  belong there. Arcade displays academic school years from original reports/README evidence
  or game-specific development history. Keep inference confidence in metadata
  and evidence tooltips; show plain school-year labels and omit unknowns.
  Keep repository creation timestamps separately as provenance. Source-reviewed
  genre/player tags describe modes available in the exported surface. Captured gallery frames live in `collections/arcade/galleries/`.
  Pinned student sources under `examples/community/`
  retain `UPSTREAM.json` hashes and separately recorded source-defect patches.
  Keep Android game libraries in the isolated `:game` process, with fresh globals
  on each launch and private saved files preserved. Collection exporters ship in
  the installed SDK via `scripts/package_student.py`.
  Native source staging must preserve both filename case and exact Unicode bytes.
  Use case-sensitive APFS for NAS-backed staging on macOS; case-sensitive HFSX
  normalizes NFC filenames and can break original game asset lookups. On this
  case-insensitive checkout, `examples/community/LuisRelvas--ProjectLCOM` is
  mounted from `build/luisrelvas-case-sensitive.sparseimage` so both
  `Easy.xpm` and `easy.xpm` exist; remount there after a reboot.
  Stage imported source files hidden by upstream ignore rules explicitly. Run
  `scripts/check_collection_sources.py --require-tracked` before publishing;
  provenance must survive a fresh checkout. Exclude OS and VCS metadata from the
  required source inventory and record those omissions in `UPSTREAM.json`.
  Preserve case-distinct upstream paths on a case-sensitive checkout. Verify all
  original hashes after extraction; immediate copy checks can miss later aliases.
  Such projects also require case-sensitive Android output staging.
  On synced/NAS storage, recheck a distinct-case probe after an idle interval.
  If the service renames conflicts, stage inside an opaque case-sensitive image.
  Leave mounted image backing files and their mount points in place when offloading build artifacts.
- Embedded app exports: `runtime/embedded/` owns the in-process C SDK; browser
  and SDL hosts supply time, event pumping, graphics and audio. Keep device logic
  in `Machine`. Android export lives in `sdk/android/`, shared source discovery
  in `sdk/export/`, and installed runtime sources in `sdk/runtime/`. Read
  `docs/embedded-runtime.md` when changing hosts and `docs-site/students/android.md`
  when changing packaging; verify with `python3 tests/android_export.py`
  inside Devenv, adding `--build-apk` when Android tools are available.
- Direct touch presets require verified guest cursor initialization, unit deltas
  and clamp bounds. Read `docs-site/students/project-config.md#direct-touch` before
  enabling one. Keep packet scheduling and cursor tracking shared in
  `runtime/embedded/Pointer.hpp`; preserve original game sources.
  Check visible cursor artwork at every edge and across menu/game transitions,
  including transparent sprite margins. A clamped hotspot can still leave the
  whole glyph offscreen. Verify reporting commands and complete-packet handling
  in the real game loop before enabling direct or calibrated browser input.
  Run gameplay checks with the authored `build.environment`, recording extra
  overrides. Accept pointer profiles after real motion and menu/game selection pass.
  Trace the loaded sprite, its renderer's transparency key and framebuffer write
  bounds. `pointer: "off"` disables screen touch, not the physical desktop mouse.
- Native exports link selected student source objects directly into the app.
  Keep data-only units in that link, including when legacy headers contain
  tentative globals. An intermediate archive can silently drop initialized tables.
  NDK candidate checks compile actual objects with `-O2 -D_FORTIFY_SOURCE=2`
  and the Android toolchain's `-Wformat -Werror=format-security` flags.
  Source Makefiles alone omit flags added by CMake's NDK toolchain.
  When admitting an import to Android, verify the resulting APK with actual
  native gameplay.
- Minix/LCF compatibility SDK: `sdk/compat/include/` and `sdk/compat/src/`,
  linked through the separate `lcom_minix` library. Keep its legacy declarations
  separate from the native SDK. For legacy project work, read
  `docs/from-minix.md` and the compatibility corpus report before claiming support.
  The expanded pinned corpus is `tests/compat-expanded.json`; its replay gate is
  `devenv shell -- python3 scripts/check_lcom_corpus.py --fetch`. Report normal
  exits, guarded captures and source defects separately. For macOS student
  distribution, use `scripts/package_macos.py` and `tests/macos_bundle_smoke.py`
  to verify the relocated archive outside Devenv.
  The legacy project builder is `sdk/compat/build_project.py`; the CLI dispatches
  it through `lcom-ng compat build`. Keep original project sources unchanged.
  `sdk/compat/assembly.py` lowers literal basic CLI/STI and the supported single
  volatile byte IN/OUT templates into generated compiler inputs for native, web
  and Android. Other assembly keeps explicit diagnostics. Byte operands must be
  character types and ports must satisfy the original 16-bit/8-bit constraints;
  verify exact I/O and single expression evaluation with `compat_byte_ports`.
  Preserve original files, source
  diagnostics and quoted includes. Its process-local interrupt flag defers and
  coalesces HARDWARE notifications; it must not drop IRQs or count nested CLI.
  Keep `sys_irqsetpolicy`'s input notification bit distinct from its returned
  opaque handle. Verify both with `compat_notifications` and `compat_interrupts`.
- Student starter contracts: `course/labs/templates/`; reference solutions and
  predefined checks: `dev/lab-solutions/` and `dev/lab-tests/`.
- Public website source: `docs-site/`. `docs-site/public/assets/` and the
  VitePress build directory are generated; do not hand-edit or stage them.
- Showcase UI primitives and input modifiers: `examples/example_ui.h`. The
  baked type tables in `examples/example_font.h` keep apps self-contained;
  regenerate with `scripts/generate_example_font.py` (Pillow and DejaVu fonts).
  Help overlays must consume editing input while maintaining button/modifier
  releases. Command is guest input. Right Option on macOS and Right Ctrl release the mouse.
  Browser physical mouse input uses click-to-capture Pointer Lock with relative
  movement and retained fractional deltas. Escape reaches the game. Fullscreen
  Keyboard Lock retains capture where supported; browser-owned windowed Escape
  unlock still forwards it once.
  Shift+Escape first releases capture, then returns to the collection when
  already released. Native browser unlock can consume
  Escape before DOM key events, so distinguish it from blur and programmatic
  release. Touch keeps its authored relative/direct controls. Keep
  shortcuts, hit targets, authored replays and screenshots aligned. See
  `docs/showcase-ui.md` before changing shared UI or help/input behavior.
- Example galleries: `scripts/showcase-gallery.json` owns the real replay states,
  fixtures, labels and source inputs. Regenerate all with
  `devenv shell -- python3 scripts/capture_gallery.py`, or select `--app <slug>`.
  `--check` validates freshness. Canonical PNGs and metadata live under
  `docs/assets/screenshots/gallery/`; every app has a docs page with `ExampleGallery`.
  Captures use `--dump-presented` so a tick guard cannot expose an unfinished repaint.
- PvZ preserves the original student project's UI, art and controls. Keep platform
  adaptations separate from game presentation; source identity is recorded in
  `docs/compatibility-results.md`.
- Canonical public video: `docs/assets/demo/machine-lab-demo.mp4`. Its source is
  `media/remotion/`, and its deterministic inputs are produced by
  `scripts/capture_marketing_examples.sh`.
- Student release work: read `docs-site/developers/releases.md`. Package the
  whole installed SDK and SDL dependencies, verify after relocation with a system
  tool PATH, and report platform CI evidence separately from the local macOS trial.
- Version: keep `project(... VERSION ...)` in `CMakeLists.txt` aligned with the
  root `package.json`; CMake enforces this.
- Audience boundaries and public/internal ownership: `CONTRIBUTING.md`.

## Named Workflows

- First checkout or missing JS dependencies:
  `devenv shell -- lcom-ng-install`.
- Focused C/C++ iteration: `devenv shell -- lcom-ng-build`, then a targeted
  `ctest --test-dir build -R <name> --output-on-failure`.
- Complete runtime/lab/integration suite:
  `devenv shell -- lcom-ng-test`.
- Browser framebuffer conversion benchmark:
  `devenv shell -- python3 scripts/benchmark_web_frames.py`. Use `--source-root`
  for a baseline checkout and `--cpu-rate 4` for a constrained-CPU sample.
  Timing budgets (`--max-ms`) are local checks, never shared CI gates. Preserve
  exact colors, opaque alpha, palette writes, row pitch and Wasm heap growth.
- Cross-surface final gate:
  `devenv shell -- lcom-ng-verify`. This builds and tests lcom-ng, checks
  the docs site and canonical assets, typechecks the Remotion project, and runs
  `git diff --check`.
- CLI/docs behavior: inspect the real `build/machinelab --help`, subcommand
  help, and `lcom-ng docs cli` before changing prose.
- Public demo regeneration:
  `devenv shell -- sh scripts/render_public_demo.sh`. Inspect the resulting MP4
  and relevant stills/audio; a successful render alone is not visual QA.
- Generated student flow: create it under ignored `build/` or `/student/`,
  compile it with `make`, and confirm a fresh TODO lab fails its predefined test
  with a useful message.

## Change Acceptance Map

| Change | Check together |
| --- | --- |
| Runtime, protocol, or device behavior | single and paired runtime owners, focused regression, full CTest |
| Lab function or lab order | JSON catalog, templates, public header, solution, predefined test, handout, generated workspace |
| CLI command or help | CLI behavior, generated CLI Markdown, completion, integration assertions, docs |
| Example or replay | headless deterministic run, bounded exit path, artifact contents, integration test, example docs |
| Screenshot, audio, or video | capture source, canonical asset, README, docs site sync/check, rendered output |
| Identity or version | CMake, package metadata, CLI aliases, docs, workflows, release artifact names |

Start behavior changes with a failing regression or reproducible command. For
interactive bugs, keep the deterministic regression and also test the real SDL
path; headless success does not prove responsiveness, timing, input, audio, or
render quality.

## Working And Review Defaults

- Audit/explanation requests are read-only. For broad repo reviews, prioritize
  architecture, correctness, performance, error propagation, duplication, and
  maintainability. Do not spend the review on cybersecurity or dependency
  advisories unless the user explicitly asks for that scope.
- Give audit findings with exact current-file evidence and a reproduction or
  test. Rank findings; do not inflate a clean review with speculative issues.
- Keep public/docs copy concise: short bullets, short labels, and focused
  audience-specific guides. Avoid turning one point into a paragraph.
- “Everywhere” means checking the adjacent consistency surfaces in the table
  above, not doing a blind repo-wide rewrite.
- Preserve dirty work. Inspect overlapping diffs before editing, and never
  stage unrelated generated or user-owned files.
- Generate isolated-work patches against an immutable starting snapshot.
  Diffing an older copy against the changing main workspace can erase other
  work. Review every deletion before integrating a patch.
- Commit, push, release, or deploy only when requested. If asked to commit a
  mixed worktree, inspect the full diff and split it by cohesive behavior
  (runtime/fix, showcase/docs, tests) rather than file count.
