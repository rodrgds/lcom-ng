# Embedded app runtime

Desktop `run` and `run-pair` preserve the process-isolated runtime and its local
protocol. Browser and Android exports rebuild the student's sources and link the
public C SDK directly to a device machine inside the app process.

## Owners

- `runtime/core/Machine.*` and `runtime/devices/`: device registers, IRQs, timer
  frequency and framebuffer semantics. Platform code must not reimplement them.
- `runtime/embedded/Client.cpp`: in-process C SDK, mapped memory, bounded clock
  catch-up and event waits. This replaces the desktop socket client for exports.
- `runtime/embedded/Host.hpp`: the platform seam. One host supplies monotonic
  milliseconds, yielding, event pumping, pause state, graphics, audio and
  clipboard policy. Each executable links its own `host()` implementation.
- `runtime/web/BrowserClient.cpp`: browser host and exported input callbacks.
  Asyncify remains browser-specific. It is absent from native exports.
- `runtime/embedded/SdlHost.cpp`: native app host. SDL owns the window, rendering,
  touch controls and audio. Common 24/32-bit frames upload directly from their
  BGR backing memory; other modes use the core's RGB conversion.
- `runtime/embedded/Player.cpp`: SDL entry point, private asset working directory,
  original student entry point and orderly termination.
- `sdk/export/config.py`: validates optional `lcom.json` once, resolves CLI
  overrides and produces the web profile or compiled native `ControlConfig`.
  The schema is `sdk/export/lcom.schema.json`. Glyph atlases and control geometry
  belong to the host, not the simulated devices.
- `sdk/export/project.py`: shared source discovery/options and relocatable CMake
  source export. Minix traversal reuses `sdk/compat/build_project.py` without
  executing Makefile recipes. Selected source objects reach the final app
  directly, preserving initialized data-only units even when legacy headers
  contain tentative definitions. Do not wrap them in another static archive.
- `sdk/android/`: Android activity, Gradle/NDK packaging, verified SDL download
  and CLI export. Installed SDKs carry shared sources under `sdk/runtime/`.

## Execution contract

One embedded game runs per process. Its SDK calls and host event pumping run on
the same thread, SDL's native app thread on Android. Device state is not accessed
from Android's Java UI thread. Long computation without SDK calls cannot service
input. SDL close unwinds to the player entry point, including legacy polling
loops that ignore SDK errors. A guest crash terminates its app process.

Mapped C pointers retain stable backing allocations across mode changes. Present
copies the selected virtual video page into the device framebuffer before giving
it to the host. Native audio uses SDL's PCM stream. Browser audio and yielding
stay behind the browser host. Clipboard is private per browser game and uses the
system clipboard in the SDL host, subject to platform access rules.

Games with overlay controls, or no control panel, fit the whole display surface
at their original aspect ratio. Controls remain inside the safe area; hidden
Android system-bar insets do not shrink the game. Direct touch uses this same
fitted game rectangle.

## Adding another app host

Implement `Host` and link it with `Client.cpp`, `Machine` and the device sources.
Inject keyboard and relative mouse input through `embedded::injectKey` and
`embedded::injectMouse` on the host thread. Mouse Y follows screen coordinates.
`embedded::injectPointer` maps an absolute target into relative packets. The
shared `Pointer` keeps the configured guest cursor position and applies its bounds after
each delivered packet. It holds at most one packet in the controller, coalesces
pending motion and preserves button edges at their coordinates. Direct mapping
requires the guest's initial position and clamp or per-axis rejection bounds, unit deltas and no cursor
warps. `configureCursor` supplies that contract before graphics mode setup.
Keep packet delivery and cursor tracking here, shared by browser and SDL hosts.
Keep platform lifecycle, rendering and assets in the host/exporter. Preserve
student SDK calls and device behavior. Do not add platform branches to the SDK
or copy device logic into a new runner.

