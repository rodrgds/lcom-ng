# lcom-ng Agent Guide

Treat this as a concise operating map for recurring work. Prefer the named
commands and source-of-truth rules below over rediscovering the repo on every
task.

## Start Here

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
- Student starter contracts: `course/labs/templates/`; reference solutions and
  predefined checks: `dev/lab-solutions/` and `dev/lab-tests/`.
- Public website source: `docs-site/`. `docs-site/public/assets/` and the
  VitePress build directory are generated; do not hand-edit or stage them.
- Showcase UI primitives and input modifiers: `examples/example_ui.h`. The
  baked type tables in `examples/example_font.h` keep apps self-contained;
  regenerate with `scripts/generate_example_font.py` (Pillow and DejaVu fonts).
  Help overlays must consume editing input while maintaining button/modifier
  releases. Command is guest input, Right Ctrl is host mouse release. Keep
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
