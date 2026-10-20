# Configure your project

Add an optional **`lcom.json`** to your project folder. Web and Android exports and
desktop bundles read it automatically; projects without it still build and run
unchanged.

## Start small

```json
{
  "name": "Sushi Slash",
  "icon": "icon.png",
  "mobile": {
    "orientation": "landscape",
    "fullscreen": true,
    "controls": {
      "keyboard": false,
      "mouseButtons": false,
      "joystick": false,
      "quickKeys": [],
      "layout": "overlay"
    }
  }
}
```

This sets the title, icon, landscape fullscreen and a clear game area. Touch drag
still moves the cursor; Android Back sends Escape.

```sh
lcom-ng export android . --output dist/android
lcom-ng export web . --output dist/web
lcom-ng web .
```

Use a square PNG at least 48 pixels wide; 1024×1024 with a centered subject suits
Android. Web exports use it for the tab and manifest.

Project paths are relative to the project; `icon` and `preview` are relative to the
config file, including with `--config`:

```sh
lcom-ng export web . --config profiles/touch.json
lcom-ng export android . --name 'Practice build' --orientation auto --icon other.png
```

CLI options override the file. Repeated source, include, asset, library and argument
options replace arrays. Invalid settings and missing files name the setting. Any
subset is valid; optional `version` must be `1`.

## Collection metadata

Add a description and screenshot:

```json
{
  "name": "Sushi Slash",
  "description": "Slice the flying sushi before it falls.",
  "preview": "screenshots/gameplay.png"
}
```

`description` is optional, up to 500 characters. `preview` is an optional PNG at
least 48 pixels per side, relative to the config file; 800×600 works well. The
library crops it to a wide tile and overlays the name, or uses the icon. The loading
modal shows the description and progress; without instructions, the game opens at
its first frame. These fields do not alter gameplay.

Optional `archetype`, such as `"Snake"`, groups related games. Nine use a 3×3 block
in the space of four tiles; each keeps its icon and controls.

## Instructions in the browser Arcade

Add optional `instructions`. A web collection shows them while downloading, then
waits for **Start game**; players can reopen **Instructions** and resume without
restarting. Games without it open directly. Only web collections display them;
standalone exports include the metadata in `game.json`. Native controls and code are
unchanged.

```json
{
  "instructions": {
    "sections": [
      {
        "title": "Before you start",
        "text": "Type a five-letter nickname, then press Space."
      },
      {
        "title": "Controls",
        "controls": [
          { "action": "Start", "bindings": [{ "keys": ["Space"] }] },
          { "action": "Jump", "bindings": [{ "keys": ["Up"] }] },
          {
            "action": "Move",
            "bindings": [{ "keys": ["A"] }, { "keys": ["Left"] }],
            "note": "Either key moves left."
          },
          { "action": "Aim", "bindings": [{ "label": "Move the mouse" }] }
        ]
      }
    ]
  }
}
```

Each section can contain `text`, `controls`, or both. Titles are optional; text
preserves line breaks. Use a short first section for setup.

### Keyboard shortcut rows

| Field | Purpose |
| --- | --- |
| `action` | Short label shown on the left, such as `Jump`, `Pause` or `Select`. |
| `bindings` | Alternatives for that action. Put each alternative in its own object. |
| `keys` | One chord inside a binding. `["Shift", "S"]` means press both together. |
| `label` | Text for mouse, touch, gamepad or other input, such as `Left click` or `Drag`. |
| `platforms` | Optional host filter: `macos`, `windows`, `linux`, `ios` or `android`. |
| `note` | Optional short hint shown under the action. |

Keys in one binding form a chord. Separate bindings are alternatives:

```json
{
  "action": "Move left",
  "bindings": [
    { "keys": ["A"] },
    { "keys": ["Left"] }
  ],
  "note": "Either key moves left."
}
```

A binding needs exactly one of `keys` or `label`. Use keycap names such as `Space`,
`Esc`, `F1` or `1`, not scancodes or `KEY_*` names. `instructions` keys are display
labels; `quickKeys` use guest names such as `ESC`.

A complete config can combine instructions with mobile settings:

```json
{
  "name": "My game",
  "instructions": {
    "sections": [
      {
        "title": "Keyboard",
        "controls": [
          { "action": "Move", "bindings": [{ "keys": ["A"] }, { "keys": ["D"] }] },
          { "action": "Jump", "bindings": [{ "keys": ["Space"] }] },
          { "action": "Pause", "bindings": [{ "keys": ["Esc"] }] }
        ]
      }
    ]
  },
  "mobile": {
    "orientation": "landscape",
    "controls": {
      "quickKeys": ["ESC", "SPACE"]
    }
  }
}
```