Touch press policy is independent of pointer coordinates. Both hosts support
held contact, tap-on-release with eight-unit drag cancellation, and hover-only
contact. Configured pads and named actions retain independent finger ownership.
Legacy imports default to signed `char` across targets, preserving x86 Minix
sentinel values on Android ARM. Authored compiler flags override this default.

The opt-in `LCOM_COMPAT_MOUSE_IRQ_ONLY=1` embedded compatibility policy suppresses
host mouse delivery while IRQ12 is unsubscribed and clears pending button edges
at unsubscribe. This preserves keyboard-only transitions in frozen games that
disable reporting after unsubscribing. Ordinary polled device behavior is unchanged
when the override is absent.

Collection chrome receives actual framebuffer and occupied control rectangles.
The browser publishes layout changes to its owning iframe parent. Android's
`AndroidLayout.hpp` publishes quantized geometry through the optional Java
`onGameLayout` callback. The Games action uses safe free gutters first, then a
free overlay corner. Unchanged frames do not repeat the bridge call.

Unchanged framebuffer presentations skip host uploads and rendering. Mode changes
and palette writes force a new upload. SDL restores the last committed frame on
resume and rendering context resets, without waiting for guest drawing. Android
app lifecycle events reach SDL event watchers without entering the polled queue.
The host defers these events to its own loop, then restores the frame after SDL
restores the graphics context. Android submits this restored scene twice, reusing
the texture, because the first presentation can leave a recreated surface blank.
The emulator check verifies pixels across three background/resume cycles without
another touch or guest repaint. Legacy notification loops present between input
bytes, so rendering every such call can create seconds of stale mouse input.

Android's generated native CMake project can also be built with desktop SDL3 for
inspection. It uses its exported source/SDK snapshot after relocation, without
requiring the original project. This is a verification path, not another public
export command.

## Verification

- `embedded_sdk` checks device-facing SDK behavior against independently chosen
  scancodes, framebuffer bytes, timer waits and PCM samples.
- `embedded_sdl` checks real SDL event processing, held input ownership, focus
  release, pixels and quit. CTest uses dummy video/audio; real-window trials are
  separate evidence.
- `tests/project_config.py` checks optional defaults, strict validation, paths,
  overrides, example profiles and bundle identity after relocation.
- `tests/android_export.py` checks CLI rejection/preservation, relocation after
  deleting the input, native compilation and installed Minix export.
- `tests/android_export.py --build-apk` also checks signed native ELF libraries,
  packaged assets and 16 KB APK alignment.
- `tests/android_smoke.py` requires an Android emulator and checks rendering,
  touch, keyboard, maximum proportional fit, resume, rotation, initialized
  data-only coordinate tables and exit. CI config lives in
  `.github/workflows/android.yml`. Device performance requires device measurements.
- `python3 tests/android_pvz_smoke.py dist/plants-vs-zombies-android` checks an
  exported original PvZ APK on a connected Android device: full-surface fit,
  deck dragging, five mower rows, planting after release and zombie lanes.
  It uses the export's bundled art for pixel checks and preserves game sources.

## Cursor latency check, 15 October 2026

A local Chrome replay drove 120 pointer events across the original PvZ main menu,
with fourfold CPU throttling. It detected the game's white cursor in presented
frames. Before the change, the cursor was still following old movement one second
after input stopped. Skipping unchanged presentations alone reached the final
position after about 40 ms. With packet backpressure and direct mapping, one
recorded run reached it after 16.8 ms and passed a 100 ms settling check.

The baseline presented 211 frames during the measured interval, including
duplicates between mouse bytes. The final run presented 50 changed frames.
These counts are work avoided, not a claim that game animation slowed down.
The SDK regression checks that 300 unchanged presents cause no host uploads;
separate tests cover changed pixels, palette changes and rendering invalidation.

These are local browser measurements. Emulator checks and physical-device
performance are separate evidence.
