# Minix compatibility reference

The compatibility SDK builds existing LCOM C sources with their original imports,
entry points, and device code. The importer reads the Minix Makefile and writes
host artifacts to `.lcom-ng/`.

For installation, see [download and install](/students/install); the
[compatibility quick start](/students/compatibility) covers the normal edit and
run loop. This page covers unusual Makefiles, device behavior, and limits.

## Build and run

In the folder containing the original Makefile:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

The builder prints the generated binary path; its name comes from `PROG`. Original
`.c`, `.h`, imports, and Makefile stay unchanged. Rebuild after editing.

For deterministic input:

```sh
lcom-ng run --headless --max-ticks 600 \
  --script /path/to/input.mlabscript --dump-frame build/project.ppm \
  -- /path/to/labs/proj/.lcom-ng/proj
```

See [Runtime and CLI](/developers/runtime-cli) for `--timeout`, traces, captures,
and replay scripts. Compilation does not prove gameplay, serial communication, or
rendering.

## Lab callbacks

The builder also handles a lab directory. Keep the original function name and
pass its arguments after the binary:

```sh
lcom-ng compat build /path/to/labs/lab2
lcom-ng run -- /path/to/labs/lab2/.lcom-ng/lab2 timer_test_int 2
lcom-ng compat build /path/to/labs/lab5
lcom-ng run -- /path/to/labs/lab5/.lcom-ng/lab5 video_test_xpm pic1 10 20
lcom-ng run -- /path/to/labs/lab5/.lcom-ng/lab5 video_test_move penguin 10 20 50 20 -3 60
```

Timer configuration and interrupts, keyboard scan/poll/timed scan, mouse
packet/async/remote/gesture, and graphics callbacks dispatch to the student's
implementation. Focus the SDL window for live input. Mouse remote mode uses
`READ_DATA` polling; stream reporting starts disabled. The native starter and
`lcom-ng test` checks use the native API.

## Makefile inputs

The importer supports:

- `SRCS`, explicit `.o: .c` rules, `.PATH`, `.PATH.c` and `VPATH`;
- preprocessing and optimization flags;
- local archives declared through `LIB_SRCS`, named `LIB_*_SRCS`, `LIB` with
  `SRCS`, `LIB_OBJS`, or declarative archive object recipes;
- object inputs in `LDADD`, rebuilt from their original C sources and Makefile;
- single-variable BSD `.for` source-directory loops and headers in `SRCS`;
- `$(shell find <relative-dir> -type f -name "*.c")` within the project tree.

It does not execute shell commands, Makefile recipes, or Minix system make
includes.

For archives created by a separate script, provide their sources explicitly:

```sh
lcom-ng compat build /path/to/project \
  --library timer=../libs/timer.c --library video=../libs/video-card.c \
  --include include --output build/imported-project
```

Repeat `--library` per archive. An explicit library is linked even when the
Makefile omitted its dependency; separate multiple sources with commas. Library
sources and extra include directories are relative to the project. Existing Minix
executables and archives must be rebuilt for the host.

The importer records source SHA-256 values, compiler commands and build status in
`.lcom-ng/build.json`. Missing sources and unsupported expressions produce errors.
Absolute `/home/lcom/labs/` library and include paths resolve under the inferred
workspace or an explicit `--asset-root`.

## Diagnose student code

Compiler failures name the source or link stage. A failed compilation removes the
previous executable and records the error in `build.json`. Runtime crashes name
the terminating signal and suggest debugging commands.

```sh
lcom-ng compat build /path/to/project --debug --sanitize undefined
lcom-ng run --trace /path/to/device-trace.jsonl -- /path/to/project/.lcom-ng/proj
```

`--debug` adds symbols and disables optimization. `--sanitize undefined` reports
integer overflow and invalid array access. `--sanitize address` checks memory
access when the host compiler and runtime support it; `address,undefined`
enables both. These options preserve sources and instrument the generated host
program.

