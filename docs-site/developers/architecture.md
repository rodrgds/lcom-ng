# Architecture

Desktop lcom-ng runs student C code as a normal process, while the host runtime
owns the virtual devices.

```text
student C process
  libmachinelab client
        | local protocol
        v
  lcom-ng runtime server
        |
        +-- bus and IRQ controller
        +-- virtual devices
        +-- headless or SDL/audio backend
```

That runtime/device boundary is deliberate:

- student code cannot accidentally depend on SDL;
- the same binary can run interactively or headlessly;
- device behavior can be traced and tested;
- crashes stay isolated at the process boundary;
- host backends can evolve without changing lab code.

## Student Surface

`sdk/include/lcom/lcom.h` is copied into generated workspaces as
`include/lcom/lcom.h`.

It exposes:

- initialization and console output;
- 8/16/32-bit port I/O;
- IRQ subscribe/unsubscribe and event waits;
- virtual physical memory map/unmap;
- VBE mode information, mode changes, and frame presentation.

It does not expose:

- SDL types;
- C++ runtime classes;
- Minix IPC structures;
- LCF callbacks;
- host pointers.

## Virtual Devices

| Device | Student-visible concepts | Host outputs |
| --- | --- | --- |
| i8254 PIT | control words, divisors, read-back, IRQ0 | virtual ticks |
| i8042 | status/data ports, keyboard IRQ1, mouse IRQ12 | live/scripted input |
| RTC/CMOS | register selection, UIP, BCD/binary fields | live or fixed time |
| VBE | mode info, framebuffer mapping, pitch, present | SDL window or PPM |
| AC97-lite | PCM mapping, rate, byte count, controls | SDL, null, or WAV |
| UART16550 | DLAB, LCR/FCR/IER/LSR/MCR, IRQs | loopback/local/paired cable |

These are educational device models, not cycle-accurate emulations.

## Determinism

Headless execution uses a virtual clock. Scripts inject input at virtual
timestamps. RTC overrides, traces, screenshots, frame sequences, WAV files, and
replay videos come from the same run.

SDL execution follows host time and live keyboard/mouse input, but the student
program still talks to the same virtual devices.

## Test Layers

1. `unit_tests`: bus, controller, device, script, and core behavior.
2. `labN_solution`: reference lab implementations through the runtime.
3. `tests/integration.sh`: CLI, examples, artifacts, pairing, workspace setup,
   replay encoding, bundles, and docs-site contracts.

```sh
devenv shell -- lcom-ng-test
```

## Embedded app exports

Browser and native Android exports link student code and the shared device core
inside one process. `runtime/embedded/Client.cpp` implements the C SDK;
`runtime/embedded/Host.hpp` separates time, input, rendering, audio and clipboard
from device behavior. Browser and SDL hosts implement that interface.

Android uses the NDK and SDL3, with a small Java activity for lifecycle and asset
preparation. The desktop process-isolated path remains available. Embedded games
share their process with the runtime, so student crashes terminate the app.

Shared source discovery lives in `sdk/export/`; Android packaging lives in
`sdk/android/`. See [Android export](../students/android) for building and limits,
and `docs/embedded-runtime.md` for the host execution contract and extension points.

Optional [`lcom.json`](../students/project-config) is resolved by
`sdk/export/config.py` for web, Android and desktop bundle identity. Exported
profiles are self-contained; normal desktop runtime input stays unchanged.
