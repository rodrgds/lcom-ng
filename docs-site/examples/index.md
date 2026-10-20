# Examples

Examples show where the labs lead. Each guide covers running the program, its
controls, and captured states. Build all examples once with
`devenv shell -- lcom-ng-build`.

## Focused examples

| Program | Demonstrates |
| --- | --- |
| `hello` | initialization, console output, clean exit |
| `timer_int` | IRQ0 subscription and event waits |
| `rtc_date` | CMOS ports, UIP, BCD conversion, fixed RTC |
| `keyboard_scan` | IRQ1 and make/break scancodes |
| `mouse_packet` | IRQ12, PS/2 enable/ACK, signed packet deltas |
| `vbe_rectangle` | VBE mode, framebuffer map, pitch, present |
| `audio_tone` | PCM mapping, synthesis, AC97-lite playback |
| `uart_loopback` | UART loopback and RX interrupt |
| `uart_peer_sender` + `uart_peer_receiver` | two hosted machines bridged by serial |

## Project seeds

| Program | Guide | Demonstrates |
| --- | --- | --- |
| `word_processor` | [Notes](./word-processor) | editor loop, cursor state, text wrapping |
| `music_maker` | [Pocket Composer](./music-maker) | tracker grid, tempo, playhead, PCM |
| `breakout_demo` | [Breakout](./breakout) | timer physics, keyboard state, collision |
| `flappy_bird` | [Flappy Bird](./flappy-bird) | compact game loop, replay, capture |
| `ninjix` | [Ninjix](./ninjix) | larger port with input, graphics, UART multiplayer |

## Showcase studios

[Seven Showcase Studios](./showcase-studios) are project-scale tools: a sprite
editor, a level editor, paired UART collaboration and transfer, a focus clock, an
interrupt monitor, and an RTC-aware aquarium. Each has an 800×600 interface, a
canonical command, captured evidence, and a full control table.

## Screenshots

Real captured frames from the included examples:

<div class="screenshot-grid">
  <figure>
    <img src="/assets/screenshots/word-processor.png" alt="Word processor example showing a document canvas with typed text and editor status bars">
    <figcaption><code>word_processor</code>: text editing, file saving, cursor state, and wrapping.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/music-maker.png" alt="Music maker tracker example showing tracks, note grid, play button, and mixer meters">
    <figcaption><code>music_maker</code>: note grid, Space transport, instruments, and PCM output.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/breakout.png" alt="Breakout demo showing paddle, ball, bricks, score, and pause controls">
    <figcaption><code>breakout_demo</code>: timer physics, keyboard state, collision, and HUD.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/flappy.png" alt="Flappy Bird style demo showing side-scrolling pipes and player character">
    <figcaption><code>flappy_bird</code>: compact game loop, replay capture, and audio hooks.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/ninjix.png" alt="Ninjix menu showing play, multiplayer, instructions, credits, and exit options">
    <figcaption><a href="/examples/ninjix">Ninjix</a>: menus, input, graphics, and UART multiplayer.</figcaption>
  </figure>
</div>

Regenerate these overview images with
`devenv shell -- sh scripts/capture_marketing_examples.sh`. Per-app galleries
come from the [screenshot workflow](./screenshots).

## Canonical commands

```sh
lcom-ng run --headless --script scripts/write_note.mlabscript \
  --dump-frame build/write.ppm -- build/examples/word_processor

lcom-ng run --headless --script scripts/music_maker_demo.mlabscript \
  --audio-wav build/music.wav --dump-frame build/music.ppm -- build/examples/music_maker

lcom-ng run-pair --headless build/examples/uart_peer_sender \
  --right build/examples/uart_peer_receiver --max-ticks 800
```

## Explore each example

| Creative tools | Devices in action | Games |
| --- | --- | --- |
| [Notes](./word-processor) | [Focus Clock](./pomodoro-clock) | [Breakout](./breakout) |
| [Pocket Composer](./music-maker) | [Interrupt Scope](./interrupt-scope) | [Flappy Bird](./flappy-bird) |
| [Pixel Studio](./pixel-studio) | [Shared Whiteboard](./serial-whiteboard) | [Ninjix](./ninjix) |
| [Tile Forge](./tilemap-editor) | [Serial Transfer](./serial-transfer) | [Plants vs. Zombies](./plants-vs-zombies) |
| | [Ambient Aquarium](./ambient-aquarium) | |

On macOS use `Cmd` where a shortcut says `Cmd/Ctrl`; use `Ctrl` on other hosts.
`Esc` closes help before leaving an app. `Cmd/Ctrl+W` closes an app. Save, undo
and export shortcuts are listed on each guide.
