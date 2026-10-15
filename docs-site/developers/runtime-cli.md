# Runtime And CLI

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
| `lcom-ng bundle` | package a runtime and program for this platform |
| `lcom-ng docs cli` | print a Markdown CLI reference |
| `lcom-ng completion` | emit shell completion |

Use `lcom-ng <command> --help` for the exact option list.

## Interactive And Headless

```sh
lcom-ng run build/examples/flappy_bird
lcom-ng run --headless --max-ticks 300 -- build/examples/timer_int 3
```

Headless runs are deterministic and suitable for tests. SDL runs are for live
teaching, demos, and student exploration.

Use `--max-ticks` to bound virtual time and `--timeout 30` to stop a run after
30 host seconds, including busy loops. The host deadline is disabled by default.
It stops and reaps the direct student processes, preserves diagnostic artifacts,
and exits with code 124. `run`, `replay`, and `run-pair` accept positive finite
seconds, including fractions.

Useful artifacts:

```sh
lcom-ng run --headless --trace build/run.jsonl -- build/examples/timer_int 3
lcom-ng run --headless --dump-frame build/frame.ppm -- build/examples/vbe_rectangle
lcom-ng run --headless --audio-wav build/audio.wav -- build/examples/audio_tone
```

## Replay Scripts

Replay scripts are line-oriented timelines:

```text
at 1 key A down
at 2 key A up
at 3s move 160 -40 during 2s
at 8s rtc 2026-06-16T12:00:00
at 9s caption top 3s REPRODUCIBLE INPUT
```

They are useful for:

- deterministic demos;
- regression tests;
- project review;
- recorded videos;
- grading artifacts.

## Paired Runtimes

```sh
lcom-ng run-pair build/examples/ninjix --right build/examples/ninjix
```

The left and right programs have separate machines, processes, input streams,
display state, and IRQ state. The runtime bridges matching serial ports:
left COM1 to right COM1 and left COM2 to right COM2. Use
`--left-dump-frame` and `--right-dump-frame` to keep both final PPM frames from
a headless paired run.

This replaces the old two-VM/null-modem setup with one reproducible command.

## Bundles

```sh
lcom-ng bundle . --program build/examples/flappy_bird --name flappy-bird
./dist/flappy-bird.mlab
```

The default bundle is executable on the current host platform. Bundling packages
existing binaries; it is not cross-compilation.

### Complete-frame screenshots

`run`, `replay`, and `run-pair` accept `--dump-presented` alongside their frame
dump options. This captures the last completed `lcom_vbe_present()` call,
excluding guest writes made after it. It is useful when a finite tick guard
stops an animation during its next repaint. Without this flag, frame dumps
retain the current shared-memory contents for debugging unfinished rendering.
