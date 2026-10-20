# Contributing

lcom-ng is both courseware and a runtime. Keep student material, instructor
material, and developer internals separate.

Public documentation is canonical at <https://lcom.rgo.pt/> and built from
[`docs-site/`](docs-site). Files in [`docs/`](docs) are package-local reference
and evidence, not a second website.

## Audience boundaries

| Audience | Stable surface | Keep out |
| --- | --- | --- |
| Students | generated workspace, public SDK headers, lab handouts, `lcom-ng test` | reference solutions, CMake, CI, runtime protocol internals |
| Instructors | adoption guide, instructor guide, lab map, examples, replay scripts, selected grading checks | accidental spoilers that solve the whole lab |
| Developers | this repository, runtime code, release workflow, test fixtures | unclear public copy or duplicated course truth |

When a patch changes one surface, check the adjacent surface. A new lab function
usually needs an SDK/header update, a catalog entry, a generated workspace
update, a public lab page, a reference solution, a predefined test, and CLI help
in the same patch.

## Local setup

With [Devenv](https://devenv.sh/):

```sh
devenv shell
lcom-ng-install
lcom-ng-test
```

For a plain CMake build and the full helper list, see
[Getting started](docs/getting-started.md). Core headless tests must pass without
a desktop session.

## Change checklist

- Keep generated student workspaces simple: plain `make`, C files, headers, and
  `lcom-ng test`.
- Keep public C headers in `sdk/include/lcom/` C-only and free of runtime C++
  types.
- Keep lab contracts aligned across `course/labs/function-requests.json`,
  `runtime/cli/`, templates, tests, solutions, and public lab pages.
- Keep reference implementations under `dev/lab-solutions/`.
- Keep predefined lab checks under `dev/lab-tests/`.
- Keep public guides in `docs-site/` and student lab handouts in
  `course/labs/docs/`. Put package-local reference and evidence in `docs/`.
- Prefer small deterministic examples over interactive-only demos.
- Add or update integration checks when you add a public artifact.
- Qualify browser changes in a browser, including multi-touch keyboard input.
- Keep original Arcade game assets unchanged and per-game settings in each
  game's `lcom.json`.

Android builds, emulator checks, and publication are paused. Do not treat
Android qualification as a condition for admitting a web change, and preserve
the implementation and signing keys.

## Docs style

State what the student should implement and what they should not. Use
`> [!NOTE]`, `> [!TIP]`, and `> [!IMPORTANT]` for context, technique, and
correctness requirements. Link external references when useful, but keep the lab
self-contained. Never paste complete solutions into student docs.

## Testing

Run the full gate before opening a pull request:

```sh
devenv shell -- lcom-ng-verify
```

The [developer guide](docs/developer-guide.md) lists narrower checks. Fresh
generated lab stubs should compile and fail the predefined tests until a student
implements the missing functions.

## Pull requests

Include the user-facing reason, the affected audience, and test commands with
results. Add visual or interactive evidence and note any skipped surface.

By contributing, you agree that code contributions are licensed under MIT and
course/documentation contributions are licensed under CC BY 4.0.
