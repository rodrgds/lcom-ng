# Browser runtime feasibility

Original assessment: 15 October 2026, before implementation. The following records
the design proposal. See the [browser export guide](../docs-site/students/browser.md)
for the implemented command, controls and current limits.

## Recommendation

Add a browser export that compiles the game's C sources, an alternative SDK
implementation, and the existing virtual-device core into one WebAssembly module.
Use Emscripten and start with one game instance, one thread, keyboard/mouse,
graphics and audio. Keep the native process architecture for desktop teaching
and testing. A browser export recompiles sources; it cannot load an existing
native executable.

The proposed command is now implemented as
`lcom-ng export web <project> --output <dir>`, with `lcom-ng web <project>` for
automatic local preview. Both rebuild sources and libraries for WebAssembly.

The proposed output is an HTML launcher, JavaScript, WebAssembly and packaged
assets. Emscripten supports this deployment shape directly. These files can be
served by static hosting without a runtime server per player. Serve `.wasm` with
`application/wasm`, preserve relative asset URLs, and verify the exported folder
over HTTP before deployment. See [Emscripten deployment guidance](https://emscripten.org/docs/compiling/Deploying-Pages.html).

## Existing boundaries to replace

| Current owner | Browser implication |
| --- | --- |
| `runtime/core/RuntimeServer.cpp`, `setupSharedMemory` and `startChild` | Replace POSIX shared memory, Unix socket pair, pipes, `fork` and `execvp` with an embedded runtime session. |
| `sdk/client/src/lcom.c`, `request_reply` and `lcom_phys_map` | Provide the same public C API through calls into that session and owned framebuffer/audio buffers. |
| `runtime/core/Machine.hpp` and native single/paired servers | Reuse port access, IRQs, device models and virtual ticks. Extract shared session semantics for mapping validation, IRQ delivery and audio triggers so the browser bridge does not become a third divergent dispatcher. |
| `CMakeLists.txt` | Add a separate cross-compilation target; exclude native CLI/process owners. Current display/audio code uses SDL3 and SDL3_ttf, not SDL2. |
| `sdk/compat/src/output_log.c` | Replace the pthread/pipe/poll/dup2 logger for browser builds, or report its unsupported operation explicitly. Do not silently claim full LCF behavior. |

Emscripten does not provide native multiprocessing through `fork`. Its networking
emulation and POSIX socket proxy require different transports and, for the full
proxy, a server. Keeping that proxy would defeat the standalone export goal.
See [pthreads limitations](https://emscripten.org/docs/porting/pthreads.html)
and [networking options](https://emscripten.org/docs/porting/networking.html).

## Scheduling, rendering and input

Existing student loops must yield to the browser. Prototype Asyncify at SDK event
waits and delays, plus bounded yielding in presentation and port-polling paths.
Legacy polling loops may never call `driver_receive`; yielding only at IRQ waits
would leave those games frozen. Use an elapsed-time budget rather than an async
round trip for every port operation. Asyncify preserves synchronous-looking C
control flow but adds code size and runtime cost. An explicit frame callback is
cleaner for new games but changes student sources. See
[Emscripten asynchronous code](https://emscripten.org/docs/porting/asyncify.html).

SDL3 already has an Emscripten backend, with SDL rendering translated to WebGL.
Cross-compile SDL3 and SDL3_ttf, including their required font dependencies, or
use a small browser display backend if the desktop shell proves unnecessary.
Do not substitute SDL2 build flags for the current SDL3 API. Rendering becomes
visible when control returns to the browser. Start through a Play button so
audio can unlock after user interaction. See the
[SDL3 Emscripten guide](https://wiki.libsdl.org/SDL3/README-emscripten).

Preserve device time independently of the display refresh rate. A PIT configured
at 60 Hz is not a promise that the browser produces exactly 60 callbacks per
second. Define bounded catch-up and hidden-tab pause behavior, release held keys
on blur, and test pointer lock, focus, scaling and audio with real browser input.
The browser guide records the implemented behavior and current verification scope.

## Files, threads and serial

Package images, sounds, fonts and game data into Emscripten's virtual filesystem,
preserving paths expected by the game. `--preload-file` supports directories and
host-to-virtual path mapping. Default MEMFS writes disappear on reload; saved
games need an explicit persistent store such as IDBFS and synchronization. Host
absolute paths require a deliberate mapping or browser import/export flow.
See [packaging files](https://emscripten.org/docs/porting/files/packaging_files.html)
and [filesystem overview](https://emscripten.org/docs/porting/files/file_systems_overview.html).

Avoid requiring pthreads initially. They add SharedArrayBuffer, COOP/COEP hosting
headers, worker startup and main-thread proxy/deadlock constraints. A threaded
and a non-threaded build are separate artifacts. If real student games cannot
yield cooperatively, evaluate a worker build as a later alternative; it still
does not restore native processes or Unix shared-memory objects. See
[Emscripten pthreads](https://emscripten.org/docs/porting/pthreads.html).

UART loopback can stay local to the virtual machine. Paired games in one page
would require independent guest state, scheduling and display/input routing;
`Machine::connectSerialPeer` supplies only the device link. Players in different
browsers require a new WebSocket relay or WebRTC transport plus connection and
timing policy. Static hosting alone does not provide that service. Browser
networking cannot directly reuse native TCP/UDP sockets. See
[Emscripten networking](https://emscripten.org/docs/porting/networking.html).

## First implementation acceptance

1. Export one existing native example without changing its game logic.
2. Verify timer/keyboard/mouse, framebuffer pixels, audio, asset loading, quit
   and restart in a real browser, including focus loss and tab suspension.
3. Serve a relocated export folder through plain static HTTP hosting.
4. Add the legacy adapter and validate representative unchanged projects from
   the pinned compatibility corpus, including polling and output logging.
5. Check the relevant browser/device combinations before documenting support.

This assessment predates implementation. Current commands, controls, compatibility
and verification limits are maintained in the browser export guide linked above.
