# Examples

Examples show where the labs lead: small focused programs, project seeds, and
larger ports.

## Focused Examples

| Program | Demonstrates |
| --- | --- |
| `hello` | initialization, runtime console, process boundary |
| `timer_int` | IRQ0 subscription and event waits |
| `rtc_date` | CMOS ports, UIP, BCD conversion, fixed RTC |
| `keyboard_scan` | IRQ1 and make/break scancodes |
| `mouse_packet` | IRQ12, PS/2 enable/ACK, signed packet deltas |
| `vbe_rectangle` | VBE mode, framebuffer map, pitch, present |
| `audio_tone` | PCM mapping, synthesis, AC97-lite playback |
| `uart_loopback` | UART loopback and RX interrupt |
| `uart_peer_sender` + `receiver` | two hosted machines bridged by serial |

## Project Seeds

| Program | Demonstrates |
| --- | --- |
| `word_processor` | editor loop, cursor state, text wrapping, framebuffer UI |
| `music_maker` | tracker grid, tempo, playhead, generated PCM |
| `breakout_demo` | timer physics, keyboard state, collision, HUD |
| `flappy_bird` | compact game loop, audio, replay, capture |
| [Ninjix](/examples/ninjix) | larger port with input, graphics, audio, profiling, UART multiplayer |

## Showcase Studios

[Explore the seven Showcase Studios](/examples/showcase-studios) for complete
project-scale tools and demonstrations: sprite and level editors, paired UART
collaboration and transfer, a Pomodoro clock, an interrupt monitor, and an
RTC-aware aquarium.

Each studio includes a polished 800×600 interface, exact controls, a canonical
command, deterministic evidence, and reusable final-project infrastructure.

## Desktop shortcuts

Use `Cmd-S` on macOS or `Ctrl-S` on other hosts to save in Notes, Pixel Studio,
and Tile Forge. Notes writes and reopens its first file argument, defaulting to
`build/lcom-notes.txt`. Pixel Studio and Tile Forge retain their `S/F2` export
shortcuts. `Cmd/Ctrl-Z` undoes; the drawing editors use `Cmd/Ctrl-Shift-Z` to redo.

Pocket Composer supports mouse note editing and instrument selection. `Space`
starts/pauses playback, `Enter` toggles the selected note, and `F1` opens help.
Breakout uses arrows or `A/D`, `Space` pause, and `R` restart.

Refresh this gallery with `devenv shell -- sh scripts/capture_marketing_examples.sh`.

## Screenshots

These are real captured frames from the included examples. They are meant to
show the range of final-project directions: tools, trackers, arcade games, and
larger ports.

<div class="screenshot-grid">
  <figure>
    <img src="/assets/screenshots/word-processor.png" alt="Word processor example showing a document canvas with typed text and editor status bars">
    <figcaption><code>word_processor</code>: text editing, real file saving, cursor state, and wrapping.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/music-maker.png" alt="Music maker tracker example showing tracks, note grid, play button, and mixer meters">
    <figcaption><code>music_maker</code>: clickable note grid, Space transport, instruments, and PCM output.</figcaption>
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
    <figcaption><a href="/examples/ninjix">Ninjix</a>: larger port with menus, input, graphics, audio hooks, and UART multiplayer.</figcaption>
  </figure>
</div>

## Canonical Commands

```sh
lcom-ng run --headless --script scripts/write_note.mlabscript \
  --dump-frame build/write.ppm -- build/examples/word_processor

lcom-ng run --headless --script scripts/music_maker_demo.mlabscript \
  --audio-wav build/music.wav --dump-frame build/music.ppm -- build/examples/music_maker

lcom-ng run-pair --headless build/examples/uart_peer_sender \
  --right build/examples/uart_peer_receiver --max-ticks 800
```

## Explore each example

Each page includes a complete workflow, controls, and screenshots of real
editing, help, progress and result states.

| Creative tools | Devices in action | Games |
| --- | --- | --- |
| [Notes](./word-processor) | [Focus Clock](./pomodoro-clock) | [Breakout](./breakout) |
| [Pocket Composer](./music-maker) | [Interrupt Scope](./interrupt-scope) | [Flappy Bird](./flappy-bird) |
| [Pixel Studio](./pixel-studio) | [Shared Whiteboard](./serial-whiteboard) | [Ninjix](./ninjix) |
| [Tile Forge](./tilemap-editor) | [Serial Transfer](./serial-transfer) | [Plants vs. Zombies](./plants-vs-zombies) |
| | [Ambient Aquarium](./ambient-aquarium) | |

[Regenerate the galleries](./screenshots) with one command after changing an app.
