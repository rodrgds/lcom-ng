# lcom-ng

`lcom-ng` lets LCOM students write normal C programs on macOS or Linux while
still learning device registers, interrupts, event loops, framebuffers, audio,
and serial protocols. Student code links with `liblcom-ng`; the `lcom` runtime
hosts deterministic virtual hardware through a headless or SDL3 backend.

It replaces the Minix/LCF execution environment, not the systems-programming
content of the course.

## Quick Start

```sh
devenv shell
lcom-test
lcom run build/examples/flappy_bird
```

Inside the dev shell, `lcom` runs `./build/lcom` and builds the CLI first when
needed. `lcom-test` configures the SDL build, compiles everything, and runs the
unit, lab, and integration test suite.

Without `devenv`, install CMake 3.20+, a C/C++ toolchain, Ninja, CLI11, and
optionally SDL3 plus SDL3_ttf, then run:

```sh
cmake -S . -B build -G Ninja -DLCOM_WITH_SDL=OFF
cmake --build build
ctest --test-dir build --output-on-failure
```

## Three Useful Paths

### Show the runtime

```sh
lcom run build/examples/sdl_demo
lcom run build/examples/ninjix
lcom run-pair build/examples/ninjix --right build/examples/ninjix
```

### Show deterministic testing and video output

```sh
lcom run --headless --rtc 2026-06-16T12:34:56 -- build/examples/rtc_date
lcom replay scripts/flappy_caption_demo.lcomscript --headless \
  --video build/flappy-demo.mp4 -- build/examples/flappy_bird
```

### Show the student workflow

```sh
lcom setup student
make -C student
lcom test rtc --project student
```

Fresh lab stubs compile but intentionally fail their predefined checks until a
student implements them.

## What Is Included

- Virtual i8254 PIT, i8042 keyboard/mouse, RTC/CMOS, VBE framebuffer,
  AC97-lite audio, and 16550 UART devices.
- Interactive SDL3 display/input/audio and deterministic headless execution.
- Replay scripts, traces, screenshots, WAV capture, MP4 rendering, and an SDL
  debug overlay.
- Two-runtime serial pairing for multiplayer and protocol testing.
- Seven student labs, generated starter workspaces, predefined tests, and
  complete reference solutions.
- Small device examples plus Flappy Bird, Ninjix, and Plants vs Zombies ports.
- Single-file and directory app bundles for finished projects.

## Documentation

- [Video demo](docs/video-demo.md): the shortest reliable teacher-demo flow.
- [Getting started](docs/getting-started.md): prerequisites, builds, and daily
  commands.
- [Examples](examples/README.md): what every example proves and its exact run
  command.
- [Student labs](labs/README.md): workspace layout, progression, and handouts.
- [Runtime and CLI](docs/runtime-and-cli.md): execution modes, capture, replay,
  bundling, and command discovery.
- [Architecture](docs/architecture.md): runtime boundary and virtual devices.
- [From Minix to lcom-ng](docs/from-minix.md): retained learning outcomes,
  removed ceremony, and new material.
- [Development roadmap](docs/roadmap.md): implemented scope and future work.

Run `lcom --help` or `lcom <command> --help` for the current CLI reference. To
generate its Markdown summary:

```sh
lcom docs cli
```

## Repository Map

| Path | Purpose |
| --- | --- |
| `include/lcom/` | public student-facing headers |
| `liblcom/` | student C library implementation |
| `runtime/` | host runtime, devices, and backends |
| `examples/` | focused examples and complete projects |
| `scripts/` | deterministic input/replay timelines |
| `labs/` | lab contracts, handouts, and reference solutions |
| `tests/` | unit, lab, and end-to-end integration tests |

The public API deliberately contains no SDL or Minix types. Student programs
only see ports, IRQ subscriptions, events, mapped physical memory, VBE, and
AC97 buffer metadata.
