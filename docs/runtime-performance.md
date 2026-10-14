# Runtime performance checks

Measured locally on macOS arm64 on 13 October 2026 with the SDL Debug build.
The baseline is a saved copy of the existing working-tree runtime, including
its compatibility changes. It is not the repository's older `HEAD` revision.

## Changes

- The SDL terminal redraws at most 60 times per second. Device requests and
  input pumping still run normally. Every explicit graphics presentation is
  still rendered.
- Visible terminal and debug text reuse textures between frames. Text or font
  changes replace the corresponding texture; color and opacity remain dynamic.
  Unused textures are released after presentation.
- PPM capture writes one RGB row at a time instead of issuing one `fwrite`
  per pixel. A 64 KiB stdio buffer combines rows into larger filesystem writes.
  Caption layout is evaluated per row and only visible glyphs are drawn.
- SDL uploads 32-bit BGRX framebuffers directly. Indexed and 15/16-bit modes
  use the same row decoder as capture, with exact existing color scaling.
- Expanded palette colors are cached at reset and updated on each DAC component
  write. Register reads still return the original six-bit values; pixel masking
  remains dynamic.
- Terminal history uses a deque, so discarding an old line does not move the
  remaining 999 strings. The 1,000-line limit is unchanged.

Runtime changes are confined to `runtime/backends/SdlBackend.cpp` and the
`runtime/devices/Vbe` implementation and header. The guest SDK, protocol, devices, virtual clock, compatibility
importer and example sources are unchanged by this work.

## Measurements

First-pass medians of three runs, before the follow-up work below. The port workload has 25 terminal lines and 2,000 port
reads. The paired result is the slower guest's elapsed time. Capture writes
60 frames to temporary files, excluding FFmpeg encoding.

| Workload | Before | After |
| --- | ---: | ---: |
| Port reads, SDL | 14,673.7 ms | 26.7 ms |
| Port reads, paired SDL | 16,859.3 ms | 19.8 ms |
| Capture, 800×600 | 10.34 ms/frame | 2.53 ms/frame |
| Capture, 1280×1024 | 28.95 ms/frame | 5.55 ms/frame |

These are focused local workloads, not whole-game speedup claims. Headless port
reads were already fast, at 14.5 ms before and 14.9 ms after. No improvement is
attributed to that timing variation.

The original SDL profile placed 2,173 of 2,233 main-thread samples in terminal
rendering. Destroying text textures flushed queued Metal rendering repeatedly.
Retaining textures alone left port polling slow because each request still
presented a window. Limiting terminal redraws removed that dependency.
The capture profile was dominated by `fwrite` and its locking overhead.

Run the maintained benchmark after building:

```sh
devenv shell -- python3 scripts/benchmark_runtime.py --sdl
devenv shell -- python3 scripts/benchmark_runtime.py \
  --runtime /path/to/baseline/machinelab --sdl
```

Omit `--sdl` for headless capture measurements. `--build-dir` selects the SDK
library directory. `--max-port-ms 1000` is an optional local regression check;
it failed with the original runtime and passed after the fix. Timing thresholds
are not part of shared CI because host and renderer speeds vary.

## Follow-up measurements

These compare the completed first pass against the final changes. Both sides of
these CPU probes use optimized builds. Medians of three alternating runs:

| Workload | Before | After | Improvement |
| --- | ---: | ---: | ---: |
| SDL software renderer, 1280×1024 BGRX32 | 2.890 ms | 0.470 ms | 6.1× |
| SDL software renderer, 1024×768 indexed | 2.181 ms | 0.974 ms | 2.2× |
| SDL software renderer, 640×480 RGB555 | 0.793 ms | 0.498 ms | 1.6× |
| SDL software renderer, 1280×1024 RGB565 | 3.367 ms | 1.957 ms | 1.7× |
| Captioned capture to `/dev/null`, 800×600 | 0.915 ms | 0.082 ms | 11.2× |
| Captioned capture to `/dev/null`, 1280×1024 | 2.534 ms | 0.216 ms | 11.7× |

