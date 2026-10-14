# Run existing Minix labs and projects

Use the compatibility SDK to build existing LCOM C sources with their original imports, entry points, and device code. The importer reads the original Minix Makefile and writes host build artifacts to `.lcom-ng/`.

For a release installation, start with [download and install](/students/install).

## Build and run

In the folder containing your original Makefile:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Use the generated binary path printed by the builder. The executable name comes from `PROG`. Original `.c`, `.h`, imports, and Makefile stay unchanged. Rebuild after editing student sources.

For deterministic input:

```sh
lcom-ng run --headless --max-ticks 600 \
  --script /path/to/input.mlabscript --dump-frame build/project.ppm \
  -- /path/to/labs/proj/.lcom-ng/proj
```

`--max-ticks` bounds virtual time. Use `--timeout 30` to stop a busy loop after 30 host seconds with exit code 124. Compilation does not prove gameplay, serial communication, or rendering.

## Run original lab callbacks

The same builder handles a lab directory. Keep the original function name and
pass its arguments after the generated binary:

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
`READ_DATA` polling; stream reporting starts disabled. The native course starter
and its predefined `lcom-ng test` checks use the separate native API.

## Build inputs

The importer supports `SRCS`, explicit `.o: .c` rules, `.PATH`, `.PATH.c`, `VPATH`, preprocessing and optimization flags, and local archives declared through `LIB_SRCS`, named `LIB_*_SRCS`, `LIB` with `SRCS`, `LIB_OBJS`, or declarative archive object recipes. Direct object inputs in `LDADD` are rebuilt from their original C sources and owning Makefile. Single-variable BSD `.for` source-directory loops and headers in `SRCS` are supported. It does not execute Makefile recipes or Minix system make includes.

For archives created through a separate script, provide their sources explicitly:

```sh
devenv shell -- lcom-ng compat build /path/to/project \
  --library timer=../libs/timer.c --library video=../libs/video-card.c \
  --include include --output build/imported-project
```

Repeat `--library` for each archive. An explicit library is linked even when the original Makefile omitted its dependency. Separate multiple sources with commas. Library sources and extra include directories are relative to the project. Existing Minix executables and archives must be rebuilt for the host.

The importer records source SHA-256 values, compiler commands and build status in `.lcom-ng/build.json`. Missing sources and unsupported Makefile expressions produce errors. Absolute `/home/lcom/labs/` library and include paths resolve under the inferred workspace or explicit `--asset-root`.

## Diagnose student code

Compiler failures name the source or link stage. A failed compilation removes
the previous generated executable and records the error in `build.json`.
Runtime crashes name the terminating signal and suggest debugging commands.

```sh
lcom-ng compat build /path/to/project --debug --sanitize undefined
lcom-ng run --trace /path/to/device-trace.jsonl -- /path/to/project/.lcom-ng/proj
```

`--debug` adds symbols and disables optimization. `--sanitize undefined` reports
errors such as integer overflow and invalid array access in the original source.
`--sanitize address` checks memory access when the host compiler and sanitizer
runtime support it; `address,undefined` enables both. These options preserve
source files and instrument the generated host program.

