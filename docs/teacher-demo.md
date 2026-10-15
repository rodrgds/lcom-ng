# LCOM teacher demo

Run from the repository root. For a short demo, show sections 1, 2, 4 and 6.

## Prepare before the meeting

```sh
devenv shell
cmake -S . -B build -G Ninja -DMACHINE_LAB_WITH_SDL=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
mkdir -p build/teacher-demo
```

Keep this shell open for every command below. `lcom` is the supported compatibility
command for lcom-ng. Student code uses the C device API, not SDL directly.

In interactive windows, `Esc` exits, `F3` toggles the runtime debug overlay and
`F11` toggles fullscreen. Right Ctrl releases the captured mouse; `F3` also releases it by opening the
debug overlay. Command remains available for app shortcuts on macOS. You may
need `Fn` for function keys on a laptop.

## 1. Show the student workflow

Use a fresh directory so the lab still contains its TODOs.

```sh
lcom lab list
lcom lab show timer
lcom setup build/teacher-demo/student
make -C build/teacher-demo/student
lcom test timer --project build/teacher-demo/student
```

The build succeeds. The test intentionally fails until the student implements
the requested functions. Open
`build/teacher-demo/student/labs/timer/timer_lab.c` to show the work they do.

## 2. See interrupts and device traffic live

```sh
lcom run build/examples/interrupt_scope
```

Type and move the mouse to populate scancodes, PS/2 bytes and IRQ graphs.
Press `L` to toggle UART loopback, `Space` to freeze the graph, then `F3` for
the runtime overlay. Explain that the program reads ports and handles IRQs.

## 3. Show graphics, input and audio

Run these one at a time, exiting each before starting the next.

| Command                                  | What to do                                                                                       |
| ---------------------------------------- | ------------------------------------------------------------------------------------------------ |
| `lcom run build/examples/word_processor` | Type a sentence, move with arrows, use Backspace and `Ctrl-Z`.                                   |
| `lcom run build/examples/music_maker`    | `P` plays/pauses, arrows move, `Space` toggles a note, `Tab` changes track, `-/+` changes tempo. |
| `lcom run build/examples/flappy_bird`    | `Space` starts and flaps. Show the timer-driven game, graphics and sound.                        |

## 4. Connect two programs over UART

```sh
lcom run-pair build/examples/serial_whiteboard left \
  --right build/examples/serial_whiteboard right
```

Draw in each window and watch strokes appear in the other. `1-5` changes ink,
`C` clears both boards and `R` restarts the local session. Exit both windows.
Each program has its own machine; matching serial ports connect the two.

For a terminal-only proof, run:

```sh
lcom run-pair --headless build/examples/uart_peer_sender \
  --right build/examples/uart_peer_receiver --max-ticks 800
```

## 5. Show a ported LCOM project

```sh
lcom run build/examples/ninjix
# Optional multiplayer, after closing the single-player window:
lcom run-pair build/examples/ninjix --right build/examples/ninjix
```

For multiplayer, select Multiplayer in both windows, then opposite ATTACK and
DEFEND roles. This is the previous Minix group project running on the host.

## 6. Replay input and export a video

```sh
lcom replay scripts/flappy_caption_demo.mlabscript --headless \
  --video build/teacher-demo/flappy.mp4 --video-fps 30 --max-ticks 600 -- \
  build/examples/flappy_bird
open build/teacher-demo/flappy.mp4
```

`open` is for macOS. The script supplies input and captions; the runtime exports
an MP4 without manual play. Show `scripts/flappy_caption_demo.mlabscript` beside
the result. The same approach supports repeatable checks and grading artifacts.

## Extra commands if there is time

```sh
# Timer IRQs plus a device trace.
lcom run --headless --trace build/teacher-demo/timer.jsonl --max-ticks 240 -- \
  build/examples/timer_int 3
head -n 5 build/teacher-demo/timer.jsonl

# Fixed RTC, keyboard scancodes and raw mouse packets.
lcom run --headless --rtc 2026-10-13T12:34:56 -- build/examples/rtc_date
lcom run --headless --script scripts/type_a_esc.mlabscript --max-ticks 120 -- \
  build/examples/keyboard_scan
lcom run --headless --script scripts/mouse_move.mlabscript --max-ticks 120 -- \
  build/examples/mouse_packet

# Export the music demo as a WAV, then listen.
lcom run --headless --script scripts/music_maker_demo.mlabscript \
  --audio-wav build/teacher-demo/music.wav --max-ticks 900 -- build/examples/music_maker
open build/teacher-demo/music.wav

# Package a standalone app for this host OS.
lcom bundle . --program build/examples/flappy_bird --name flappy-bird \
  --output build/teacher-demo/flappy-bird.mlab
./build/teacher-demo/flappy-bird.mlab
```
