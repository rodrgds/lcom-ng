# Showcase Studios

The device studios show how the device labs combine into useful tools,
observable protocols, and calm ambient software. Every studio uses the public C
API, renders at 800×600, and has a deterministic path for teaching, screenshots,
and regression evidence.

| Studio | Main systems | Project infrastructure to reuse |
| --- | --- | --- |
| Pixel Studio | mouse, keyboard, PIT, RTC, VBE | tools, drag gestures, palettes, history, export |
| Paired Serial Whiteboard | mouse, keyboard, PIT, VBE, UART | framing, sequencing, reconnect, resync |
| Desk Clock / Pomodoro | RTC, PIT, keyboard, mouse, VBE, audio | wall time vs monotonic time, modes, signals |
| Interrupt Scope | PIT, RTC, keyboard, mouse, UART, VBE | rate windows, byte histories, event logging |
| Tile Forge | mouse, keyboard, PIT, RTC, VBE | layered maps, validation, serialization, playtest |
| Reliable Serial Transfer | PIT, UART, VBE | manifests, block transfer, checksum, retry |
| Ambient Aquarium | RTC, PIT, mouse, keyboard, VBE, audio | time-aware behavior, deterministic animation |

The editors use Command on macOS and Ctrl on other hosts. `Cmd/Ctrl-W`
closes an interactive studio. `Esc` closes help before exiting. Help blocks
keyboard and mouse edits beneath it.

All nine frames on this page come from
`devenv shell -- sh scripts/capture_showcase_examples.sh`.

## Pixel Studio

[Open the complete Pixel Studio guide and gallery](./pixel-studio).

<img src="/assets/screenshots/pixel-studio.png" alt="Pixel Studio showing a 16 by 16 sprite canvas, tool rail, palette, preview, zoom controls, and export status">

Pixel Studio turns PS/2 packets into a complete editing workflow. Continuous
drag interpolation avoids gaps, each gesture becomes one undo state, and export
produces a reusable RTC-stamped C header.

- **Tools:** pencil, eraser, flood fill, eyedropper, 16-color palette.
- **Editing:** 32-state undo/redo, three zoom levels, optional grid, live preview.
- **Controls:** mouse draw/select; right-click pick; `B/P/E/F/I`; `1/2/3` or
  `-/+`; `Cmd/Ctrl-Z` undo, `Cmd/Ctrl-Shift-Z` redo (`Ctrl-Y` also works); `C`; `G`; `S/F2` or `Cmd/Ctrl-S`; `F1/H`; `Esc`.
- **Artifact:** `build/pixel_studio_sprite.h` by default, or the first program
  argument.

```sh
lcom-ng run -- build/examples/pixel_studio \
  build/pixel_studio_sprite.h
```

The authored replay paints, fills, undoes, redoes, exports, and exits:

```sh
lcom-ng run --headless --script scripts/pixel_studio_demo.mlabscript \
  --dump-frame build/pixel-studio.ppm --max-ticks 900 -- \
  build/examples/pixel_studio build/pixel_studio_sprite.h
```

## Paired Serial Whiteboard

[Open the complete guide and gallery](./serial-whiteboard).

<div class="screenshot-grid">
  <figure>
    <img src="/assets/screenshots/serial-whiteboard-left.png" alt="Left Serial Whiteboard peer showing local and remote strokes, shared cursors, palette, and protocol telemetry">
    <figcaption>Left peer: draw locally while watching acknowledgements, queue depth, and resync state.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/serial-whiteboard-right.png" alt="Right Serial Whiteboard peer showing the synchronized drawing and remote cursor">
    <figcaption>Right peer: the same canvas reconstructed from bounded framed UART messages.</figcaption>
  </figure>
</div>

Two independent guests share strokes and cursor positions over COM1. Frames
stay at or below the UART's 16-byte FIFO, while the UI makes transport state
visible instead of hiding it behind a networking library.

- **Protocol:** CRC8, sequence numbers, ACK/NACK, timeout/retry, duplicate and
  out-of-order handling.
- **Recovery:** HELLO sessions, bounded queues and stroke histories, periodic
  incremental snapshots, atomic resync, and same-token transport-reset
  recognition.
