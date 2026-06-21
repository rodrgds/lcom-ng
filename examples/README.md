# Examples

Build once before running examples:

```sh
devenv shell
lcom-build
```

Use the focused examples in the order below when explaining the platform. They
progress from the client/runtime boundary to interrupts, input, mapped memory,
audio, and serial communication.

## Focused Examples

| Program | Demonstrates | Canonical command |
| --- | --- | --- |
| `hello` | initialization, runtime console, process boundary | `lcom run --headless -- build/examples/hello` |
| `timer_int` | IRQ0 subscription and event waits | `lcom run --headless -- build/examples/timer_int 3` |
| `rtc_date` | CMOS ports, UIP, BCD conversion, fixed RTC | `lcom run --headless --rtc 2026-06-16T12:34:56 -- build/examples/rtc_date` |
| `keyboard_scan` | IRQ1 and make/break scancodes | `lcom run --headless --script scripts/type_a_esc.lcomscript -- build/examples/keyboard_scan` |
| `mouse_packet` | IRQ12, PS/2 enable/ACK, signed packet deltas | `lcom run --headless --script scripts/mouse_move.lcomscript -- build/examples/mouse_packet` |
| `vbe_rectangle` | VBE mode, framebuffer map, pitch, present | `lcom run --headless --dump-frame build/rectangle.ppm -- build/examples/vbe_rectangle` |
| `audio_tone` | PCM mapping, synthesis, AC97-lite playback | `lcom run --headless --audio-wav build/tone.wav -- build/examples/audio_tone` |
| `uart_loopback` | one UART's MCR loopback and RX interrupt | `lcom run --headless -- build/examples/uart_loopback` |
| `uart_pair` | COM1-to-COM2 cable inside one machine | `lcom run --headless -- build/examples/uart_pair` |
| `uart_peer_sender` + `receiver` | cable between two runtime processes | `lcom run-pair --headless build/examples/uart_peer_sender --right build/examples/uart_peer_receiver --max-ticks 400` |
| `sdl_demo` | PIT-driven VBE animation and live keyboard input | `lcom run build/examples/sdl_demo` |

`uart_pair` and `run-pair` are deliberately different. The former tests two
ports owned by one guest machine; the latter creates two independent machines
and bridges matching ports between them.

## Complete Programs

### Flappy Bird

A compact example of timer-driven animation, keyboard input, VBE rendering,
audio, and replay-friendly behavior.

```sh
lcom run build/examples/flappy_bird
lcom replay scripts/flappy_caption_demo.lcomscript --headless \
  --video build/flappy.mp4 -- build/examples/flappy_bird
```

### Ninjix

The port of the previous Minix group project. It exercises a larger state
machine, mouse/keyboard input, framebuffer rendering, audio, profiling hooks,
and a UART multiplayer protocol.

```sh
lcom run build/examples/ninjix
lcom run-pair build/examples/ninjix --right build/examples/ninjix
lcom run-pair --headless build/examples/ninjix \
  --right build/examples/ninjix --max-ticks 900
```

In interactive multiplayer, select Multiplayer in both windows and choose
opposite ATTACK and DEFEND roles. Headless paired mode automatically runs a
short role-specific smoke test.

### Plants vs Zombies

A second larger project port that is useful for showing compatibility with a
different codebase and project layout.

```sh
lcom run build/examples/pvz
lcom run --headless --script scripts/pvz_menu_exit.lcomscript \
  --dump-frame build/pvz.ppm --max-ticks 240 -- build/examples/pvz
```

## Replay Scripts

| Script | Intended program/use |
| --- | --- |
| `demo_exit.lcomscript` | exit `sdl_demo` after a few ticks |
| `type_a_esc.lcomscript` | keyboard and Lab 3 checks |
| `mouse_move.lcomscript` | one deterministic mouse packet |
| `flappy_demo.lcomscript` | short integration replay |
| `flappy_caption_demo.lcomscript` | video/caption/capture-cut showcase |
| `flappy_space_stress.lcomscript` | dense input and frame-capture regression |
| `ninjix_menu_exit.lcomscript` | menu/initialization smoke test |
| `ninjix_level1_demo.lcomscript` | authored long-form Ninjix video timeline |
| `pvz_menu_exit.lcomscript` | PvZ startup and exit smoke test |

Scripts used by automated tests should stay short and stable. Longer authored
video scripts may prioritize presentation timing; verify them visually when
game layouts or balance change.