The builder verifies that the requested sanitizer can start before compiling. On
macOS 26, AddressSanitizer initialization has hung, also reported in
[LLVM issue 200447](https://github.com/llvm/llvm-project/issues/200447). The
builder returns an error after five seconds. UndefinedBehaviorSanitizer and
ordinary debug builds work on the tested Mac.

## Device compatibility

| Original interface | Host behavior |
| --- | --- |
| `<lcom/lcf.h>`, `<minix/...>`, `<machine/int86.h>` | Separate headers with original source contracts |
| `lcf_start`, `proj_main_loop`, lab commands | Student callback dispatch, built-in pixmap names, signed animation speed, runtime connection |
| `sys_inb`, `sys_outb` | Runtime device register access |
| `<minix/devio.h>` | Fixed-width port/value pairs and checked `pv_set` assignments |
| `CLOCK_IRQ`, LCF `timer_get_conf` | IRQ 0 and PIT status reads; student definitions override the fallback |
| IRQ policies and `driver_receive` | Shared physical IRQ lines, per-hook notification bits, disable/enable, reenabling policy |
| Minix endpoints and IPC status | FEUP Minix 3.4 constants, notification type/status and endpoint decoding |
| `tickdelay`, `sleep`, `usleep` | Virtual delays at 60 Hz, retaining coalesced notifications |
| `lm_alloc`, BIOS mode info, `vm_map_phys` | Low-memory buffers and stable VRAM maps, including unaligned page offsets |
| INT 10h `4F00`, `4F02`, `4F06` (BL=0/1/2), `4F07`, text restoration | Controller info, mode setup, logical scanline sizing/query, page flipping, mode 3 |
| INT 10h `1130` (BH=6), `sys_readbios` | BIOS 8×16 Latin font and bounded low-memory reads |
| IntelliMouse sample-rate negotiation | Device ID 3, four-byte packets and signed wheel input |
| Direct VRAM writes | Presentation before the next interrupt wait |
| `vt_info_get`, `vt_info_display`, VGA text memory | Course `libvt` metadata and mapped 80×25 CP437 cells at `0xB8000` |
| RTC CMOS registers and IRQ8 | BCD/binary, 12/24-hour time, clock writes, update/periodic/alarm flags, read-C acknowledgment |
| UART FIFO reception | Trigger levels 1/4/8/14, character-timeout IIR cause, reset on receiver read |
| `freopen("/dev/console", "w", stdout)` | Captured student output, including original Lab 1 assertions |
| XPM loading | Indexed and truecolor formats, transparency, multiple-character symbols, X11 colors |

### Video modes

The LCOM modes `0x105`, `0x110`, `0x115`, `0x11A`, and `0x14C` match the
[FEUP VBE handout](https://pages.up.pt/~up238172/aulas/lcom2223/labs/lab5/lab5_3.html).
`0x11A` is 1280 by 1024 RGB565. The native SDK's 1280 by 1024 32-bit host
extension uses `0x200`; its symbolic constant keeps its name.

Standard packed-pixel and direct-color modes are supported through `0x11F`,
including `0x107` and `0x11B`. Mode information and the BIOS mode list share the
SDK catalog. The
[SeaBIOS mode table](https://github.com/coreboot/seabios/blob/rel-1.16.1/vgasrc/svgamodes.c)
also supplies `0x143` (800×600, 32 bits), `0x144` (1024×768, 32 bits) and `0x14B`
(1152×864, 24 bits). Planar VGA graphics and VBE text catalog entries remain
unavailable.

Mapped BIOS mode 3 supports 80×25 color text with the CP437 font, foreground
intensity, default colors and blink. Text memory survives graphics-mode
transitions. The
[original `libvt` contract](https://pages.up.pt/~up238172/aulas/lcom2011/labs/lab1/lab1.html)
reports `vram_size` in 16-bit words; `vm_map_phys` takes bytes. The fixed `0xB8000`
text aperture must not overlap configured graphics or audio memory. Monochrome
text, uploaded fonts, CRT cursor/start registers and multiple text pages are not
implemented.

### Memory apertures

The default video aperture starts at `0xE0000000`. For a game that expects another
layout, set `LCOM_VBE_PHYSICAL_BASE` through
[`build.environment`](/students/project-config#runtime-environment), or in the CLI
environment. BIOS `PhysBasePtr` and physical mapping use that address; the default
is not retained as an alias. The
[VBE 3.0 specification](https://pdos.csail.mit.edu/6.828/2018/readings/hardware/vbe3.pdf)
defines a 32-bit physical framebuffer address, and
[SeaBIOS](https://github.com/coreboot/seabios/blob/rel-1.16.1/vgasrc/bochsvga.c)
reads it from the graphics device's PCI BAR.

The SDK's 2 MiB AC97 PCM window defaults to `0xD0000000`. A game whose framebuffer
uses that address can select a disjoint window with `LCOM_AC97_PCM_BASE`, for
example video `0xD0000000` and PCM `0xC0000000`. Both options are parsed at CLI and
embedded launch boundaries into an immutable machine configuration. Physical maps
and `lcom_ac97_get_buffer` use that owner; AC97 I/O ports and defaults are
unchanged. This relocates the SDK's PCM backing memory, not the controller's I/O
BAR.

### Legacy declarations

Older projects with local `struct reg86u` or zero-argument `lm_init` declarations
use matching legacy adapters. The importer retains their headers and sources. BIOS
font slots above ASCII remain blank; this font does not provide the complete CP437
set.

`lm_init(bool enable_logging)` keeps low memory available with either flag value.
The flag selects diagnostics, as named in the
[original liblm header](https://github.com/angelacoelho01/FEUP-LCOM-PROJECT/blob/91a05613c52571680533877b880cc4b1974ffbb5/lab5/liblm.h).
Calling it again preserves the memory base and live allocations.

## Interrupt handles

`sys_irqsetpolicy` takes a notification-bit index in `*hook_id` and replaces it
with a separate one-based hook handle. Save the bit before subscribing; use the
returned handle for enable, disable and removal. Replacing a policy takes the
original bit index again.
[Minix's kernel owner](https://github.com/Stichting-MINIX-Research-Foundation/minix/blob/v3.3.0/minix/kernel/system/do_irqctl.c)
and [libsys wrapper](https://github.com/Stichting-MINIX-Research-Foundation/minix/blob/v3.3.0/minix/lib/libsys/sys_irqctl.c)
define these roles. Distinct bits can share an IRQ, whose line remains masked
while any registered hook is disabled or awaiting one-shot reenable.

## Process identity

`sys_whoami` reports the hosted POSIX process ID with bit 30 set as its
compatibility endpoint. It is stable per process and disjoint from Minix
selectors. PIDs outside 1 through 2^30 - 1 return `EOVERFLOW`. The process name is
the Makefile `PROG` basename, truncated to 15 visible characters. Direct links
without importer metadata use the platform executable name. The display title does
not rename the process. Native paired programs have separate host processes;
browser programs use Emscripten's POSIX identity.

Privilege flags are zero: there is no Minix scheduler or privilege table.
`sys_enable_iop`, `sys_privctl(SYS_PRIV_ADD_MEM)`, `vm_map_phys` and
`vm_unmap_phys` accept `SELF` or this endpoint. Other processes and remote memory
operations are unsupported. This is a platform identifier, not a Minix
process-table encoding.

## Assets and paths

The importer detects a project/workspace asset root. `fopen` calls using
`/home/lcom/labs/` can resolve beneath it. For unusual layouts, set `--asset-root`
during import or `LCOM_COMPAT_PROJECT_ROOT` when running. Relative paths retain the
host process's working directory.

## Inline assembly and interrupt flag

Basic literal `CLI` and `STI` statements in C translation units are lowered during
compatibility compilation and app export. Original sources and provenance hashes
stay unchanged. They clear or set the compatibility process's interrupt-enable
flag, rather than nesting locks, as specified by
[AMD64 system instructions](https://docs.amd.com/api/khub/documents/sD1_QL~h4Afq2_tvzxqqSQ/content).
Devices, time and port I/O continue while notification delivery is disabled;
pending IRQ bits stay coalesced until `STI`. Cleanup restores delivery.

The bridge accepts single literal, explicitly volatile `inb %1, %0` and
`outb %0, %1` templates with the original `=a` output or `a` input and `Nd` port
constraint. The byte operand must have a character type; the port must be a 16-bit
integer expression or an unsigned 8-bit constant, matching
[GCC x86 constraints](https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html).
Each operand is evaluated once. I/O uses the existing byte-port interface; a
failed transport stops execution with a diagnostic. Original source bytes stay
unchanged.

Other register instructions, clobbers, labels, nonliteral templates and standalone
assembler files produce a source-location or compiler error. The bridge adds no
assembly keyword macros or CPU state to the native C API.

## Compatibility behavior

This mode models course device interfaces, not the Minix operating system. Unknown
BIOS functions and other Minix services return errors. The standard 256-color VGA
palette, six- and eight-bit DAC ports `0x3C6` through `0x3C9`, and BIOS palette
functions `4F08`/`4F09` are supported. Every mode set resets DAC precision to six
bits. BIOS palette entries use blue, green, red and an unused alignment byte, per
[VBE3.0, functions 08/09](https://pdos.csail.mit.edu/6.828/2018/readings/hardware/vbe3.pdf#page=61).
Direct-color modes reject palette operations; secondary palettes are unavailable.

Device timing is a model, and notifications may coalesce while the student program
is busy. Repeated delays retain one pending hardware notification with combined
IRQ bits, matching Minix; receiving it clears those bits. RTC periodic rates above
the PIT frequency coalesce within that tick; daylight-saving automation and
square-wave pins are not modeled.

### Output logging

`lcf_log_output(path)` saves all stdout bytes while keeping the console visible.
It captures `printf`, `puts`, `fprintf(stdout, ...)`, native print helpers and
direct stdout writes. `/dev/console` forwarding keeps the log active. Absolute
`/home/lcom/labs/` paths use the importer asset root; parent directories must
exist. A successful call starts a fresh file; a failed open preserves the active
log. `lcf_cleanup`, `lcf_cleanup_fast` and normal exit drain pending output and
restore stdout. Cleanup reports asynchronous log I/O errors. Stderr stays separate,
and abrupt termination can lose queued bytes.

Call `lcf_cleanup()` before replacing the process with POSIX `exec`; an active
logging worker does not survive `exec`. Fork/exec lifecycle compatibility is
outside this runtime.

### Call tracing and lab commands

LCF call tracing is unavailable because its wrapped-call format and source context
are not implemented. `lcf_trace_calls` returns false with `ENOSYS` and creates no
file. Use runtime `--trace` for device operations. The lab command parser accepts
`pic1`, `pic2`, `pic3`, `cross`, `penguin`, and `minix3_xpm`.

### Serial ports

UART connects virtual COM ports. FIFO timeouts use the configured baud, data,
parity and stop bits, rounded to the timer quantum. The virtual wire buffers
bursts; it does not simulate physical bit timing, baud mismatch, parity corruption,
or transmit shift-register delays. An external serial device needs its own peer.

### Mouse reporting

Minix starts with mouse stream reporting disabled, as described in the
[FEUP mouse handout](https://pages.up.pt/~up238172/aulas/lcom2223/labs/lab4/lab4_4.html).
To reproduce a VM where the mouse was already enabled:

```sh
LCOM_COMPAT_MOUSE_REPORTING=1 devenv shell -- lcom-ng run /path/to/proj/.lcom-ng/proj
```

This option applies only to the compatibility SDK. Native and normal compatibility
startup keep reporting disabled. The pinned Space Shooter project needs this
option because its initialization omits the mouse enable command.
