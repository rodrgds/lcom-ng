# Examples

Build once before running examples:

```sh
devenv shell
lcom-ng-build
```

Use the focused examples in the order below when explaining the platform. They
progress from the client/runtime boundary to interrupts, input, mapped memory,
audio, and serial communication.

## Focused Examples

| Program | Demonstrates | Canonical command |
| --- | --- | --- |
| `hello` | initialization, runtime console, process boundary | `lcom-ng run --headless -- build/examples/hello` |
| `timer_int` | IRQ0 subscription and event waits | `lcom-ng run --headless -- build/examples/timer_int 3` |
| `rtc_date` | CMOS ports, UIP, BCD conversion, fixed RTC | `lcom-ng run --headless --rtc 2026-06-16T12:34:56 -- build/examples/rtc_date` |
| `keyboard_scan` | IRQ1 and make/break scancodes | `lcom-ng run --headless --script scripts/type_a_esc.mlabscript -- build/examples/keyboard_scan` |
| `mouse_packet` | IRQ12, PS/2 enable/ACK, signed packet deltas | `lcom-ng run --headless --script scripts/mouse_move.mlabscript -- build/examples/mouse_packet` |
| `vbe_rectangle` | VBE mode, framebuffer map, pitch, present | `lcom-ng run --headless --dump-frame build/rectangle.ppm -- build/examples/vbe_rectangle` |
| `audio_tone` | PCM mapping, synthesis, AC97-lite playback | `lcom-ng run --headless --audio-wav build/tone.wav -- build/examples/audio_tone` |
| `uart_loopback` | one UART's MCR loopback and RX interrupt | `lcom-ng run --headless -- build/examples/uart_loopback` |
| `uart_pair` | COM1-to-COM2 cable inside one machine | `lcom-ng run --headless -- build/examples/uart_pair` |
| `uart_peer_sender` + `receiver` | cable between two runtime processes | `lcom-ng run-pair --headless build/examples/uart_peer_sender --right build/examples/uart_peer_receiver --max-ticks 800` |
| `sdl_demo` | PIT-driven VBE animation and live keyboard input | `lcom-ng run build/examples/sdl_demo` |

`uart_pair` and `run-pair` are deliberately different. The former tests two
ports owned by one guest machine; the latter creates two independent machines
and bridges matching ports between them.

## Showcase Studios

These seven finished programs combine several labs into project-scale tools,
monitors, and ambient applications. They are intentionally deeper than the
focused examples: each has a complete UI, deterministic replay path, final
console summary, and visible device behavior.

Regenerate all nine canonical frames with
`devenv shell -- sh scripts/capture_showcase_examples.sh`.

| Program | What it demonstrates | Controls |
| --- | --- | --- |
| `pixel_studio` | 16×16 sprite editing, palette tools, continuous drag strokes, 32-state history, zoom, RTC-stamped C export | Mouse draws and selects; right-click picks; `P/E/F/I` tools; `1/2/3` or `-/+` zoom; `Ctrl-Z/Y`; `C` clear; `G` grid; `S/F2` export; `F1/H` help |
| `serial_whiteboard` | two-window UART drawing, shared cursors, CRC8 frames, sequence/ACK/NACK, retries, HELLO reconnect, bounded queues, snapshot resync | Mouse drag draws; palette or `1-5` selects ink; `C` clears both peers; `R` rotates the local session; `H` help; optional role argument `restart` exercises a same-token transport reset |
| `pomodoro_clock` | RTC wall-clock snapshots beside a PIT-backed monotonic focus timer, mouse controls, completion tones | `Space` start/pause; `R` reset; `Tab` switch focus/break; `D` 5s/3s demo; `M` mute; or click the visible buttons |
| `interrupt_scope` | live IRQ rates, history graph, scancode and PS/2 bytes, UART loopback traffic, RTC IRQ/poll status, recent events | Generate keyboard/mouse activity; `Space` freezes the rate view; `C` clears; `L` toggles UART loopback |
| `tilemap_editor` | layered tile/object painting, gesture history, validation, RTC-stamped `.mlmap` export, isolated Sokoban playtest | Edit: drag, right-click pick, `1-5` brushes, `Ctrl-Z/Y`, `G`, `V`, `S/F2`; `Enter/M` playtest. Play: arrows/WASD, `U` undo, `R` reset |
| `serial_transfer` | paced 8-byte blocks in 16-byte frames, manifest, CRC8/CRC32, stop-and-wait ACK/NACK, timeout and retransmission | Automatic after `send`/`receive` role selection; deterministic role arguments exercise block loss, DONE-ACK loss, CRC rejection, and peer timeout |
| `ambient_aquarium` | RTC-driven dawn/day/sunset/night lighting, PIT motion, mouse feeding, bubbles, and AC97 event tones | Click or `F` feeds; `L` cycles lighting; `P/Space` pauses motion; `B` bubbles; `S` sound; `R` reset |

