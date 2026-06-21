# Teacher Video Demo

This walkthrough is ordered to tell one story: normal tooling, preserved device
learning, deterministic testing, a complete project, and a student workspace.
Run it once before recording so Nix, the build, and ffmpeg are warm.

## Prepare

From the repository root:

```sh
devenv shell
lcom-test
rm -rf student build/video-demo
mkdir -p build/video-demo
```

Expected result: all nine CTest entries pass. Keep the terminal at a readable
width so commands and test names remain visible.

## 1. Establish the Model

Show these three paths briefly:

```sh
ls include/lcom runtime/devices examples
```

Explain that a student program includes the public C headers, while `lcom run`
hosts virtual devices. SDL is only a host backend; student code does not call
SDL. Open `examples/timer_int.c` as the smallest interrupt-driven illustration.

## 2. Run Focused Device Examples

```sh
lcom run --headless -- build/examples/timer_int 3
lcom run --headless --rtc 2026-06-16T12:34:56 -- build/examples/rtc_date
lcom run --headless --script scripts/type_a_esc.lcomscript -- \
  build/examples/keyboard_scan
lcom run --headless --script scripts/mouse_move.lcomscript -- \
  build/examples/mouse_packet
```

These demonstrate IRQ events, deterministic RTC state, keyboard scancodes, and
raw PS/2 packets without relying on live input.

Produce inspectable graphics and audio artifacts:

```sh
lcom run --headless --dump-frame build/video-demo/rectangle.ppm -- \
  build/examples/vbe_rectangle
lcom run --headless --audio-wav build/video-demo/tone.wav -- \
  build/examples/audio_tone
```

## 3. Show Interactive Runtime Tooling

```sh
lcom run build/examples/sdl_demo
```

In the window:

1. Press F3 to show the debug overlay.
2. Point out scancode/mouse history and runtime metrics.
3. Press F11 if fullscreen helps the recording.
4. Press Escape to exit.

On macOS, Command releases the captured mouse. On other hosts, use Right Ctrl.

## 4. Show a Complete Existing Project

The old Minix group project was ported as Ninjix. Run one client:

```sh
lcom run build/examples/ninjix
```

Then show that the runtime can replace a two-VM/null-modem setup with one
command:

```sh
lcom run-pair build/examples/ninjix --right build/examples/ninjix
```

Choose Multiplayer in both windows, then choose opposite ATTACK and DEFEND
roles. COM1 is bridged to COM1 and COM2 to COM2 across the two hosted programs.

For a fast deterministic proof of the same bridge:

```sh
lcom run-pair --headless build/examples/uart_peer_sender \
  --right build/examples/uart_peer_receiver --max-ticks 400
```

## 5. Render a Reproducible Demo Video

```sh
lcom replay scripts/flappy_caption_demo.lcomscript --headless \
  --video build/video-demo/flappy.mp4 --video-fps 30 -- \
  build/examples/flappy_bird
```

This replay demonstrates authored input, captions, and capture pause/resume
markers. The MP4 is deterministic and can be used as B-roll or grading proof.

## 6. Show the Student Experience

```sh
lcom setup student
make -C student
lcom lab list
lcom lab show timer
lcom test timer --project student
```

The final command should fail because the generated implementation contains
TODOs. Open `student/labs/timer/timer_lab.c`, then compare its small public
contract with the old LCF/Minix ceremony described in
[From Minix to lcom-ng](from-minix.md).

Explain that the predefined test is visible under
`student/.lcom/tests/labs/lab2_test.c`; the workflow is deterministic rather
than dependent on manual VM input.

## 7. Close With Portability

```sh
lcom bundle . --program build/examples/flappy_bird --name flappy-bird
./dist/flappy-bird.lcom
```

The bundle packages the runtime and program for the current host platform. It
does not cross-compile; a bundle for another OS still needs matching binaries.

## Short Version

If the recording must stay under five minutes, use only:

```sh
lcom-test
lcom run build/examples/sdl_demo
lcom run-pair --headless build/examples/uart_peer_sender \
  --right build/examples/uart_peer_receiver --max-ticks 400
lcom replay scripts/flappy_caption_demo.lcomscript --headless \
  --video build/video-demo/flappy.mp4 -- build/examples/flappy_bird
lcom setup student
lcom test timer --project student
```
