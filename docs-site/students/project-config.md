# Configure your project

Add an optional **`lcom.json`** to your project folder. Web and Android exports
read it automatically. Projects without it still build and run. Normal desktop
execution keeps its existing input and presentation.

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

This gives the game a title and launcher icon, requests landscape and fullscreen,
and leaves the game clear of controls. Touching and dragging still moves its cursor
with the left mouse button held. Android Back sends Escape.

```sh
lcom-ng export android . --output dist/android
lcom-ng export web . --output dist/web
lcom-ng web .
```

Use a square PNG at least 48 pixels wide. A 1024×1024 image with the subject near
the center works well with Android's adaptive launcher masks. The web export uses
the icon for the browser tab, home-screen shortcut and web app manifest.

Paths in `lcom.json` are relative to the project. The icon path is relative to the
configuration file, including when you select another file with `--config`:

```sh
lcom-ng export web . --config profiles/touch.json
lcom-ng export android . --name 'Practice build' --orientation auto --icon other.png
```

Explicit CLI options override matching file settings. Repeated source, asset,
include and argument options replace their configured arrays. Unknown settings,
invalid values and missing files produce an error naming the setting. The file
can contain just `name`, just `mobile`, or any subset below. `version` is optional;
when supplied, use `1`.

## Web collection artwork

Add a short gameplay description and a screenshot for the browser library:

```json
{
  "name": "Sushi Slash",
  "description": "Slice the flying sushi before it falls.",
  "preview": "screenshots/gameplay.png"
}
```

`description` is optional, with up to 500 characters. `preview` is an optional
PNG of at least 48 pixels in each dimension, relative to the configuration file.
A gameplay capture around 800×600 works well. The library crops it to a wide tile
and puts the game name over the image. Without a preview, it uses the icon.
The loading screen shows the description and actual download progress, then
opens the game when its first frame appears. These fields do not alter gameplay
or native controls.

## Mobile controls

| Setting under `mobile` | Default | Choices |
| --- | --- | --- |
| `orientation` | `"auto"` | `"auto"`, `"landscape"`, `"portrait"` |
| `fullscreen` | `false` | Request fullscreen on mobile |
| `controls.keyboard` | `true` | Make the expandable keyboard available |
| `controls.mouseButtons` | `true` | Show separate left/right mouse buttons |
| `controls.quickKeys` | `["ESC", "SPACE", "ENTER"]` | Up to sixteen guest keys, or `[]` |
| `controls.repeatKeys` | `[]` | Keys to repeat while held, after 350 ms then at 10 Hz, for games that move once per key press |
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
| `controls.pads` | `[]` | Up to two directional pads, sticks or steering wheels |
| `controls.actions` | `[]` | Up to eight named key or mouse actions, at most four per corner |

### Direct touch

For a game with a cursor, use `"pointer": "direct"` to tap where your finger lands
and drag from there. Each tap moves to its target before pressing. A second
finger cannot take over the game cursor. Browser mouse clicks and hovering also
use game coordinates in this mode. Native physical mice retain relative motion.

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

The cursor settings must match the game's code. This example is for an 800×600
cursor starting at `(400, 300)` and clamped to `(0, 0)` through `(799, 599)`.
Start and maximum settings default to `-1`: centered start, or the final framebuffer pixel.
Minimums default to zero. Use `"bounds": "reject"` when the guest ignores an
out-of-bounds delta rather than clamping it. Each axis is tracked independently.
Sushi Slash clamps inclusively to `800, 600`, so its preset uses those bounds.
Both PvZ and Sushi Slash presets enable direct touch without changing their C sources.

Direct mapping follows the relative packets delivered to the game, including
edge clamps and movement from other controls. It requires one cursor pixel per
mouse delta and no independent cursor resets, wrapping or acceleration. Use
`"drag"` or `"move"` for games with those behaviors. Verify taps after dragging
against each edge and switching input devices before publishing a direct preset.

For point-and-click games, combine `"pointer": "direct"` with
`"touch": "tap"`. A finger moves the cursor without pressing. Releasing clicks
only if the finger stayed within eight screen units of its starting point.
Dragging away and back, cancellation and a concurrent explicit mouse-button hold
do not click. This lets players inspect pieces without selecting them. Use
`"touch": "press"` for drawing, slashing or dragging objects, and `"move"` when
separate buttons should provide all clicks. These settings affect touch only.

Some games check held mouse buttons once per frame instead of handling mouse
press edges. Set `"tapDuration": 60` to hold synthesized screen taps and quick
mouse-action button taps across those timer ticks. Longer button holds release
immediately on lift. Ordinary press/drag input and tap-hold latching retain their own lifetimes.
A new contact, cancellation, backgrounding or a right-click releases the pulse.

For drag-based boards, `"touch": "hold-drag"` keeps movement immediate without
selecting on a quick swipe. Hold still for 180 ms to press, then drag and lift
to place, all in one gesture. Moving more than eight screen units before that
hold keeps the whole gesture as hover-only. A quick stationary tap clicks.
The older `"tap-hold"` alternates press and release across two taps when that
interaction is preferred. Cancellation, backgrounding and closing the controls
release held input. Choose `"layout": "panel"` to keep controls off the board.