For different host shortcuts, write explicit platforms:

```json
{
  "action": "Save",
  "bindings": [
    { "keys": ["Command", "S"], "platforms": ["macos"] },
    { "keys": ["Ctrl", "S"], "platforms": ["windows", "linux"] }
  ]
}
```

The viewer selects matching bindings. Omit `platforms` for any-platform input.
Supported values are `macos`, `windows`, `linux`, `ios` and `android`; unknown
platforms show every variant with a label. `Command`, `Option`, `Shift` and arrows
get symbols. The viewer never remaps keys, so verify each row.

Do not list the Arcade's own controls: Right Ctrl, Right Option on macOS, and
Shift+Escape release the mouse or return to the collection.

Validation allows up to 8 sections, 16 controls per section, 8 alternatives per
control and 6 keys per chord. Section text is limited to 1,000 characters. Missing
fields, empty controls, unknown properties and invalid platforms produce a
setting-specific error.

## Mobile controls

| Setting under `mobile` | Default | Choices |
| --- | --- | --- |
| `orientation` | `"auto"` | `"auto"`, `"landscape"`, `"portrait"` |
| `fullscreen` | `false` | Request fullscreen on mobile |
| `controls.keyboard` | `true` | Make the expandable keyboard available |
| `controls.mouseButtons` | `true` | Show separate left/right mouse buttons |
| `controls.quickKeys` | `["ESC", "SPACE", "ENTER"]` | Up to sixteen guest keys, or `[]` |
| `controls.repeatKeys` | `[]` | Keys to repeat while held, after 350 ms then at 10 Hz, for games that move once per key press |
| `controls.repeatReleaseKeys` | `[]` | Touch keys to repeat as release/press cycles at the same rate, for games that act on key release |
| `controls.joystick` | `true` | Optional mouse stick menu; `false` hides it; an object shows a configured stick immediately |
| `controls.dpad` | `false` | `true` or an object enables a directional pad |
| `controls.look` | `false` | `true` or an object enables a mouse aiming stick |
| `controls.layout` | `"panel"` | `"panel"` beside/below the game, or `"overlay"` over the full game area |
| `controls.pointer` | `"drag"` | `"drag"` holds left click with relative motion, `"move"` moves without clicking, `"direct"` taps at the finger, `"off"` disables screen touch |
| `controls.touch` | `"auto"` | `"press"`, `"tap"`, `"hold-drag"`, `"tap-hold"`, `"move"`; see gestures below |
| `controls.touchClick` | `true` | Initial touch-click switch state; `false` starts in Move only mode while separate mouse actions still work |
| `controls.tapDuration` | `0` | Minimum tap hold in milliseconds, `0` to `250`; use `60` for games that sample held mouse buttons on timer ticks |
| `controls.toolbar` | `"auto"` | Menu/control cluster: `"auto"` or a corner, such as `"top-right"` |
| `controls.keyboardPosition` | `"auto"` | Expanded keyboard: `"auto"`, `"bottom"`, `"left"`, `"right"` |
| `controls.cursor` | Centered, bounded by the framebuffer | Guest start `x`, `y`, inclusive `minX`, `minY`, `maxX`, `maxY`, and `bounds`: `"clamp"` or `"reject"` |
| `controls.cursor.desktopScale` | Unset | Physical browser mouse calibration: guest pixels per PS/2 unit, `0.5`, `1` or `2` |
| `controls.cursor.desktopFractional` | `false` | The calibrated guest keeps fractional cursor positions, as LCOMaze does |
| `controls.pads` | `[]` | Up to two directional pads, sticks or steering wheels |
| `controls.actions` | `[]` | Up to eight named key, mouse or wheel actions, at most four per corner |

### Direct touch

`"pointer": "direct"` taps where the finger lands and drags from there. Each tap
moves before pressing; a second finger cannot take control. Browser clicks use game
coordinates; native mice stay relative.

```json
{
  "mobile": {
    "controls": {
      "pointer": "direct",
      "cursor": { "x": 400, "y": 300, "maxX": 799, "maxY": 599 }
    }
  }
}
```

Settings must match the game. This 800×600 example starts at `(400, 300)` and clamps
to `(0, 0)` through `(799, 599)`. Start and maxima default to `-1`; minima to zero.
Use `"bounds": "reject"` when the guest ignores out-of-bounds deltas. Axes are
independent. Sushi Slash clamps to `800, 600`; its preset and PvZ's need no C
changes.

Direct mapping follows delivered packets, including edge clamps and other controls,
and needs one cursor pixel per mouse delta with no resets, wrapping or acceleration;
otherwise use `"drag"` or `"move"`. Verify taps at each edge and after switching
devices.

