# Getting started

This page covers building and testing the repository itself. For the stable
architecture and release contracts, use the public
[developer guide](https://lcom.rgo.pt/developers/). For a generated workspace,
use the [student guide](https://lcom.rgo.pt/students/).

## Recommended environment

The Devenv shell provides the compiler, CMake, Ninja, CLI11, SDL3, SDL3_ttf,
ffmpeg, and Bun tools used by the test and demo workflow.

```sh
devenv shell
lcom-ng-install
lcom-ng-verify
```

Shell helpers:

| Command | Action |
| --- | --- |
| `lcom-ng` | run `build/machinelab`, building the CLI first if missing |
| `lcom-ng-install` | install locked VitePress and Remotion dependencies |
| `lcom-ng-build` | configure and compile an SDL-enabled debug build |
| `lcom-ng-test` | build and run the complete CTest suite |
| `lcom-ng-verify` | run CTest, docs checks, Remotion typecheck, and `git diff --check` |
| `lcom-ng-replay-flappy-video` | render `build/replays/flappy.mp4` |
| `lcom` | compatibility alias for `lcom-ng` |

The first `devenv shell` may take time while Nix resolves dependencies.

## Plain CMake

Install a C11/C++17 toolchain, CMake 3.20+, Ninja and CLI11:

```sh
cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

For interactive windows, also install SDL3 and SDL3_ttf and configure with
`-DMACHINE_LAB_WITH_SDL=ON`.

## Execution model

Always start a student binary through the runtime:

```sh
lcom-ng run build/examples/hello
```

An SDL-enabled build selects SDL by default; a headless-only build selects the
deterministic backend. Use `--headless` explicitly in tests, scripts, and docs so
results do not depend on build configuration:

```sh
lcom-ng run --headless -- build/examples/timer_int 3
```

The `--` separator ends runtime options. Use it whenever the student program has
arguments or a command should be unambiguous.

## Daily development loop

```sh
lcom-ng-build
ctest --test-dir build --output-on-failure
lcom-ng run build/examples/breakout_demo
```

Single groups:

```sh
ctest --test-dir build -R unit --output-on-failure
ctest --test-dir build -R integration --output-on-failure
```

Integration artifacts go under `build/test-output/`.

## Student workspace

Generate a workspace outside this repository, or use the ignored `student/`
directory for a local demonstration:

```sh
lcom-ng setup student
make -C student
lcom-ng test rtc --project student
```

`lcom-ng setup` does not overwrite existing starter files unless passed
`--force`. The public [student guide](https://lcom.rgo.pt/students/) documents
the generated layout; the [Lab Track](https://lcom.rgo.pt/labs/) covers course
progression.

## Troubleshooting

- `lcom-ng: command not found`: enter `devenv shell`, or use
  `build/machinelab` directly.
- SDL package or configuration errors: use the dev shell or configure a headless
  build with `-DMACHINE_LAB_WITH_SDL=OFF`.
- A graphical program appears hung under `--headless`: supply an exit script
  and, while debugging, a `--max-ticks` guard.
- MP4 rendering fails: confirm `ffmpeg` is on `PATH`.
- Fresh generated labs fail: expected; TODO stubs are starting points, not
  solutions.