`Esc` exits every interactive studio except `serial_transfer`, which exits both
peers after the receiver verifies the final CRC32.

Run the single-window studios:

```sh
lcom-ng run -- build/examples/pixel_studio build/pixel_studio_sprite.h
lcom-ng run --rtc 2026-07-30T09:30:00 -- build/examples/pomodoro_clock
lcom-ng run --rtc 2026-07-30T09:30:00 -- build/examples/interrupt_scope
lcom-ng run -- build/examples/tilemap_editor build/tilemap_demo.mlmap
lcom-ng run --rtc 2026-07-30T18:30:00 -- build/examples/ambient_aquarium
```

Run the paired studios:

```sh
lcom-ng run-pair build/examples/serial_whiteboard left \
  --right build/examples/serial_whiteboard right

lcom-ng run-pair build/examples/serial_transfer send \
  --right build/examples/serial_transfer receive

# The sender deliberately drops one block, then recovers by timeout/retry.
lcom-ng run-pair --headless --max-ticks 1400 \
  build/examples/serial_transfer send fault \
  --right build/examples/serial_transfer receive
```

Capture the authored single-window demonstrations:

```sh
lcom-ng run --headless --script scripts/pixel_studio_demo.mlabscript \
  --dump-frame build/pixel-studio.ppm --max-ticks 900 -- \
  build/examples/pixel_studio build/pixel_studio_sprite.h

lcom-ng run --headless --rtc 2026-07-30T09:40:00 \
  --script scripts/pomodoro_clock_demo.mlabscript \
  --audio-wav build/pomodoro-clock.wav \
  --dump-frame build/pomodoro-clock.ppm --max-ticks 500 -- \
  build/examples/pomodoro_clock

lcom-ng run --headless --rtc 2026-07-30T09:40:00 \
  --script scripts/interrupt_scope_demo.mlabscript \
  --dump-frame build/interrupt-scope.ppm --max-ticks 360 -- \
  build/examples/interrupt_scope

lcom-ng run --headless --script scripts/tilemap_editor_demo.mlabscript \
  --dump-frame build/tilemap-editor.ppm --max-ticks 700 -- \
  build/examples/tilemap_editor build/tilemap_demo.mlmap

lcom-ng run --headless --rtc 2026-07-30T08:15:00 \
  --script scripts/ambient_aquarium_demo.mlabscript \
  --audio-wav build/ambient-aquarium.wav \
  --dump-frame build/ambient-aquarium.ppm --max-ticks 520 -- \
  build/examples/ambient_aquarium
```

Capture the paired whiteboard and both transfer roles:

```sh
lcom-ng run-pair --headless \
  --left-script scripts/serial_whiteboard_left.mlabscript \
  --right-script scripts/serial_whiteboard_right.mlabscript \
  --left-dump-frame build/serial-whiteboard-left.ppm \
  --right-dump-frame build/serial-whiteboard-right.ppm \
  --max-ticks 1000 \
  build/examples/serial_whiteboard left \
  --right build/examples/serial_whiteboard right

lcom-ng run-pair --headless \
  --left-dump-frame build/serial-transfer-sender.ppm \
  --right-dump-frame build/serial-transfer-receiver.ppm \
  --max-ticks 1200 \
  build/examples/serial_transfer send \
  --right build/examples/serial_transfer receive
```

The `fault` argument belongs to this transfer example. Runtime-level UART
latency, loss, and framing-error injection remain separate roadmap work.
`receive dropack` proves that a lost final ACK is recovered; `receive corrupt`
and sender modes `badcrc` / `stall` terminate both peers with a coordinated,
machine-readable CRC or timeout reason.