- **Controls:** drag to draw; click the palette or press `1-5`; `C` clears both
  peers; `R` rotates the local session; `F1/H` help; `Esc` exit.

```sh
lcom-ng run-pair build/examples/serial_whiteboard left \
  --right build/examples/serial_whiteboard right
```

The deterministic pair replay draws from both sides, rotates the left session,
resynchronizes both stroke histories, and exits cleanly:

```sh
lcom-ng run-pair --headless \
  --left-script scripts/serial_whiteboard_left.mlabscript \
  --right-script scripts/serial_whiteboard_right.mlabscript \
  --left-dump-frame build/serial-whiteboard-left.ppm \
  --right-dump-frame build/serial-whiteboard-right.ppm \
  --max-ticks 1000 \
  build/examples/serial_whiteboard left \
  --right build/examples/serial_whiteboard right
```

Pass `restart` after either role to exercise an in-process, same-session-token
transport reset. The peer recognizes the reset and rebuilds synchronized
protocol state without discarding either canvas.

## Focus Clock / Pomodoro

<img src="/assets/screenshots/pomodoro-clock.png" alt="Pomodoro clock showing RTC wall time above a separate PIT-backed focus countdown and session controls">

The wall clock is read from RTC/CMOS. The focus session advances only from PIT
interrupts. Scripted RTC jumps can move the clock forward or backward without
changing elapsed session time.

- **Sessions:** classic 25-minute focus / 5-minute break, or 5-second / 3-second
  demo lengths.
- **Audio:** a generated completion signal, with a mute state and visible
  availability.
- **Controls:** `Space` start/pause; `R` reset; `Tab` switch mode; `D` demo
  lengths; `M` sound; mouse Start/Reset/Switch buttons; `Esc`.

```sh
lcom-ng run --rtc 2026-07-30T09:30:00 -- \
  build/examples/pomodoro_clock
```

```sh
lcom-ng run --headless --rtc 2026-07-30T09:40:00 \
  --script scripts/pomodoro_clock_demo.mlabscript \
  --audio-wav build/pomodoro-clock.wav \
  --dump-frame build/pomodoro-clock.ppm --max-ticks 500 -- \
  build/examples/pomodoro_clock
```

## Interrupt Scope

[Open the complete guide and gallery](./interrupt-scope). / Device Monitor

<img src="/assets/screenshots/interrupt-scope.png" alt="Interrupt Scope dashboard showing per-device IRQ rates, a rate history graph, raw bytes, RTC status, and recent events">

Interrupt Scope is a live instrument panel for the lab devices. It keeps
totals, one-second rate windows, raw byte histories, and a recent-event log in
one framebuffer.

- **Sources:** PIT IRQ0, keyboard IRQ1, mouse IRQ12, UART IRQ4, plus a live
  one-second RTC polling stream.
- **Details:** make/break scancodes, PS/2 packets, UART RX bytes and errors, RTC
  snapshots with separate IRQ8 subscription/event status.
- **Controls:** use the keyboard and mouse to generate traffic; `Space` freezes
  the rate history; `C` clears counters; `L` toggles COM1 loopback; `Esc`.

```sh
lcom-ng run --rtc 2026-07-30T08:40:00 -- \
  build/examples/interrupt_scope
```

```sh
lcom-ng run --headless --rtc 2026-07-30T09:40:00 \
  --script scripts/interrupt_scope_demo.mlabscript \
  --dump-frame build/interrupt-scope.ppm --max-ticks 360 -- \
  build/examples/interrupt_scope
```

## Tile Forge

[Open the complete guide and gallery](./tilemap-editor). / Level Editor

<img src="/assets/screenshots/tilemap-editor.png" alt="Tile Forge showing a Sokoban map canvas, tile palette, validation panel, minimap, and editor controls">

Tile Forge combines a layered 10×8 map editor with a tiny Sokoban player. The
playtest uses an isolated copy, so moving crates never damages the authored map.

- **Editor:** continuous painting, right-click pick, gesture-level undo/redo,
  grid, minimap, validation for player/crate/goal invariants.