These settings affect touch only; cancellation, backgrounding and closing controls
release input.

| Setting | Behavior |
| --- | --- |
| `"touch": "tap"` | Finger moves without pressing; release clicks only within eight screen units of the start. Dragging away and back, cancellation and a concurrent button hold do not click. |
| `"touch": "press"` | Press on touch; use for drawing, slashing or dragging objects. |
| `"touch": "move"` | No click; separate mouse buttons provide all clicks. |
| `"tapDuration": 60` | Holds synthesized taps and quick mouse-action taps across timer ticks, for games that sample buttons once per frame. Longer holds release on lift. |
| `"touch": "hold-drag"` | Movement stays immediate without selecting on a swipe. Hold 180 ms to press, then drag and lift. Moving over eight units first stays hover-only; a stationary tap clicks. |
| `"touch": "tap-hold"` | Older behavior: alternates press and release across two taps. |
| `"touchClick": false` | Touch moves only; named mouse actions discover, flag or select. The pointer switch re-enables clicks per `touch`. |
| `"touch": "move"` | Keeps clicks off and hides the pointer switch. |

### Movement and aiming

- Joystick: `mode` (`arrows`, `wasd`, `mouse`) and `side`.
- Dpad: `keys` (`arrows`, `wasd`) and `side`.
- Look stick: `side` and `sensitivity` from `0.1` to `5`, default `1`.

Movement defaults left, aiming right. One pad per side; directional or look pads
hide the joystick menu. `"joystick": true` with another pad shows a mouse stick on
the free side.

```json
{
  "name": "Move and aim",
  "mobile": {
    "orientation": "landscape",
    "fullscreen": true,
    "controls": {
      "layout": "overlay",
      "keyboard": false,
      "quickKeys": ["ESC", "SPACE"],
      "pointer": "move",
      "joystick": { "mode": "wasd", "side": "left" },
      "look": { "side": "right", "sensitivity": 1.2 }
    }
  }
}
```

Replace `joystick` with `false` and add
`"dpad": { "keys": "arrows", "side": "left" }` for buttons. Pads support diagonals;
fingers hold independent controls, so you can move, aim and click together.
Cancellation or backgrounding releases input.

Games that move once per key make can set `"repeatKeys": ["W", "A", "S", "D"]`; keys
repeat after a short hold. Releasing the last finger or cancelling stops repeats;
leave it empty for held-key games. Use `repeatReleaseKeys` for release-driven games;
a tap still acts once. Physical keyboards are unchanged, and a key cannot be in both
lists.

### Custom and two-player controls

Use `pads` for players or custom directions, and `actions` for named thumb buttons.
`quickKeys` stay in the toolbar for menus.

```json
{
  "mobile": {
    "controls": {
      "layout": "overlay",
      "keyboard": false,
      "mouseButtons": false,
      "pointer": "off",
      "quickKeys": ["ESC", "ENTER"],
      "pads": [
        { "side": "left", "mode": "wasd", "style": "dpad" },
        { "side": "right", "mode": "keypad", "style": "dpad" }
      ],
      "actions": [
        { "side": "left", "label": "P1 Bomb", "key": "SPACE" },
        { "side": "right", "label": "P2 Bomb", "key": "KP0" }
      ]
    }
  }
}
```

Modes: `arrows`, `wasd`, `keypad` (8/4/2/6), `mouse`; styles: `dpad`, `stick`,
`wheel`, with `stick` for mouse. Map directions with
`"keys": {"up":"W","left":"A","down":"S","right":"D"}`. Explicit pads hide the
joystick menu. Mouse sticks take `sensitivity` from `0.1` to `10` for coarse
text-mode games.

Each action needs a `side` or `position`, a label up to 16 characters, and one of
`key`, `mouseButton` (`left`, `right`, `middle`) or `wheel` (nonzero, -127 to 127).
For example, `{"side":"right","label":"Slow time","wheel":2}` sends two steps per
tap; IntelliMouse must be enabled. Buttons hold while touched, including modifiers.
Positions: `top-left`, `top-right`, `bottom-left`, `bottom-right`; `position` wins
over `side`. `axes` removes unused directions.

```json
{
  "mobile": { "controls": {
    "toolbar": "top-right",
    "keyboardPosition": "auto",
    "quickKeys": [],
    "dpad": { "side": "left", "keys": "arrows", "axes": "horizontal" },
    "actions": [
      { "position": "top-right", "label": "Menu", "key": "ESC" },
      { "position": "bottom-right", "label": "Fire", "key": "SPACE" }
    ]
  } }
}
```

