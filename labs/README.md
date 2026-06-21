# lcom-ng Labs

These labs are the student-facing `lcom-ng` track. They keep the valuable parts
of the old LCOM progression, but replace Minix/LCF-specific service boilerplate
with normal C programs that run through `lcom run`.

The historical order is still useful when talking about the course, but the
generated student workspace uses device-named folders:

| order | generated folder | topic | predefined command |
| --- | --- | --- | --- |
| `lab1` | `rtc/` | bitwise helpers and RTC/CMOS reads | `lcom test rtc` |
| `lab2` | `timer/` | i8254 PIT and IRQ0 timer events | `lcom test timer` |
| `lab3` | `kbd/` | i8042 keyboard and Set 1 scancodes | `lcom test kbd` |
| `lab4` | `mouse/` | PS/2 mouse command path and packets | `lcom test mouse` |
| `lab5` | `graphics/` | VBE framebuffer graphics and XPM sprites | `lcom test graphics` |
| `lab6` | `audio/` | AC97-lite PCM audio | `lcom test audio` |
| `lab7` | `uart/` | 16550 UART serial ports | `lcom test uart` |

## Repository Layout

- `labs/docs/` contains the lab handout drafts.
- `labs/templates/labN/include/` contains the tested function contracts copied
  into `student/labs/<device>/include/`.
- `labs/solutions/labN/` contains reference implementations written only with
  the public student API: port I/O, IRQ subscriptions, event waits, framebuffer
  mapping, AC97 buffer mapping, and UART register access.
- `labs/function-requests.json` is the structured function/request index used
  by docs and planning.
- `tests/labs/` contains the predefined tests copied into `.lcom/tests/labs/`
  by `lcom setup`.

## Student Workspace Shape

Generate a fresh workspace with:

```sh
lcom setup student
```

The generated project contains:

- `include/lcom/`: the public lcom-ng runtime/device headers.
- `labs/<device>/include/`: the tested lab API for that device area.
- `labs/<device>/*_lab.c`: TODO starter implementations that the predefined
  tests call directly.
- `lib/<device>/`: optional helper headers students can extend freely without
  hiding the tested entry points.
- `proj/`: a tiny app linked with the lab objects, ready to become a course
  project.
- `Makefile`: a plain Makefile so students do not need CMake in their own
  workspace.

Students can build the starter app and run tests with:

```sh
cd student
make
lcom test rtc
lcom test uart
```

Fresh TODO stubs are expected to compile and fail their checks until implemented.

## Learning Coverage

- `rtc/` keeps bit masks, byte extraction, BCD conversion, and CMOS port reads.
- `timer/` keeps PIT control words, divisors, read-back status, IRQ subscription,
  and tick counters.
- `kbd/` keeps i8042 status/data reads, command writes, make/break handling, and
  two-byte scancode assembly.
- `mouse/` keeps PS/2 command sequencing, ACK handling, packet synchronization,
  signed deltas, and button state decoding.
- `graphics/` keeps VBE mode selection, framebuffer mapping, pixel offsets,
  color models, rectangle drawing, XPM parsing, transparency, and present calls.
- `audio/` adds PCM buffer mapping, sample generation, sample-rate registers,
  playback control, and deterministic wav capture.
- `uart/` keeps divisor latch programming, LCR/FCR/IER/LSR register work,
  loopback, RX interrupts, and two-endpoint protocol design.

## What Changed From Minix

- Kept: registers, IRQs, event loops, state machines, modular C, Makefiles.
- Kept: graphics, input, timers, RTC, audio, serial protocols, final projects.
- Removed: Minix startup ceremony, `driver_receive`, `sys_irqsetpolicy`.
- Removed: VM/shared-folder/capture issues and LCF compatibility quirks.
- Added: headless tests, replay scripts, fixed RTC values, traces, videos.
- Added: SDL3 backend, debug overlay, serial pairing, app bundles.