- **Playtest:** arrows or WASD, move undo, reset, solved state, move/push counts.
- **Controls:** `1-5` brushes; `Cmd/Ctrl-Z` undo, `Cmd/Ctrl-Shift-Z` redo (`Ctrl-Y` also works); `G`; `V`; `S/F2` or `Cmd/Ctrl-S`; `Enter/M`; `F1/H`;
  then arrows/WASD, `U`, `R`; `Esc`.
- **Artifact:** RTC-stamped `.mlmap` text with a portable 10×8 glyph grid and
  validation result.

```sh
lcom-ng run -- build/examples/tilemap_editor build/tilemap_demo.mlmap
```

```sh
lcom-ng run --headless --script scripts/tilemap_editor_demo.mlabscript \
  --dump-frame build/tilemap-editor.ppm --max-ticks 700 -- \
  build/examples/tilemap_editor build/tilemap_demo.mlmap
```

## Reliable Serial Transfer

[Open the complete guide and gallery](./serial-transfer).

<div class="screenshot-grid">
  <figure>
    <img src="/assets/screenshots/serial-transfer-sender.png" alt="Reliable Serial Transfer sender showing the source sprite, block ledger, progress, sequence, and retry telemetry">
    <figcaption>Sender: manifest, paced blocks, in-flight sequence, timeout, and retry budget.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/serial-transfer-receiver.png" alt="Reliable Serial Transfer receiver reconstructing the sprite with matching CRC32">
    <figcaption>Receiver: progressive reconstruction and final whole-file CRC32 verification.</figcaption>
  </figure>
</div>

The sender transfers a built-in 16×16 indexed sprite in 8-byte blocks. Every
frame fits the 16-byte FIFO, carries CRC8, and participates in stop-and-wait
delivery; the completed image must also match its advertised CRC32.

- **Flow:** HELLO, manifest, 32 data blocks, final verification.
- **Recovery:** ACK/NACK, duplicate rejection, timeout, bounded retry budget,
  final-ACK recovery, and coordinated terminal failures.
- **Completion:** both roles print bytes, blocks, checksums, and retry totals,
  then exit automatically.

```sh
lcom-ng run-pair build/examples/serial_transfer send \
  --right build/examples/serial_transfer receive
```

Add the sender's local `fault` argument to suppress one block on its first
attempt. The timeout retransmits it and the final CRC32 still matches:

```sh
lcom-ng run-pair --headless --max-ticks 1400 \
  build/examples/serial_transfer send fault \
  --right build/examples/serial_transfer receive
```

This demonstrates application-level recovery. General runtime UART latency,
loss, and framing-error injection remain near-term work.

For failure-path teaching, `receive dropack` recovers a lost final ACK;
`receive corrupt` and sender modes `badcrc` / `stall` make both peers exit with
the same machine-readable CRC or peer-timeout reason.

## Ambient Aquarium

[Open the complete guide and gallery](./ambient-aquarium).

<img src="/assets/screenshots/ambient-aquarium.png" alt="Ambient Aquarium showing fish, bubbles, food, a manual daylight override, PIT motion state, and controls">

Ambient Aquarium is deliberately not a score-driven game. RTC wall time selects
dawn, day, sunset, or night lighting; PIT ticks keep fish, food, and bubbles
moving;
mouse interaction and generated tones turn it into a small desktop habitat.

- **Behavior:** nine fish, feeding attraction, bubbles, deterministic motion,
  RTC-following or manually overridden lighting.
- **Audio:** distinct water chimes for feeding and lighting changes.
- **Controls:** click or `F` feed; `L` lighting; `P/Space` pause; `B` bubbles;
  `M/S` sound; `R` reset; `Esc`.

```sh
lcom-ng run --rtc 2026-07-30T18:30:00 -- \
  build/examples/ambient_aquarium
```

```sh
lcom-ng run --headless --rtc 2026-07-30T08:15:00 \
  --script scripts/ambient_aquarium_demo.mlabscript \
  --audio-wav build/ambient-aquarium.wav \
  --dump-frame build/ambient-aquarium.ppm --max-ticks 520 -- \
  build/examples/ambient_aquarium
```

## Where To Go Next

- Start with [the focused examples](/examples/) when learning one device.
- Use these studios as reusable final-project infrastructure.
- Study [Ninjix](/examples/ninjix) when you need a larger multi-module port.
- Follow the [student lab track](/labs/) to implement the device layer first.
