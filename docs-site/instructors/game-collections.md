# Build a class Arcade

Combine class projects into one browser game library and, optionally, one Android
APK. Games keep their original presentation and mobile controls; launchers adapt
to phones, tablets, and desktop screens.

## Try the included Arcade

From the repository checkout:

```sh
lcom-ng web collections/arcade
lcom-ng export web collections/arcade --output dist/lcom-arcade-web
lcom-ng export android collections/arcade --output dist/lcom-arcade-android
```

The web output is static: serve it over HTTP or HTTPS; opening `index.html` from
the filesystem does not work. The Android output contains `collection.apk` and a
relocatable Gradle project; prerequisites match
[single-game exports](../students/android).

Pinned games live under `examples/community/`. Their catalog records upstream
URLs, revisions, controls, and verification.

## Create a collection

Create a folder with an `lcom.collection.json` file:

```json
{
  "version": 1,
  "name": "Our class arcade",
  "icon": "icon.png",
  "projects": [
    "../../students/alice/proj",
    "../../students/bob/src"
  ]
}
```

- Paths are relative to the collection folder and point to each game's build
  folder, usually the one with its Makefile and `lcom.json`.
- `name` and `icon` are optional. The icon must be a square PNG of at least 48
  pixels.
- Paths must be unique and must exist. Nested collections are rejected.
- Moving the collection and games together preserves folder list and identities.

Exports accept only `--name` and `--icon` as game-content overrides. Source,
asset, argument, and orientation settings belong in each game's `lcom.json`.
Re-exporting to the same web output removes bundles for removed games and
preserves files you added.

## Profile each game

Set the title, icon, description, preview, source and assets, and mobile controls
in the game's [`lcom.json`](../students/project-config). Both exporters reuse
that profile.

```json
{
  "name": "Sushi Slash",
  "icon": "icon.png",
  "description": "Slice the flying sushi before it falls.",
  "preview": "screenshots/gameplay.png"
}
```

- `description` appears in the library and loading screen.
- `preview` is the wide image used by expanded tiles. Without one, the library
  uses the game icon.
- `archetype` groups related games, such as `"Snake"` or `"Space Invaders"`.
  Nine unfeatured variants form a compact 3×3 group in the space of four tiles.
  Each variant keeps its own icon, search result, and launch target.

## Instructions and keyboard help

