# Getting Started

This page is for building and testing the lcom-ng repository itself. If you
are working inside a generated course workspace, use the
[student guide](student-guide.md) instead.

## Recommended Environment

The repository's `devenv` shell provides the compiler, CMake, Ninja, CLI11,
SDL3, SDL3_ttf, and ffmpeg used by the complete test and demo workflow.

```sh
devenv shell
lcom-ng-install
lcom-ng-verify
```

Available shell helpers:

| Command | Action |
| --- | --- |
| `lcom-ng` | run `build/machinelab`, building the CLI first if it is missing |
| `lcom-ng-install` | install locked VitePress and Remotion dependencies |
| `lcom-ng-build` | configure and compile an SDL-enabled debug build |
| `lcom-ng-test` | build and run the complete CTest suite |
| `lcom-ng-verify` | run CTest, docs checks, Remotion typecheck, and `git diff --check` |
| `lcom-ng-run-sdl` | build and open the focused SDL demo |
| `lcom-ng-replay-flappy-video` | render `build/replays/flappy.mp4` |
| `lcom` | compatibility alias for `lcom-ng` |

The first `devenv shell` may take time while Nix resolves dependencies.
Subsequent runs reuse them.

## Plain CMake

For a deterministic headless build, install a C11/C++17 toolchain, CMake 3.20+
and CLI11:

```sh
cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

For interactive windows, also install SDL3 and SDL3_ttf and configure with
`-DMACHINE_LAB_WITH_SDL=ON`.

## Execution Model

Always start a student binary through the runtime:

```sh
lcom-ng run build/examples/hello
```

An SDL-enabled build selects SDL by default. A headless-only build selects the
deterministic backend. Use `--headless` explicitly in tests, scripts, and
documentation so the result does not depend on build configuration:

```sh
lcom-ng run --headless -- build/examples/timer_int 3
```

The `--` separator ends runtime options. Use it whenever the student program
has arguments or when a command should be unambiguous.

## Daily Development Loop

```sh
lcom-ng-build
ctest --test-dir build --output-on-failure
lcom-ng run build/examples/sdl_demo
```

For a single check:

```sh
ctest --test-dir build -R unit --output-on-failure
ctest --test-dir build -R integration --output-on-failure
```

All integration artifacts go under `build/test-output/`.

## Student Workspace

Developers and instructors can generate a sample workspace outside this
repository, or use the ignored `student/` directory for a local demonstration:

```sh
lcom-ng setup student
make -C student
lcom-ng test timer --project student
```

`lcom-ng setup` does not overwrite existing starter files unless passed `--force`.
See the [student guide](student-guide.md) for the generated layout and
[the lab guide](../course/README.md) for the course progression.

## Troubleshooting

- `lcom-ng: command not found`: enter `devenv shell`, or use `build/machinelab` directly.
- SDL package/configuration errors: use the dev shell or configure a headless
  build with `-DMACHINE_LAB_WITH_SDL=OFF`.
- A graphical program appears hung under `--headless`: supply an exit script
  and, while debugging, a `--max-ticks` guard.
- MP4 rendering fails: confirm `ffmpeg` is on `PATH`; frame dumps and replay
  execution do not otherwise require it.
- Fresh generated labs fail: this is expected; their TODO stubs are compilable
  starting points, not solutions.
