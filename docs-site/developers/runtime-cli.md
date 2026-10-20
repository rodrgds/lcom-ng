# Runtime and CLI

`lcom-ng` is the supported entry point for running, testing, recording, and
packaging student programs.

## Commands

| Command | Purpose |
| --- | --- |
| `lcom-ng run` | host one student program |
| `lcom-ng run-pair` | host two programs with bridged UARTs |
| `lcom-ng replay` | run one program with an authored input timeline |
| `lcom-ng setup` | generate a student workspace |
| `lcom-ng test` | compile and run one predefined lab test |
| `lcom-ng lab` | list labs or inspect requested functions |
| `lcom-ng compat build` | build unchanged Minix/LCF sources; see [From Minix](/developers/from-minix) |
| `lcom-ng bundle` | package a runtime and program for this platform |
| `lcom-ng export web` | rebuild C sources as a standalone browser game |
| `lcom-ng export android` | rebuild C sources as a native SDL3 APK |
| `lcom-ng web` | build a project folder and open it in a browser |
| `lcom-ng docs cli` | print a Markdown CLI reference |
| `lcom-ng completion` | emit shell completion |

Use `lcom-ng <command> --help` for the exact option list.

## Interactive and headless

```sh
lcom-ng run build/examples/flappy_bird
lcom-ng run --headless --max-ticks 300 -- build/examples/timer_int 3
```

Headless runs are deterministic and suitable for tests. SDL runs advance virtual
time from host time for live teaching and demos.

`--max-ticks` bounds virtual time. `--timeout 30` stops a run after 30 host
seconds, including busy loops; the host deadline is disabled by default. It stops
and reaps direct student processes, preserves diagnostic artifacts, and exits
with code 124. `run`, `replay`, and `run-pair` accept positive finite seconds,
including fractions.

### Artifacts

```sh
lcom-ng run --headless --trace build/run.jsonl -- build/examples/timer_int 3
lcom-ng run --headless --dump-frame build/frame.ppm -- build/examples/vbe_rectangle
lcom-ng run --headless --audio-wav build/audio.wav -- build/examples/audio_tone
```

### Complete frames

`run`, `replay`, and `run-pair` accept `--dump-presented` with their frame dump
options. It captures the last completed `lcom_vbe_present()` call, excluding
guest writes after it, which is useful when a finite tick guard stops an
animation mid-repaint. Without it, frame dumps retain current shared-memory
contents for debugging unfinished rendering.

## Replay scripts

Replay scripts are line-oriented timelines:

```text
at 1 key A down
at 2 key A up
at 3s move 160 -40 during 2s
at 8s rtc 2026-06-16T12:00:00
at 9s caption top 3s REPRODUCIBLE INPUT
```

Plain timestamps are virtual ticks; `s` and `ms` suffixes are converted at
60 Hz. `move` expands into smooth relative packets.

```sh
lcom-ng replay scripts/flappy_demo.mlabscript --headless \
  --max-ticks 600 -- build/examples/flappy_bird
```

Use them for demos, regression tests, project review, videos, and grading.

## Paired runtimes

```sh
lcom-ng run-pair build/examples/ninjix --right build/examples/ninjix
```

Left and right programs have separate machines, processes, input streams,
display state, and IRQ state. The runtime bridges matching serial ports: left
COM1 to right COM1 and left COM2 to right COM2. Use `--left-script` and
`--right-script` for separate deterministic input, and `--left-dump-frame` and
`--right-dump-frame` to keep both final PPM frames from a headless run.

## Bundles

```sh
lcom-ng bundle . --program build/examples/flappy_bird --name flappy-bird
./dist/flappy-bird.mlab
```

The default bundle is executable on the current host platform. `--format dir`
produces an exploded directory with launchers, manifest, runtime, program, SDK
headers, and static library. Bundling packages existing binaries; it is not
cross-compilation.
