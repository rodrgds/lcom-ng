# Tile Forge

A tile editor with terrain and object layers, a map preview, validation and a playable Sokoban test. The tools belong beside the map, and play mode gives the board the available room.

## Run it

Build the examples with `devenv shell -- lcom-ng-build`, then run:

```sh
lcom-ng run -- build/examples/tilemap_editor build/tilemap-demo.mlmap
```

## Try a complete workflow

Paint walls, floors and goals, then place the player and crates. Validate the map, enter a playtest and solve it. Return to editing with Escape and save the map for a later session.

## Controls

| Control | Action |
| --- | --- |
| 1-5 | Floor / wall / goal / crate / player |
| Mouse drag / right-click | Paint / pick tile |
| Arrows + Space | Move tile cursor and paint |
| Delete | Erase the tile under the cursor |
| Enter / F5 | Switch editor / playtest |
| Arrows / WASD in playtest | Move player |
| U / R in playtest | Undo move / reset puzzle |
| Cmd/Ctrl+Z / Shift+Z | Undo / redo map edit |
| V / G | Validate / toggle grid |
| Cmd/Ctrl+S / S / F2 | Save map |
| Esc in playtest | Return to editor |
| F1 / ? button | Shortcuts |

Command shortcuts work on macOS; Ctrl shortcuts work on other hosts. `Esc` closes help first. `Cmd/Ctrl+W` closes the app.

## In the app

These are actual runtime captures. Open a screenshot for a larger view; use
Left and Right to browse, and Escape to close.

<ExampleGallery app="tilemap-editor" />

## What it demonstrates

Layered editing, map invariants, bounded parsing and serialization, keyboard movement and a separate playtest state.

A playable map needs a single player, matching crates and goals, and accessible puzzle space. Invalid drafts can be saved and corrected.

Source: [examples/tilemap_editor.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/tilemap_editor.c).

## Refresh these screenshots

```sh
devenv shell -- python3 scripts/capture_gallery.py --app tilemap-editor
```

See [the screenshot workflow](./screenshots) to add states or check freshness.