Add optional `instructions` to a game's `lcom.json` to show player help in a web
collection:

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
          { "action": "Move", "bindings": [{ "keys": ["A"] }, { "keys": ["Left"] }] },
          { "action": "Jump", "bindings": [{ "keys": ["Space"] }] },
          { "action": "Aim", "bindings": [{ "label": "Move the mouse" }] }
        ]
      }
    ]
  }
}
```

The collection shows these sections while the game downloads and pauses until
**Start game**. Players can reopen **Instructions** during play. Games without
the field open directly. Only web collections show instructions; native controls
and game code are unchanged.

- Keys in one binding form a chord, such as `["Ctrl", "S"]`.
- Separate bindings are alternatives, such as **A or Left**.
- Use `label` for mouse, touch, or gamepad input.
- Optional `platforms` filters a binding to `macos`, `windows`, `linux`, `ios`,
  or `android`.
- Key names are display labels, not guest key codes or input remappings.

Keep rows short and verify them against the build. The Arcade reserves **Right
Ctrl**, and **Right Option** on macOS, for releasing the mouse. **Shift+Escape**
releases capture, or returns to the collection when capture is already released.

Configuration allows up to 8 sections, 16 controls per section, 8 alternatives
per control, and 6 keys per chord. See
[instructions in the browser Arcade](../students/project-config#instructions-in-the-browser-arcade)
for every field.

## Web galleries and year metadata

An optional `web.json` beside the collection manifest adds web-only
presentation:

```json
{"version": 1, "metadata": "game-metadata.json", "gallery": "gallery.json", "favicon": "favicon.png"}
```

Metadata and gallery files use `{"version": 1, "games": {...}}`, keyed by the
exported game IDs in the generated `collection.json`. Export once and copy the
IDs from that catalog.

- `taxonomy` is an array of `{id, label, facet}`, where `facet` is `genre` or
  `players`. Each game's `tags` selects those IDs. Describe modes that work in
  the exported game, and distinguish same-device multiplayer from separate UART
  connections.
- `academicYear` uses consecutive school years such as `2022/23`, or `null` when
  unknown. Pair it with `academicYearSource`: `kind`, `confidence` (`explicit`,
  `inferred`, or `unknown`), `detail`, and optional HTTP `url` and ISO `date`.
- Use original reports or README statements when available; otherwise use the
  game's development history and mark the inference. Upload dates do not
  establish a school year.
- `repository` is a public HTTP URL that adds an icon link separate from the
  game. `favicon` is a PNG or SVG inside the collection folder.
- `analytics` optionally adds a cookieless Umami page-view tracker to the
  launcher shell with `{"src": "https://…/script.js", "websiteId": "…"}`. It is
  off by default and never runs inside an exported game.
- Gallery entries are arrays of up to 12 relative WebP paths. The exporter
  copies them under content hashes and loads them when a tile expands.
- `authors` lists the participants, only when evidence supports them: a local
  README, an upstream report, a source header, or a profile name for a single
  contributor. Use the published full name, never one derived from a username.
  A plain name string is enough; use `{name, email, url}` only for details the
  project or the person's public profile publishes, and pair any change with
  `creditsSource` (`kind`: `readme`, `project`, `commit`, `profile`, `unknown`;
  `detail`; optional HTTP `url`).
- `slug` is the game's fixed share address. `sdk/collection/share.py` derives a
  new one from the credited initials and the game name, such as `afg-ninjix`,
  and uses the name alone when no authors are evidenced. A published slug stays
  unchanged when later evidence adds a participant, so shared links keep
  working. Validation rejects duplicates, slugs whose name no longer matches the
  game, prefixes that use letters no credited person provides, and slugs over 64
  characters or outside `^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$`.

The Arcade shows plain labels, keeps inference details in tooltips, and omits
unknown years. Repository creation timestamps remain separate provenance with
exact `createdAt` dates. The earlier `year`, `yearSource`, and `yearLabel` fields
remain supported.

## Library behavior

- Square game icons, with larger featured tiles. Each visit and **Shuffle**
  selects five random highlights from `featuredPool`; resizing keeps the same
  selection.
- Search filters by name or tag; **Filters** combines genre, player mode, and
  school year.
- Hover or keyboard focus expands a tile into screenshots, name, known year, and
  approximate first-load size. Touch opens a game directly.
- The loading modal shows the description, category tags, and the game's
  participants below any authored controls. A credited person becomes a link to
  their published site or email. The upstream repository sits on the left of the
  modal footer when the metadata has one.
- **Ctrl+K**, **Command+K**, or **/** focuses search. Query and filters survive
  launches and reloads in the same tab. **Surprise me** chooses from current
  matches. Reduced motion keeps results still.

The included Arcade keeps capture sequences and provenance beside
`gallery.json`. Its script records real browser canvas states; read the gallery
instructions before regenerating images.

## Playing and returning

On the web, tapping a game requests fullscreen when supported. Click to capture
the mouse; relative movement stays in the game even at the window edge.
**Escape** releases capture and reaches the game once. Click again to recapture.
The upper-right button toggles fullscreen.

**Games**, browser history, a normal game exit, and **Shift+Escape** return to
the library. After mouse capture, a centered shortcut hint appears briefly.

The **Share** button copies the game's clean address,
`https://<host>/share/<slug>`, without the library hash. Touch layouts keep that
corner for the game's own controls and omit it. Opening one of those
links starts that game directly on a plain background, with no side
suggestions, and its **Games** button and normal exit return to the library.
The hosting Worker serves that route with the game's title, description and
preview, so the address is suitable for a résumé or a message.

Android runs each game in a separate native process. **Games**, Android Back,
normal game exit, and a crash return to the launcher. Relaunching starts fresh
game globals. Files in that game's private storage survive relaunch.
Backgrounding pauses the embedded runtime.

Choose each game's touch behavior, movement pads, and named action buttons in
its [`lcom.json`](../students/project-config). Board games can use a separate
control panel to keep every square touchable. Local two-player games can give
each player a pad and actions.

## Permissions and provenance

The repository's MIT license does not grant rights to imported students' source
or artwork. Each game's `UPSTREAM.json` records provenance and license status;
original notices are retained. Many public student repositories have no explicit
license. Obtain the authors' permission before redistributing those projects or
publishing a class APK or website.

Original student C and headers stay unchanged except recorded corrections for
reproduced defects. Each correction has a `SOURCE_PATCH.diff` and original and
patched hashes in `UPSTREAM.json`. Runtime adaptations live in the shared
compatibility SDK.

## Publish checklist

- Export into a clean output folder.
- Open a sample of games and check instructions, controls, sound, and **Games**.
- Check previews, tags, and year evidence on the tiles.
- Confirm permission for every game, screenshot, and gallery frame.
- Serve the web folder over HTTPS, and install the APK on a device before
  sharing.