Overlays put the toolbar top right by default; panels keep it inside. The keyboard
reserves space beside or below the game. Auto placement uses a side in landscape and
the bottom in portrait, with a fallback on narrow screens. The pointer icon
highlights while touch clicks are on; tap it for **Move only**. Profiles that never
click with touch omit the switch.

Quick keys use guest names: `A` to `Z`, digits, `ESC`, `SPACE`, `ENTER`, `UP`,
`DOWN`, `LEFT`, `RIGHT`, `TAB`, `BACKSPACE`, `LSHIFT`, `LCTRL`, `LALT`, `F1` to
`F12`, `HOME`, `END`, `PAGEUP`, `PAGEDOWN`, `DELETE`, `INSERT`, `KP_ENTER` and `KP0`
to `KP9`. Actions also accept `MINUS`, `EQUAL`, `LBRACKET`, `RBRACKET`,
`BACKSLASH`, `SEMICOLON`, `APOSTROPHE`, `BACKQUOTE`, `COMMA`, `PERIOD` and `SLASH`.
Match the original game's layout. Keypad directions send single-byte codes; arrows
keep extended codes.

Android requests orientation and fullscreen directly; browsers may require a
gesture. The fullscreen button requests both; unsupported orientation locking shows
a rotation hint. Desktop keeps the game-only view.

### Driving controls

Use a wheel for held left/right steering and pedals for acceleration and braking:

```json
{
  "mobile": { "controls": {
    "layout": "overlay",
    "joystick": false,
    "pads": [
      { "side": "left", "mode": "wasd", "style": "wheel",
        "maxAngle": 135, "deadZone": 8 }
    ],
    "actions": [
      { "position": "bottom-right", "label": "Brake / reverse",
        "key": "S", "icon": "brake" },
      { "position": "bottom-right", "label": "Accelerate",
        "key": "W", "icon": "accelerator" }
    ]
  } }
}
```

Drag the rim to turn. The wheel centers on release and can be held with a pedal;
outside its dead zone it sends held left/right keys, with no analog steering.
`maxAngle` is 45 to 270 degrees (default 135); `deadZone` is 0 to 30 (default 8).
Wheels use the horizontal axis; custom `keys` need `left` and `right`. Modes are
`arrows`, `wasd` or `keypad`.

`icon` accepts `accelerator`, `brake` or `reverse` and keeps the label. Match keys
to the game: MINIX Grand Prix brakes and reverses with S; lane games with automatic
acceleration keep directional controls.

## Colors and font

```json
{
  "mobile": {
    "theme": {
      "accent": "#ff8844",
      "keyboardAccent": "#77cc99",
      "button": "#203040",
      "text": "#f2f5ff",
      "font": "mono"
    }
  }
}
```

Use `#RRGGBB`. `accent` styles sticks and held buttons; `keyboardAccent` defaults to
`accent`. `button` and `text` style resting controls. Fonts are `sans` (default) and
`mono`. Native exports bundle glyphs, so no Internet is needed. Choose contrasting
colors.

## Source layouts and desktop bundles

Build settings use exporter paths and semantics:

```json
{
  "name": "My game",
  "build": {
    "sources": ["src/main.c", "src/game.c"],
    "include": ["include"],
    "assets": ["assets"],
    "arguments": ["--practice"]
  }
}
```

