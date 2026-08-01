# Machine Lab Agent Guide

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
FFmpeg, Node, or npm from the host shell until the same command has been tried
through `devenv shell -- ...`.

## Stable Sources Of Truth

- Public product/course name: **Machine Lab**. New docs and scripts use
  `machinelab`; `lcom`, `lowlab`, `mlab`, and `lcom_*` remain compatibility
  names where already required.
- Lab catalog and generated-workspace metadata:
  `course/labs/function-requests.json`. CMake generates the runtime lab catalog
  from it. Do not restore a second hand-maintained catalog.
- Public C API: `sdk/include/lcom/`; host implementation:
  `runtime/`; C client/protocol boundary: `sdk/client/`.
- Student starter contracts: `course/labs/templates/`; reference solutions and
  predefined checks: `dev/lab-solutions/` and `dev/lab-tests/`.
- Public website source: `docs-site/`. `docs-site/public/assets/` and the
  VitePress build directory are generated; do not hand-edit or stage them.
- Canonical public video: `docs/assets/demo/machine-lab-demo.mp4`. Its source is
  `media/remotion/`, and its deterministic inputs are produced by
  `scripts/capture_marketing_examples.sh`.
- Version: keep `project(... VERSION ...)` in `CMakeLists.txt` aligned with the
  root `package.json`; CMake enforces this.
- Audience boundaries and public/internal ownership: `CONTRIBUTING.md`.

## Named Workflows

- First checkout or missing JS dependencies:
  `devenv shell -- machinelab-install`.
- Focused C/C++ iteration: `devenv shell -- machinelab-build`, then a targeted
  `ctest --test-dir build -R <name> --output-on-failure`.
- Complete runtime/lab/integration suite:
  `devenv shell -- machinelab-test`.
- Cross-surface final gate:
  `devenv shell -- machinelab-verify`. This builds and tests Machine Lab, checks
  the docs site and canonical assets, typechecks the Remotion project, and runs
  `git diff --check`.
- CLI/docs behavior: inspect the real `build/machinelab --help`, subcommand
  help, and `machinelab docs cli` before changing prose.
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
- Commit, push, release, or deploy only when requested. If asked to commit a
  mixed worktree, inspect the full diff and split it by cohesive behavior
  (runtime/fix, showcase/docs, tests) rather than file count.

