# Developer guide

For people maintaining lcom-ng itself: runtime code, lab scaffolding, docs,
tests, examples, and releases. Treat the public course kit and the host runtime
as first-class: a runtime change that breaks the student workspace, or docs that
drift from the runtime, is incomplete.

## Current delivery focus

The browser Arcade is the active delivery surface. Its collection and manifest
live in `collections/arcade/`; the published site is <https://arcade.rgo.pt/>.

Android builds, emulator checks, and publication are paused. Preserve
`sdk/android/`, the shared exporter, and signing keys. Qualify web imports in the
browser; Android qualification is not a web admission gate.

## Audience boundaries

The audience table and cross-surface rules are in
[CONTRIBUTING.md](../CONTRIBUTING.md#audience-boundaries). Read them before
changing a public artifact.

## Source map

| Path | Ownership |
| --- | --- |
| `runtime/` | C++ host runtime, devices, backends |
| `runtime/cli/` | CLI helpers, course catalog, setup/test support |
| `common/` | process protocol shared by runtime and C client |
| `sdk/client/` | C client linked into student programs |
| `sdk/include/lcom/` | public student SDK headers copied as `include/lcom/` |
| `sdk/compat/` | Minix/LCF compatibility SDK and project builder |
| `sdk/export/` | shared export configuration and source discovery |
| `sdk/android/` | Android export, preserved while Android work is paused |
| `course/labs/templates/` | APIs copied into generated workspaces |
| `course/labs/docs/` | student-facing handouts |
| `course/labs/function-requests.json` | structured lab truth for CLI/help |
| `docs-site/` | VitePress public website |
| `docs/` | package-local reference and evidence, not a second website |
| `dev/lab-solutions/` | reference implementations for tests and review |
| `dev/lab-tests/` | predefined student-lab checks |
| `examples/` | examples, project seeds, and pinned community sources |
| `collections/arcade/` | Arcade manifest, per-game settings, previews |
| `scripts/` | deterministic replay, capture, and packaging scripts |
| `tests/integration.sh` | end-to-end repo contract |

Architecture and runtime behavior are documented on the public
[architecture](../docs-site/developers/architecture.md) and
[runtime and CLI](../docs-site/developers/runtime-cli.md) pages.

## Change rules

- Keep `sdk/include/lcom/` small, C-only, and free of host backend types.
- Keep generated workspace text student-facing and short.
- Keep CLI strings, lab aliases, and generated workspace layout aligned with
  `course/labs/function-requests.json` and `runtime/cli/`.
- Keep the `lcom`, `machinelab`, `lowlab`, and `mlab` aliases working.
- Do not make students learn repo internals to pass labs.
- Prefer one shared helper over repeated generated text.
- Add public docs and integration checks when adding public features.
- Keep binary demo artifacts reproducible from scripts or documented commands.
- Keep public guides in `docs-site/`; put maintainer notes and evidence in
  `docs/` instead of copying a public page.

## Adding or changing a lab

Update these surfaces together:

1. `course/labs/function-requests.json`
2. `course/labs/templates/labN/include/`
3. generated starter source in `runtime/cli/FileOps.*` if needed
4. `dev/lab-solutions/labN/`
5. `dev/lab-tests/labN_test.c`
6. `course/labs/docs/labN-*.md` and the matching `docs-site/labs/labN/` pages
7. `course/README.md`
8. public lab track and role guides if the course map changes
9. integration tests

Fresh stubs should compile and fail meaningfully. Completed reference solutions
should pass the predefined checks.

## Adding or changing a device

Check:

- port map and register names;
- public SDK header shape;
- runtime device state;
- IRQ behavior;
- script/replay support, if input driven;
- headless artifact support, if visual or audio;
- example program;
- lab or project use case;
- docs and external references.

Do not expose runtime internals through public headers for one example's
convenience.

## Test gate

```sh
devenv shell -- lcom-ng-verify
```

Narrower checks:

```sh
devenv shell -- ctest --test-dir build -R integration --output-on-failure
devenv shell -- lcom-ng docs cli
devenv shell -- lcom-ng setup build/student-smoke --force
devenv shell -- lcom-ng test rtc --project build/student-smoke
```

Browser changes also run through `lcom-ng-web-test`, which includes real
multi-touch keyboard input. For visual or audio changes, capture artifacts:

```sh
devenv shell -- lcom-ng run --headless \
  --script scripts/music_maker_demo.mlabscript \
  --audio-wav build/music.wav \
  --dump-frame build/music.ppm \
  -- build/examples/music_maker
```

## Release workflow

GitHub Actions builds SDL-enabled Linux x86_64/arm64, macOS x86_64/arm64, and
Windows MSYS2 x86_64 student archives, each containing the SDK, desktop
dependencies, lab assets, and checksums. Tagging, candidate builds, packaging
owners, and platform constraints are in the
[release guide](../docs-site/developers/releases.md).

## Licensing

Code is MIT licensed. Course and documentation material is CC BY 4.0. Keep new
files aligned with that split.

The site is VitePress, deployed to Cloudflare Pages. Build, asset sync, checks,
and deployment details are in [Docs Site](../docs-site/developers/docs-site.md).