Discovery and assets follow the [browser exporter](./browser#your-project);
`"assets": []` excludes automatic assets. Minix Makefiles can set `"assetRoot"`,
`"assets"` and `"libraries"`, and explicit sources override discovery. LCF/Minix
imports keep the compatibility compiler and entry point, even without a Makefile;
other lists use the native SDK. Keep headers in `build.include`.

`build.cStandard` selects `gnu89`, `gnu99`, `gnu11` (default) or `gnu17` for
compatibility builds, browser exports and native objects including Android. SDK and
runtime keep their own standard. A Minix 3.1.8 project using GNU89 inline rules sets
`{"build": {"cStandard": "gnu89"}}`. Exporters honor `--config`.

Configured arguments begin at `argv[1]`. Native exports use `./<PROG>` as `argv[0]`
for Minix Makefiles and `./game` for native sources, regardless of display name.
Relative asset paths use the exported project's working directory.

Desktop bundles use the configured name and `icon.png` from `bundle.json`:

```sh
lcom-ng bundle . --program build/my-game --format dir --output dist/desktop
```

`--name`, `--icon` and `--config` also work for bundles.

## Repository examples

Each example has `examples/<name>/lcom.json`; single-file examples keep their C file
beside it and select it with `build.sources`:

```sh
lcom-ng web examples/word_processor
lcom-ng export android examples/breakout_demo
lcom-ng web examples/flappy
```

For Sushi, copy its `lcom.json` and `icon.png` into `proj/`, then export. For PvZ,
copy its profile and icon and add the asset-root settings above.

Editor completion uses `sdk/export/lcom.schema.json`; add a local `$schema` path
when supported.

### Runtime environment

`build.environment` sets string variables before browser, Android and desktop app
exports start. A game that never enables PS/2 reporting can opt in without source
changes:

```json
{"build": {"environment": {"LCOM_COMPAT_MOUSE_REPORTING": "1"}}}
```

If a frozen game unsubscribes before disabling reporting, a touch release can leave
mouse bytes ahead of keyboard input. For verified games, also set
`LCOM_COMPAT_MOUSE_IRQ_ONLY`; input then arrives only while the game subscribes.
Leave it unset for polling games.

Legacy builds use signed `char`, matching x86 Minix on Android ARM. An explicit
`-funsigned-char` still wins.

A fixed-framebuffer game can select its address without source changes:

```json
{"build": {"environment": {"LCOM_VBE_PHYSICAL_BASE": "0xF0000000"}}}
```

The default is `0xE0000000`. VBE BIOS calls report the selected address, which owns
the only video aperture. It must be page aligned, fit the 16 MiB aperture in 32-bit
physical space, and avoid low memory and the audio buffer. Invalid values stop
initialization. For CLI runs, set the variable for `run`, `replay` or `run-pair`.

If the framebuffer overlaps the default audio buffer at `0xD0000000`, relocate the
2 MiB PCM window:

```json
{"build": {"environment": {
  "LCOM_VBE_PHYSICAL_BASE": "0xD0000000",
  "LCOM_AC97_PCM_BASE": "0xC0000000"
}}}
```

The page-aligned windows must be disjoint and outside low memory.
`lcom_ac97_get_buffer` reports the PCM address; AC97 ports are unchanged. Each
device owns only its window, with no alias. Omitting both keeps the defaults.

### Legacy save-file paths

Some Minix games open hard-coded home paths. Map them to exported assets:

```json
{"build": {"pathAliases": {"/home/lcom/labs/my-project": "."}}}
```

Keys are explicit absolute or relative POSIX prefixes; `home/lcom/game/assets`
works without a slash. Values are existing directories relative to
`build.assetRoot`, or the project when no root is set.

- The SDK applies the longest matching prefix when the original `access`, `open`,
  `fopen`, `remove` or `rename` path is missing or denied by a host directory such
  as `/tmp`; successful calls win.
- Denied paths need an explicit alias; a failed alias preserves the original error.
- Repeated `/` is allowed; a prefix matches whole components, so `home/lcom/game`
  does not match `home/lcom/game2`.
- Absolute prefixes relocate literal `#include` paths through Clang's VFS overlay.
  Android copies the mapped header closure, so exports do not need the original
  project.
- Clang is required only for aliases with absolute includes; desktop GCC builds use
  `--cc clang`.
- Relative aliases stay runtime-only; macro-built includes are out of scope.

Android keeps saved files between launches; browser writes last for the session.
Include initial save assets. Browser and Android configure aliases automatically;
plain `compat build` does not.

### Small keyboard layouts

For two actions, hide the expandable keyboard and list only those keys. Quick keys
wrap and support simultaneous holds; enable a pad only when needed.

```json
{"mobile": {"controls": {"keyboard": false, "joystick": false,
  "mouseButtons": false, "pointer": "off", "quickKeys": ["SPACE", "ESC"]}}}
```

### Desktop mouse calibration

For a verified relative cursor with inclusive bounds and no warps, `desktopScale`
sets guest pixels per PS/2 unit. Memory Match starts at `(0, 0)`, clamps to
`(799, 599)` and halves deltas:

```json
"cursor": { "x": 0, "y": 0, "maxX": 799, "maxY": 599, "desktopScale": 0.5 }
```

Browsers preserve position across focus release and canvas re-entry. Half speed
emits even deltas; double speed keeps sub-step travel. Set `desktopFractional: true`
when the guest stores fractions. Tracking follows delivered packets, keeping touch
and mouse aligned. Leave it unset for cursors that warp, wrap or accelerate.

### Legacy C callback signatures

Older projects may assign callbacks with omitted parameters to function pointers
taking arguments. Native Minix allows this; WebAssembly checks exact signatures.
If a browser export stops with a mismatch, enable this without editing sources:

```json
{"build": {"emulateFunctionPointerCasts": true}}
```

This affects legacy Minix browser exports only and adds callback overhead; leave it
unset when signatures match.