## Complete Programs

### Project Seeds

These are small complete apps intended as final-project starting points. Each
one keeps the device-facing pieces visible: IRQ loops, scancode handling,
framebuffer drawing, and device-buffer output.

| Program | Demonstrates | Canonical command |
| --- | --- | --- |
| `word_processor` | cursor editing, wrap/scroll, insert/delete, document stats, VBE UI | `lcom-ng run --headless --script scripts/write_note.mlabscript --dump-frame build/write.ppm -- build/examples/word_processor` |
| `music_maker` | 3-track tracker, playhead, tempo, mixer bars, AC97 PCM synthesis | `lcom-ng run --headless --script scripts/music_maker_demo.mlabscript --audio-wav build/music.wav --dump-frame build/music.ppm -- build/examples/music_maker` |
| `breakout_demo` | timer-driven physics, keyboard state, collision, game HUD | `lcom-ng run --headless --script scripts/breakout_demo.mlabscript --dump-frame build/breakout.ppm -- build/examples/breakout_demo` |

### Flappy Bird

A compact example of timer-driven animation, keyboard input, VBE rendering,
audio, and replay-friendly behavior.

```sh
lcom-ng run build/examples/flappy_bird
lcom-ng replay scripts/flappy_caption_demo.mlabscript --headless \
  --video build/flappy.mp4 -- build/examples/flappy_bird
```

### Ninjix

The port of the previous Minix group project. It exercises a larger state
machine, mouse/keyboard input, framebuffer rendering, audio, profiling hooks,
and a UART multiplayer protocol.

```sh
lcom-ng run build/examples/ninjix
lcom-ng run-pair build/examples/ninjix --right build/examples/ninjix
lcom-ng run-pair --headless build/examples/ninjix \
  --right build/examples/ninjix --max-ticks 900
```

In interactive multiplayer, select Multiplayer in both windows and choose
opposite ATTACK and DEFEND roles. Headless paired mode automatically runs a
short role-specific smoke test.

### Plants vs Zombies

A second larger project port that is useful for showing compatibility with a
different codebase and project layout.

```sh
lcom-ng run build/examples/pvz
lcom-ng run --headless --script scripts/pvz_menu_exit.mlabscript \
  --dump-frame build/pvz.ppm --max-ticks 240 -- build/examples/pvz
```

## Replay Scripts

| Script | Intended program/use |
| --- | --- |
| `demo_exit.mlabscript` | exit `sdl_demo` after a few ticks |
| `type_a_esc.mlabscript` | keyboard and Lab 3 checks |
| `mouse_move.mlabscript` | one deterministic mouse packet |
| `write_note.mlabscript` | scripted text entry for `word_processor` |
| `music_maker_demo.mlabscript` | edit and exit the tracker grid |
| `breakout_demo.mlabscript` | move the paddle and exit `breakout_demo` |
| `flappy_demo.mlabscript` | short integration replay |
| `flappy_caption_demo.mlabscript` | video/caption/capture-cut showcase |
| `flappy_space_stress.mlabscript` | dense input and frame-capture regression |
| `ninjix_menu_exit.mlabscript` | menu/initialization smoke test |
| `ninjix_level1_demo.mlabscript` | authored long-form Ninjix video timeline |
| `pvz_menu_exit.mlabscript` | PvZ startup and exit smoke test |
| `pixel_studio_demo.mlabscript` | draw, fill, undo/redo, export, and exit Pixel Studio |
| `pomodoro_clock_demo.mlabscript` | jump RTC wall time while PIT session time stays monotonic |
| `interrupt_scope_demo.mlabscript` | populate all monitored IRQ and byte streams |
| `tilemap_editor_demo.mlabscript` | edit, validate, serialize, and solve the Sokoban map |
| `ambient_aquarium_demo.mlabscript` | feed fish and tour RTC-driven lighting states |
| `serial_whiteboard_left.mlabscript` | left-side strokes plus explicit HELLO session rotation |
| `serial_whiteboard_right.mlabscript` | right-side strokes for paired whiteboard replay |

Scripts used by automated tests should stay short and stable. Longer authored
video scripts may prioritize presentation timing; verify them visually when
game layouts or balance change.