For boards where an accidental touch could reveal a cell or choose a card, set
`"touchClick": false`. Touch still moves the cursor, and separate named mouse
actions can discover, flag or select. Players can enable touch clicks with the
pointer switch. Keep the underlying `touch` policy, such as `"tap"`, to choose
what that switch enables. Use `"touch": "move"` when clicks should stay disabled
and the switch should be hidden.

### Movement and aiming

A configured joystick accepts `mode` (`"arrows"`, `"wasd"` or `"mouse"`) and
`side` (`"left"` or `"right"`). A directional pad accepts `keys` (`"arrows"` or
`"wasd"`) and `side`. A look stick accepts `side` and `sensitivity` from `0.1` to
`5`, default `1`. Movement defaults to the left; aiming defaults to the right.
Only one configured pad can occupy each side. Adding a directional or look pad
hides the default optional joystick menu. Explicit `"joystick": true` alongside
one other pad shows a mouse stick on the free side; use an object to choose its
mode and side yourself.

For movement on the left and independent mouse aiming on the right:

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

Replace `joystick` with `false` and add `"dpad": { "keys": "arrows", "side": "left" }`
for arrow buttons instead. Pads support diagonals. Fingers own controls
independently, so you can move, aim and hold a mouse button together. Releasing,
cancelling, rotating or backgrounding releases held input.

Games that move once per key make, rather than keeping a held movement state,
can set `"repeatKeys": ["W", "A", "S", "D"]`. Those keys repeat after a short
hold, so a movement pad can travel continuously. Releasing the last finger or
cancelling stops repeats. Leave this empty for games with their own held-key logic.

### Custom and two-player controls

Use `pads` for separate players or custom direction keys, and `actions` for
clearly named buttons near each thumb. `quickKeys` stay with the toolbar for
menu commands. For example:

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

Pad modes are `arrows`, `wasd`, `keypad` (8/4/2/6), and `mouse`. Styles are
`dpad`, `stick` and `wheel`; mouse mode requires `stick`. A pad can instead map all four
directions through `"keys": {"up":"W","left":"A","down":"S","right":"D"}`.
Only one pad can occupy each side, including the older joystick, look and dpad
settings. Explicit pads hide the optional default joystick menu.

Each action requires a `side` or `position`, a short `label` (up to 16 characters), and either
`key` or `mouseButton` (`left`, `right`, `middle`). Actions hold while touched,
including modifier keys. Multiple fingers can hold both pads and both players'
actions independently. Positions are `top-left`, `top-right`, `bottom-left` and
`bottom-right`. A legacy `side` places the action by that thumb; an explicit
`position` takes precedence. Use `axes: "horizontal"` or `"vertical"` on pads,
the legacy dpad or a configured joystick to remove unused directions. WASD and
keypad controls display their actual keys, while arrow controls display arrows.


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

Overlay toolbars default to the top right; panel layouts keep their toolbar in
the panel. The expanded keyboard reserves space beside or below the game.
Automatic placement uses a side in landscape and the bottom in portrait, with
a bottom fallback on narrow displays. The pointer icon is highlighted while touch
clicks are enabled. Tap it for “Move only”; separate mouse buttons still work.
Hover-only profiles omit this switch. Top utility keys size to their labels,
while bottom thumb actions keep larger targets.

Quick keys use guest names: letters `A` to `Z`, digits, `ESC`, `SPACE`, `ENTER`,
`UP`, `DOWN`, `LEFT`, `RIGHT`, `TAB`, `BACKSPACE`, `LSHIFT`, `LCTRL`, `LALT`, `F1`
to `F12`, `HOME`, `END`, `PAGEUP`, `PAGEDOWN`, `DELETE`, `INSERT`, `KP_ENTER`
and `KP0` to `KP9`. Keypad directions produce their original single-byte codes;
arrow keys retain extended codes. Arrow and
mouse controls use symbols; web controls retain accessible names and tooltips.

Android requests the selected orientation and fullscreen directly. Browsers can
require a player gesture for fullscreen and orientation lock. The fullscreen
button requests both; unsupported orientation locking shows a rotation hint
instead. Desktop browsers keep the game-only view. Browser support and device
settings ultimately control whether fullscreen or orientation requests succeed.

### Driving controls

Use a steering wheel for games with held left/right steering, and separate
pedal actions for acceleration and braking. For a game controlled with WASD:

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

Drag around the rim to turn. The wheel returns to centre on release and can be
held with a pedal using another finger. It sends ordinary held left/right keys
outside its dead zone. It does not add analog steering to the guest game.
`maxAngle` is the rotation limit in each direction, from 45 to 270 degrees,
default 135. `deadZone` is from 0 to 30 degrees, default 8. These settings apply
only to wheels, which always use the horizontal axis. Custom wheel `keys` need
`left` and `right` mappings. Wheel modes can be `arrows`, `wasd` or `keypad`.

Action `icon` accepts `accelerator`, `brake` or `reverse` to draw a pedal while
keeping its label. Match each key and label to the game's behavior. MINIX Grand
Prix uses S to brake through zero and then reverse. Lane-changing games with
automatic acceleration should keep their directional controls.