The builder checks that the requested sanitizer can start before compiling the
project. AddressSanitizer currently hangs during initialization on this trial's
macOS 26 host, also reported in [LLVM issue 200447](https://github.com/llvm/llvm-project/issues/200447).
The builder returns a useful error after five seconds. UndefinedBehaviorSanitizer
and ordinary debug builds work on the tested Mac.

## Device compatibility

| Original interface | Host behavior |
| --- | --- |
| `<lcom/lcf.h>`, `<minix/...>`, `<machine/int86.h>` | Separate headers with original source contracts |
| `lcf_start`, `proj_main_loop`, lab commands | Student callback dispatch, built-in pixmap names, signed animation speed, and runtime connection |
| `sys_inb`, `sys_outb` | Runtime device register access |
| IRQ policies and `driver_receive` | Hook bits, notification messages, disable/enable, and reenabling policy |
| Minix endpoints and IPC status | FEUP Minix 3.4 numeric constants, notification type/status and endpoint decoding |
| `tickdelay`, `sleep`, `usleep` | Virtual delays using a 60 Hz system clock, retaining coalesced notifications |
| `lm_alloc`, BIOS mode info, `vm_map_phys` | Low-memory buffers and stable VRAM maps, including unaligned page offsets |
| INT 10h `4F00`, `4F02`, `4F07`, text restoration | Controller information, mode setup, linear page flipping, and mode 3 |
| Direct VRAM writes | Presentation before the next interrupt wait |
| RTC CMOS registers and IRQ8 | BCD/binary, 12/24-hour time, clock writes, update/periodic/alarm flags and read-C acknowledgment |
| UART FIFO reception | Trigger levels 1/4/8/14, character-timeout IIR cause and reset on receiver read |
| `freopen("/dev/console", "w", stdout)` | Captured student output, including original Lab 1 assertions |
| XPM loading | Indexed and truecolor formats, transparency, multiple-character symbols, and X11 colors |

The LCOM modes `0x105`, `0x110`, `0x115`, `0x11A`, and `0x14C` match the [FEUP VBE handout](https://pages.up.pt/~up238172/aulas/lcom2223/labs/lab5/lab5_3.html). In particular, `0x11A` is 1280 by 1024 RGB565. The native SDK's 1280 by 1024 32-bit host extension uses `0x200`; its symbolic constant retains its name.

The runtime also supports standard packed-pixel and direct-color modes through
`0x11F`, including `0x107` and `0x11B`. Mode information and the BIOS mode list
share the SDK catalog. Planar VGA and VBE text modes require other device
semantics and remain unavailable.

## Assets and limits

The importer detects a project/workspace asset root. `fopen` calls using `/home/lcom/labs/` can resolve beneath it. For unusual layouts, set `--asset-root` during import or `LCOM_COMPAT_PROJECT_ROOT` when running. Relative paths retain the host process's working directory.

This mode models course device interfaces, not the Minix operating system. Unknown BIOS functions and other Minix services return errors. The standard 256-color VGA palette and six-bit DAC ports `0x3C6` through `0x3C9` are supported. BIOS palette functions and extended DAC precision are unavailable. Device timing is a model, and notifications may coalesce while the student program is busy. RTC periodic rates above the PIT simulation frequency coalesce within that tick; daylight-saving automation and square-wave pins are not modeled.

`lcf_log_output(path)` saves all stdout bytes while keeping the runtime console visible. It captures `printf`, `puts`, `fprintf(stdout, ...)`, native print helpers and direct stdout writes. `/dev/console` forwarding keeps the log active. Absolute `/home/lcom/labs/` paths use the importer asset root; parent directories must already exist. A successful call starts a fresh file, and a failed file-open attempt preserves the active log. `lcf_cleanup`, `lcf_cleanup_fast` and normal process exit drain pending output and restore stdout. Cleanup reports asynchronous log I/O errors. Stderr remains separate, and abrupt termination can lose bytes still queued for logging. Call `lcf_cleanup()` before replacing the process with a POSIX `exec` function; an active logging worker does not survive `exec`. Fork/exec lifecycle compatibility is outside this course runtime.

LCF call tracing remains unavailable because its original wrapped-call format and source context are not implemented. `lcf_trace_calls` returns false with `ENOSYS` and creates no file. Use runtime `--trace` for device operations. The lab command parser accepts `pic1`, `pic2`, `pic3`, `cross`, `penguin`, and `minix3_xpm`.

UART connects virtual COM ports. FIFO timeout intervals use the configured baud,
data, parity and stop bits, rounded to the runtime's timer quantum. The virtual
wire buffers bursts; it does not simulate physical wire bit timing, baud mismatch,
parity corruption or transmit shift-register delays. A project's external music
player or other serial device needs its own peer. Linux and Windows/MSYS2 release
workflows build the same SDK; this trial's interactive checks run on macOS.

Minix starts with mouse stream reporting disabled, as described in the [FEUP mouse handout](https://pages.up.pt/~up238172/aulas/lcom2223/labs/lab4/lab4_4.html). A project that depended on a mouse previously enabled in its VM can reproduce that starting state explicitly:

```sh
LCOM_COMPAT_MOUSE_REPORTING=1 devenv shell -- lcom-ng run /path/to/proj/.lcom-ng/proj
```

This option applies only to the compatibility SDK. The native SDK and normal compatibility startup keep reporting disabled. The pinned Space Shooter project needs this option because its initialization omits the mouse enable command.