Renderer probes include conversion, upload and drawing to an SDL texture target;
they exclude window presentation pacing. Capture probes isolate encoding and
stdio, excluding filesystem storage and FFmpeg. These are component throughput
measurements, not whole-game FPS claims. Color readback also passed on Metal.

With the final 64 KiB output buffer, the complete 60-frame capture workload,
including IPC, shared framebuffer copying and filesystem writes, improved from
6.84 to 1.89 ms/frame at 800×600 and from 15.34 to 4.25 ms/frame at 1280×1024.
Both medians are 3.6× faster than the completed first pass. Disk timings varied
substantially, although every alternating comparison improved. A 256 KiB buffer
had no reliable advantage, so the smaller buffer was retained.

Discarding old terminal lines improved the isolated 100,000-line console sink
from 71.52 ms to 8.46 ms, 8.5×. The complete 20,000-message software-SDL workload
improved only from 101.02 ms to 85.92 ms, 1.18×, because IPC dominates it. The
container change is three lines and preserves the same bounded history.

Additional experiments were discarded: gathering the host's socket writes
improved a Unix-socket round-trip probe by about 3%; replacing trace stream
formatting improved its probe by 11%; using a four-channel conversion buffer
added only 1.1–1.4× to rendering while complicating the shared decoder. None met
the requested 1.5× threshold for further work.

The maintained SDL test has an optional benchmark mode. Build it with the same
optimization level when comparing revisions:

```sh
devenv shell -- cmake -S . -B build/performance/release -G Ninja \
  -DMACHINE_LAB_WITH_SDL=ON -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release
devenv shell -- cmake --build build/performance/release --target sdl_render_tests
devenv shell -- env SDL_VIDEODRIVER=dummy SDL_RENDER_DRIVER=software \
  build/performance/release/tests/sdl_render_tests --benchmark
```

## Compatibility evidence

- All 54 before/after PPM comparisons were byte-identical across nine video
  modes, with empty, short and clipped captions at both positions.
- Unit coverage checks RGB output at both ends of every row, alternating rows,
  dimensions, exact file length, caption placement, glyph pixels and clipping.
- SDL readback checks all nine modes, mode changes, bitfield rounding, BGRX
  padding opacity, each individual DAC component write and dynamic pixel masks.
  It passes in Debug and Release builds.
- The expanded Minix corpus passed 28 cases and 41 scripted runs across 20
  repositories. Original source hashes remained unchanged.
- All 57 historical-project runs had matching exit outcomes before and after.
  Of 33 frame comparisons, 32 were byte-identical. Tetris seeds its pieces with
  `srand(time(NULL))`; its gameplay frame also differed between two runs of the
  saved baseline. The normal Tetris menu-exit frame matched.
- The original corpus passed eight of nine case verdicts, including both
  supplied archives and the expected rejection of incomplete upstream sources.
  BomberLab's menu exit passed; its gameplay replay raised signal 11 in both
  the saved baseline and updated runtimes, with identical final frame bytes.
- Live SDL inspection covered keyboard input, terminal text updates, graphics,
  debug overlays and the historical Bamboozlers level-selection screen.
  Follow-up inspection also covered Asteroids in 32-bit mode and the Space
  Invaders indexed-color menu and debug overlay at 60 FPS. Both exited normally.
- Of 110 integration image, audio and map artifacts, 109 matched byte-for-byte.
  Ninjix's menu differed only in its live clock text; fixing the RTC input made
  that frame byte-identical too.

All 23 non-integration CTests pass in the current workspace. The full standard
gate had passed all 22 then-existing CTests, docs/assets checks and Remotion
typechecking earlier in the pass. A later full rerun stopped at paired UART
startup following separate, concurrent UART edits. The saved runtime still
passes that same sender/receiver example. The new FIFO-enable behavior discards
a byte received before initial configuration, which can lose the startup
handshake. A standalone guest reproduces the difference without graphics.
These performance changes do not modify UART, paired scheduling or the example.
Final docs/assets checks and Remotion typechecking passed separately.

Local timings, profiles, frame comparisons and corpus results are under
`build/performance/`. These generated diagnostics are ignored by Git.