## Colors and font

These settings style the controls, leaving game artwork and text unchanged:

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

Use `#RRGGBB` colors. `accent` styles sticks and held buttons; `keyboardAccent`
styles held keyboard keys and defaults to `accent`. `button` and `text` style
resting controls. Font choices are `"sans"` (default) and `"mono"`. Native exports bundle their glyphs. Players do not need to install fonts
or connect to the Internet. Choose contrasting text and button colors.

## Source layouts and desktop bundles

Build settings are optional too. They use the same paths and semantics as the
exporter's command-line options:

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

Sources are discovered automatically when `sources` is absent. An `assets/`
folder is included automatically when `assets` is absent. Use `"assets": []` to
exclude it explicitly. Original Minix
Makefiles can also set `"assetRoot": ".."`, `"assets": ["proj/assets"]` and
`"libraries": ["name=source.c,other.c"]`. Explicit source lists select native
C projects instead of Makefile discovery.

Desktop bundles use the configured name and include `icon.png`, referenced in
`bundle.json`. They retain the existing executable/directory format:

```sh
lcom-ng bundle . --program build/my-game --format dir --output dist/desktop
```

`--name`, `--icon` and `--config` also work for bundles. The mobile settings do
not change the normal desktop runtime.

## Repository examples

Each example has a profile in `examples/<name>/lcom.json`. Single-file examples
keep their C file beside that folder and select it with `build.sources`, so
existing desktop build commands stay valid:

```sh
lcom-ng web examples/word_processor
lcom-ng export android examples/breakout_demo
lcom-ng web examples/flappy
```

Word Processor, Music Maker, Pixel Studio and Tilemap Editor expose a keyboard. Breakout uses a left directional pad;
Flappy exposes Space and Escape. PvZ, Ninjix and Sushi Slash keep mobile gameplay
clear of control panels. The Sushi preset in `examples/sushi-slash/` accompanies
the separately obtained original project: copy `lcom.json` and `icon.png` into
its `proj/` folder, then export that folder. For original PvZ sources, copy its
profile and icon into `proj/` and add the Minix asset-root settings above.

Editor completion is available through the bundled JSON schema at
`sdk/export/lcom.schema.json`. Add a `$schema` path relative to your `lcom.json`
when your editor supports local JSON schemas.

### Runtime environment

`build.environment` sets string environment variables before the game starts in
browser and Android exports. For a legacy game that never enables PS/2 mouse
reporting, opt into the compatibility override without changing its source:

```json
{"build": {"environment": {"LCOM_COMPAT_MOUSE_REPORTING": "1"}}}
```

If a frozen game unsubscribes its mouse before disabling reporting, a touch
release can leave unread mouse bytes ahead of keyboard input. For games verified
to have this transition, set `"LCOM_COMPAT_MOUSE_IRQ_ONLY": "1"` in the same
environment object. Embedded players then deliver mouse input only while the game
subscribes to its interrupt. Leave this unset for games that poll the mouse.

Legacy builds default to signed `char`, matching the original x86 Minix target
on Android ARM too. An explicit `-funsigned-char` in the original Makefile still wins.

### Legacy save-file paths

Some Minix games open files under a hard-coded home directory. Map that directory
to a folder in the exported assets without editing the game:

```json
{"build": {"pathAliases": {"/home/lcom/labs/my-project": "."}}}
```

Alias keys are absolute POSIX directory prefixes. Values are existing directories
relative to `build.assetRoot`, or the project folder when no asset root is set.
The compatibility SDK applies the longest matching directory prefix when the
original `fopen`, `remove` or `rename` path does not exist. Android keeps each game's saved files in
private storage between launches. Browser writes last for the current game
session. Include any initial save-file assets the game expects.

Browser and Android exports configure these aliases automatically. Plain desktop
`compat build` does not read this setting.

### Small keyboard layouts

Quick keys also accept `KP_ENTER` for games that require the keypad Enter key.

For a game with only two actions, hide the expandable keyboard and list only
those keys. Quick keys wrap on smaller screens and support simultaneous holds:

```json
{"mobile": {"controls": {"keyboard": false, "joystick": false,
  "mouseButtons": false, "pointer": "off", "quickKeys": ["SPACE", "ESC"]}}}
```

Only enable a directional pad or stick when the game uses one.

### Desktop mouse calibration

For a relative-pointer game with a verified cursor start, inclusive bounds and
no independent cursor warps, set `cursor.desktopScale` to the number of guest
pixels moved per PS/2 unit. For example, Memory Match starts at `(0, 0)`, clamps
to `(799, 599)` and divides each packet delta by two:

```json
"cursor": { "x": 0, "y": 0, "maxX": 799, "maxY": 599, "desktopScale": 0.5 }
```

The browser preserves this position across focus release and canvas re-entry.
Half-speed mapping emits even raw deltas; double-speed mapping retains sub-step
travel until a guest pixel step is representable. Touch gestures and native
physical mice retain their existing relative behavior. Keep this unset for
cursors that warp, wrap, accelerate or move independently through other inputs.
