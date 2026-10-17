# Build a game collection

Combine a class's projects into one Android APK and a browser library. The native
Android menu adapts its columns to the screen and text size. The web menu adapts to phones, tablets and desktop
screens. Games keep their original presentation and their own mobile controls.
Android's **Find a game** field filters titles as you type and keeps the query
when you return from playing.

## Try the student arcade

From the repository checkout:

```sh
lcom-ng web collections/arcade
lcom-ng export web collections/arcade --output dist/lcom-arcade-web
lcom-ng export android collections/arcade --output dist/lcom-arcade-android
```

The web output is a static folder. Serve it over HTTP or HTTPS. The Android output
contains `collection.apk` and its relocatable Gradle project. Android prerequisites
and architecture switches are the same as [single-game exports](../students/android).
The included arcade has pinned student games under `examples/community/`.
Their catalog records upstream URLs, revisions, control choices and verification.

## Make your own collection

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

Paths are relative to the collection folder and point to each game's build folder,
usually the folder containing its Makefile and `lcom.json`. `name` and `icon` are
optional. When supplied, the icon must be a square PNG of at least 48 pixels.
Folders must be unique; nested collections are rejected. Moving the collection
and games together preserves the folder list and game identities.

Configure each game's name, icon, description, preview screenshot, source/assets and controls in its own
[`lcom.json`](../students/project-config). Both exporters reuse that profile.
Collection-wide `--name` and `--icon` override the launcher identity. Source,
asset, argument and orientation switches belong in the individual game profiles.

Re-exporting to the same web output removes generated bundles for games you took
out of the list. Files you added yourself are preserved.

The web library uses square game icons, with larger featured games spread down
the grid. Each visit and Shuffle selects five random highlights from a curated `featuredPool`; resizing keeps the same selection. Desktop density fits nine
columns at 1280px. Games with the same `archetype` in their `lcom.json` form
compact 3×3 groups when nine unfeatured variants are available. Each group takes
the space of four ordinary tiles, and every variant keeps its own launch target.
Icons load near the viewport and reveal after decoding;
offscreen games wait until scrolling or Shuffle brings them into view.
Hover or keyboard focus expands a tile into gameplay screenshots with
its name, year when known, and approximate first-load download size. Screenshots
slide every few seconds; reduced motion keeps the first frame still. Touch opens
a game directly. Search filters the displayed grid by name or tag.
**Ctrl+K**, **Command+K** or **/** focuses the search field. The query and selected
filters survive game launches and reloads within the same tab. Enter or Arrow
Down moves into the visible game grid; arrow keys follow its actual layout.
**Shuffle** rearranges the current results. **Filters** combines genres, player
modes and school years in one panel. Choices within each group are alternatives;
groups combine. Active chips remove individual choices; **Clear all** resets
search and filters. Filtered results use regular tiles, keeping small selections
together. Surprise chooses from the same matching games.

**Surprise me** spins for about nine seconds, ticking as each game passes the
marker. It slows to a point within the chosen icon, then expands into the game's
screenshot, description, year, download size and public repository link, with a
**Play** button. Escape or a tap outside closes it. Reduced motion shows the
result immediately.
The logo and four actions have separate framed tiles. Phones keep these in two
columns above a three-column game grid. UI sounds play after
interaction and can be muted through the browser tab. No audio downloads or
third-party requests are needed.
A compact loading modal shows the game's `description` and measured progress.
Close cancels; failed downloads offer a retry or a return to the library.
Unused side margins show brighter playable game icons, with two columns when
space permits. Hover and keyboard focus use the library's gameplay previews.
Suggestions stay inside the free margins and clear of the game's controls.

## Web galleries and metadata

An optional `web.json` beside the collection manifest adds web-only presentation:

```json
{"version": 1, "metadata": "game-metadata.json", "gallery": "gallery.json", "favicon": "favicon.png"}
```

Metadata and gallery files use `{"version": 1, "games": {...}}`, keyed by exported
game IDs from `collection.json`. Metadata also accepts a `taxonomy` array of
`{id, label, facet}` definitions, where `facet` is `genre` or `players`.
Each game's `tags` selects those IDs. Describe modes that work in the exported
game, distinguishing same-device multiplayer from separate UART connections.

`academicYear` uses consecutive school years such as `2022/23`, or `null` when
unknown. Pair it with `academicYearSource`: `kind`, `confidence` (`explicit`,
`inferred` or `unknown`), `detail`, and optional HTTP `url` and ISO `date`.
Use original reports or README statements; otherwise use the game's actual
development history and mark the inference. The Arcade keeps labels plain, retains inference details in evidence tooltips,
and omits unknowns. Repository upload dates do not establish a school year.

`featured`, `featuredPool`, `repository`, and the earlier `year`, `yearSource` and `yearLabel`
fields remain supported. The Arcade retains repository creation timestamps as
separate provenance, with exact `createdAt` dates. A public `repository` HTTP URL
adds an icon link that opens separately from the game. GitHub repositories use
the GitHub mark, with an accessible repository name. The optional `favicon`
is a PNG or SVG inside the collection folder.

Gallery entries are arrays of up to 12 relative WebP paths inside the collection
folder. The exporter copies them under content hashes and loads them only when
a tile expands. A game without a gallery uses its configured preview or icon.
Download sizes include the game bundle and player, exclude gallery imagery,
and may shrink with caching or server compression.

The included Arcade keeps its capture sequences and provenance beside
`gallery.json`. Its capture script records actual browser canvas states; see
the collection's gallery instructions before regenerating images.

## Playing and returning

The mobile launcher fills the screen. On the web, tapping a game requests
fullscreen when the browser supports it; the player also fills the viewport
without that API. Touch players use **Games** to return to the library. It occupies
unused letterboxing when space permits and avoids the game's input controls.
The Home button stays visible for keyboard and touch players. After mouse capture,
a centered Shift/Esc hint appears after one second and fades away four seconds
later. Clicking Home or pressing **Shift+Escape** returns to the library.
Browser history works too.

Click the game to capture the physical mouse. Relative movement then stays in
the game even when the host cursor would reach a window edge. **Escape** releases
capture and reaches the game once. Click again to recapture. The upper-right
button toggles fullscreen.
When a game exits normally, its shared player automatically returns to the web
library. Standalone exports retain their restart behavior.

Android runs each game in a separate native process. **Games**, Android Back,
normal game exit and a game crash return to the existing launcher. Launching again
starts fresh game globals. Files written in that game's private storage survive
relaunch. Backgrounding pauses the embedded runtime.

Choose each game's touch behavior, movement pads and named action buttons in
its [`lcom.json`](../students/project-config). Board games can use a separate
control panel to keep every square touchable; local two-player games can give
each player their own pad and actions.

## Third-party projects

The repository's MIT license does not grant rights to the imported students'
source or artwork. Each game's `UPSTREAM.json` records its provenance and license
status; original notices are retained. Many public student repositories have no
explicit license. Obtain the authors' permission before redistributing those
projects or publishing a class APK or website.

Original student C and headers stay unchanged except recorded corrections for
reproduced defects. Each correction has a `SOURCE_PATCH.diff` and original and
patched hashes in `UPSTREAM.json`. Runtime adaptations live in the shared
compatibility SDK.
