# Plants vs. Zombies

The original student project, with its original menus, bitmap font, seed-selection grid and lawn HUD. The native example keeps the platform adapter needed to run outside Minix.

[Play in your browser](https://plantsvszombiesrafa.pages.dev/).

## Run it

Build the examples with `devenv shell -- lcom-ng-build`, then run:

```sh
lcom-ng run -- build/examples/pvz
```

## Try a complete workflow

Choose seeds, start the wave, select a seed slot and place a plant on the lawn. Collect sun and defend the house. Pause preserves the current game.

## Controls

- Use the original Start Game button, or Enter twice, to open plant selection.
- Pick six plants with arrows and Enter, or drag seeds into deck slots. Space starts the lawn.
- 1 to 6 selects a seed; arrows move the target; Enter or Space plants. Mouse selection and placement also work.
- Click suns to collect them. S selects the shovel; Escape or right click cancels a held action.
- Escape pauses. Enter resumes; Q returns to the menu while paused; Escape quits from pause.

## In the app

These are actual runtime captures. Open a screenshot for a larger view; use
Left and Right to browse, and Escape to close.

<ExampleGallery app="plants-vs-zombies" />

## What it demonstrates

Mouse targets, keyboard menus, sprite rendering, resource costs, timed enemies, projectiles and application state.

The demo uses the original project art and supported plant set. It is a course example rather than a complete commercial game.

Source: [examples/pvz/](https://github.com/rodrgds/lcom-ng/tree/main/examples/pvz/).

## Refresh these screenshots

```sh
devenv shell -- python3 scripts/capture_gallery.py --app plants-vs-zombies
```

See [the screenshot workflow](./screenshots) to add states or check freshness.
