# Architecture

Desktop lcom-ng runs student C code as a normal process, while the host runtime
owns the virtual devices.

```text
student C process
  libmachinelab client (aliases: liblcom, liblowlab)
        | local protocol
        v
  lcom-ng runtime server
        |
        +-- bus and IRQ controller
        +-- virtual devices
        +-- headless or SDL/audio backend
```

The runtime/device boundary keeps student code independent of SDL, allows one binary to
run interactively or headlessly, isolates crashes, and lets host backends evolve
without changing lab code.

## Student surface

`sdk/include/lcom/` is copied into generated workspaces as `include/lcom/`. It
contains `lcom.h` and device headers for the PIT, i8042, RTC, VBE, AC97, UART and
memory map.

`lcom.h` exposes initialization and console output, 8/16/32-bit port I/O, IRQ
subscribe/unsubscribe and event waits, virtual physical memory map/unmap, and VBE
mode information, mode changes, and frame presentation.

It does not expose SDL types, C++ runtime classes, Minix IPC structures, LCF
callbacks, or host pointers.

## Virtual devices

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
replay videos come from the same run. SDL execution follows host time and live
input but uses the same virtual devices.

## Test layers

1. `unit`: bus, controller, device, script, and core behavior.
2. `lab1_solution` through `lab7_solution`: reference lab implementations through
   the runtime.
3. `integration`: CLI, examples, artifacts, pairing, workspace setup, replay
   encoding, bundles, and docs-site contracts.

```sh
devenv shell -- lcom-ng-test
```

Browser and collection regressions run through `devenv shell -- lcom-ng-web-test`,
including real multi-touch keyboard input. Android export checks are separate
from the default gate.

## Embedded app exports

Browser and native Android exports link student code and the shared device core
inside one process. `runtime/embedded/Client.cpp` implements the C SDK;
`runtime/embedded/Host.hpp` separates time, input, rendering, audio and clipboard
from device behavior. Android uses the NDK and SDL3 with a small Java activity.
Desktop keeps the process-isolated path. Embedded crashes terminate the app.

Source discovery lives in `sdk/export/`; Android packaging lives in
`sdk/android/`. See [Android export](../students/android) for building and limits,
and `docs/embedded-runtime.md` for the host contract and extension points.

Optional [`lcom.json`](../students/project-config) is resolved by
`sdk/export/config.py` for web, Android and desktop bundle identity. Exported
profiles are self-contained; normal desktop input stays unchanged.

## Game collections

`sdk/collection/` validates `lcom.collection.json` and builds both collection
launchers. The checked-in Arcade is `collections/arcade/`; the authoring guide is
[Build a game collection](../instructors/game-collections).

- Each game's `lcom.json` owns its title, icon, preview, description, mobile
  controls and optional `instructions`. Instructions are display metadata; they
  do not remap keys or change guest input. See
  [project configuration](../students/project-config#instructions-in-the-browser-arcade).
- Collection-wide `web.json` points to `game-metadata.json` and `gallery.json`.
  Keep academic-year evidence, tags and featured selection in metadata, not in
  game source.
- Pinned imports under `examples/community/` keep original bytes and an
  `UPSTREAM.json` with source hashes, license status and any recorded
  `SOURCE_PATCH.diff` correction. Do not edit original game assets to fit a
  launcher; runtime adaptations belong in the compatibility SDK.

### Maintenance checks

Before publishing, verify pinned sources and gallery evidence:

```sh
python3 scripts/check_collection_sources.py --require-tracked
python3 scripts/capture_arcade_galleries.py --check
```

`scripts/package_arcade_web.py` produces the compressed, hashed site payload.
Publication steps stay in `collections/arcade/README.md`.
