# Play and share your game in a browser

## Open a game

One command builds, serves and opens the game:

```sh
lcom-ng web path/to/my-game
```

Inside the project folder:

```sh
lcom-ng web .
```

The project path defaults to the current folder. In this repository, prefix commands
with `devenv shell --`:

```sh
devenv shell -- lcom-ng web examples/ninjix
```

Click the game to capture the mouse; the cursor moves past the browser edge.

**Escape** reaches the game. In fullscreen, browsers with Keyboard Lock keep
capture. Windowed play and other browsers reserve Escape to release capture and
also send it to the game once. Click again to recapture.

**Shift+Escape** releases a captured mouse; in a collection, press it again to
return to the library. The top-right button toggles fullscreen.

**Right Option on Mac** and **Right Ctrl** release the mouse without sending
Escape. Command stays available for app shortcuts. Sound starts on the first
keypress or tap. Refresh restarts; press Enter after the game exits. If the browser
cannot capture the pointer, the game reports it and keeps ordinary dragging.

On mobile, touch presses the left button at the game cursor; lift to click, drag to
move with the button down. This is the default relative control. Projects can
enable [direct touch](./project-config#direct-touch) to tap at the finger.

- **Mouse** stick moves without clicking. **Left click** and **Right click** can be
  held with another finger; holding **Right click** makes a drag use that button.
- **Esc**, **Space** and **Enter** stay within reach. **Keyboard** shows the full
  keyboard, including latched Shift, Ctrl and Alt and **Fn** keys.
- **Joystick** starts in **Mouse** mode; push further to move faster. **Arrow keys**
  or **WASD** switch it to keyboard movement. **Mouse + arrows** or
  **Mouse + WASD** shows two sticks.
- **Hide controls** collapses the panel; **Controls** reopens it. Panels start
  closed. Landscape places panel controls beside the game; portrait moves them to
  the bottom.

Press Ctrl+C to stop. Re-run after source changes. `--port 8080` chooses a port,
`--no-open` prints the URL, and `--host 0.0.0.0` lets a phone on the same network
connect.

## Play published games

- [Plants vs. Zombies](https://plantsvszombiesrafa.pages.dev/)
- [Ninjix](https://ninjix.rgo.pt/)
- [Sushi Slash](https://sushi-slash.pages.dev/)

Collection games with authored instructions open on **Start game**.
**Instructions** reopens them during play.

## Your project

Optional [`lcom.json`](./project-config) sets title, icon, preview, mobile settings
and theme; web and Android share it. For instruction screens and shortcut rows, see
[Instructions in the browser Arcade](./project-config#instructions-in-the-browser-arcade).

The exporter discovers:

- C files in the project root and under `src/`, with headers under `include/`;
- generated student workspaces, including `proj/`, `lib/` and `labs/`;
- original Minix Makefiles and their source libraries.

`assets/` is included automatically, preserving paths. For another layout, select
files explicitly:

```sh
lcom-ng web . --source game/main.c --source shared/timer.c --include headers \
  --asset images --asset sounds
```

Explicit `--asset` replaces the automatic `assets/`. Use `--arg value`, or
`--arg=--option` for values starting with a dash. Minix source libraries use
`--library name=file.c,other.c`.

For legacy absolute paths, select the parent workspace:

```sh
lcom-ng web . --asset-root .. --asset proj/assets
```

Sources stay unchanged. The exporter rebuilds for WebAssembly; it does not convert
a desktop or Minix binary.

## Publish

```sh
lcom-ng export web path/to/my-game --output dist/my-game
```

Upload the complete folder to static hosting. Keep files together and serve `.wasm`
as `application/wasm`. HTTPS is recommended. Players need only a browser; no
special isolation headers are required.

Preview with `python3 -m http.server 8080 --directory dist/my-game`. Opening
`index.html` from the filesystem is not supported.

## Browser compatibility

The same export targets current Chrome, Edge, Firefox and Safari. Emscripten
compiles to WebAssembly using
[Asyncify](https://emscripten.org/docs/porting/asyncify.html); no threads,
`SharedArrayBuffer` or JavaScript Promise Integration are required.

| Browser | Expected behavior | Verification |
| --- | --- | --- |
| Chrome and Edge | Game, audio, keyboard and captured desktop mouse input | Chromium automated tests |
| Firefox on desktop | Same export, with desktop mouse capture | Automated collection startup and pause/resume checks |
| Safari on macOS | Same export, with desktop mouse capture | Not yet tested |
| Safari on iPhone and iPad | Game, audio and custom touch keyboard; no Pointer Lock | Physical devices not yet tested |

Desktop capture uses Pointer Lock after a click. Touch keeps relative or direct
controls. Embedded browsers may restrict capture. Mobile tests use touch emulation
and do not certify every browser.

## Tools and limits

Devenv supplies the compiler and build tools. Outside Devenv, install Emscripten,
Python 3.11 or newer, CMake and Ninja. Minix exports also need the matching
`llvm-link` next to Emscripten's Clang. Earlier archives may not include these
commands.

- Single player and local UART loopback are supported. Paired instances and online
  multiplayer are not.
- Saved files disappear on reload. The clipboard is private to the game.
- Program output goes to the browser console. LCF output-file logging reports that
  it is unavailable.
- Inactive windows and hidden tabs pause virtual time and release held input.
- Native processes, threads, assembly and external libraries may need changes. Long
  computation without SDK calls cannot yield to the browser.
