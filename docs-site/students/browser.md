# Play and share your game in a browser

## Open a game

One command builds the game, starts a local server and opens your browser:

```sh
lcom-ng web path/to/my-game
```

Inside the project folder, run:

```sh
lcom-ng web .
```

The project defaults to the current folder. In this repository, prefix commands
with `devenv shell --`. Examples use the same project-folder command:

```sh
devenv shell -- lcom-ng web examples/ninjix
```

The game starts automatically. Desktop players see the game with black bars
where needed. Click the game to capture and hide the system mouse. **Right Ctrl**
releases capture. **Escape** reaches the game and also releases capture, so going
back or pausing takes one press. Sound starts on the first keypress or tap.
Refresh the page to restart, or press Enter after the game exits.
Mouse capture requires Pointer Lock support. Embedded browsers without it hide
the system cursor over the game but cannot keep it inside the window.

On mobile, drag anywhere above the controls to move the game cursor, like a
touchpad. Lifting and placing your finger never clicks or jumps the cursor.
Use **Left click** and **Right click** separately. Hold either button with one
finger while dragging with another to drag objects or slash.

**Esc**, **Space** and **Enter** stay within reach. Tap **Keyboard** to show or
hide the full keyboard. Hold keys, combine touches, or tap Shift, Ctrl and Alt
to latch modifiers. **Fn** switches to function keys, navigation and punctuation.
The optional **Joystick** starts in **Mouse** mode: hold the stick to move the
cursor, and push further to move faster. **Arrow keys** and **WASD** control
keyboard movement instead. **Mouse + arrows** or **Mouse + WASD** shows two
independent sticks, so you can move and aim together while holding a click button.
Both panels start closed to leave more room for the game. Desktop stays game-only.

Press Ctrl+C in the terminal to stop the preview. Re-run the command after source
changes. Use `--port 8080` to choose a port or `--no-open` to copy the printed URL
into another browser. To try a phone on the same network, add `--host 0.0.0.0`
and open your computer's network address with the printed port.

## Play published games

- [Plants vs. Zombies](https://plantsvszombiesrafa.pages.dev/), built from the unchanged original student archive.
- [Ninjix](https://ninjix.rgo.pt/).
- [Sushi Slash](https://sushi-slash.pages.dev/), built from the original upstream project.

## Your project

The exporter discovers:

- C files in the project root and under `src/`, with headers under `include/`;
- generated student workspaces, including `proj/`, `lib/` and `labs/`;
- original Minix Makefiles and their source libraries.

An `assets/` folder is included automatically, preserving paths such as
`assets/background.bmp`. For another layout, select files explicitly:

```sh
lcom-ng web . --source game/main.c --source shared/timer.c --include headers \
  --asset images --asset sounds
```

Explicit `--asset` options replace the automatic `assets/` selection. Use
`--arg value` for each game argument; use `--arg=--option` for values beginning
with a dash. Minix source-library overrides use `--library name=file.c,other.c`.

For legacy absolute paths such as `/home/lcom/labs/proj/assets`, select the parent
workspace as the asset root:

```sh
lcom-ng web . --asset-root .. --asset proj/assets
```

Source files stay unchanged. The exporter rebuilds for WebAssembly rather than
converting a desktop or Minix executable.

## Publish

Create a standalone export without starting a preview server:

```sh
lcom-ng export web path/to/my-game --output dist/my-game
```

Upload the complete output folder to static hosting. Keep the files together and
serve `.wasm` as `application/wasm`. HTTPS is recommended. Players need a browser,
without installing lcom-ng or connecting to a game server. No special isolation
headers are required.

Exports can also be previewed with `python3 -m http.server 8080 --directory dist/my-game`.
Opening `index.html` directly from the filesystem is not supported.

## Browser compatibility

The same export targets current Chrome, Edge, Firefox and Safari. Emscripten
compiles the game and device core to WebAssembly. The build uses
[Asyncify](https://emscripten.org/docs/porting/asyncify.html) and does not require
threads, `SharedArrayBuffer` or JavaScript Promise Integration.

| Browser | Expected behavior | Verification |
| --- | --- | --- |
| Chrome and Edge | Game, audio, keyboard and desktop mouse capture | Chromium automated tests; native Chrome Escape checked |
| Firefox on desktop | Same export, including mouse capture | Not yet tested |
| Safari on macOS | Same export, including mouse capture | Not yet tested |
| Safari on iPhone and iPad | Game, audio and custom touch keyboard; no Pointer Lock | Physical devices not yet tested |

Desktop Firefox and Safari support mouse capture. Safari on iOS does not expose
Pointer Lock. Embedded browsers may also omit it. See the
[Pointer Lock compatibility data](https://developer.mozilla.org/en-US/docs/Web/API/Element/requestPointerLock#browser_compatibility).
Browser API availability is separate from testing this player on each device.

Chromium tests cover Flappy, Ninjix, native and Minix fixtures, installed SDK
relocation, student workspaces, mouse capture, a single Escape after capture,
multi-touch keyboard input, relative touch dragging, separate mouse buttons,
and the optional joystick. Mobile tests use touch emulation. This does not
certify every Minix project or every mobile browser.

## Tools and limits

The repository's Devenv shell supplies the compiler and build tools. Outside
Devenv, install Emscripten, Python 3.11 or newer, CMake and Ninja. Minix exports
also need the matching `llvm-link` next to Emscripten's Clang. Current source
builds include these commands; earlier published archives do not.

- Single player and local UART loopback are supported. Paired instances and online
  multiplayer are not included.
- Saved files in the virtual filesystem disappear on reload. The clipboard is
  private to the game.
- Program output appears in the browser's developer console. LCF output-file
  logging explicitly reports that it is unavailable.
- Inactive windows and hidden tabs pause virtual time and release held input.
- Native processes, threads, assembly and external system libraries may require
  changes. Long computation without SDK calls cannot yield to the browser.
