# Ninjix

The larger game port included with lcom-ng. It came from an earlier Minix final
project and shows the application structure the labs lead to.

[Play in your browser](https://ninjix.rgo.pt/).

![Ninjix menu screenshot](/assets/screenshots/ninjix.png)

## What it exercises

Keyboard and mouse input, framebuffer rendering, menus and HUD text, profiling,
reusable device libraries, a UART multiplayer protocol, and paired execution.

The game separates input translation, rendering and game state, drives
progression with timers, and uses deterministic smoke scripts.

## Run it

```sh
lcom-ng run -- build/examples/ninjix
```

Multiplayer:

```sh
lcom-ng run-pair build/examples/ninjix --right build/examples/ninjix
```

In both windows, select Multiplayer, then choose opposite ATTACK and DEFEND
roles.

Fast deterministic smoke test:

```sh
lcom-ng run-pair --headless build/examples/ninjix \
  --right build/examples/ninjix --max-ticks 900
```

Longer authored demo:

```sh
lcom-ng run --headless --script scripts/ninjix_level1_demo.mlabscript \
  --dump-frame build/ninjix.ppm -- build/examples/ninjix
```

## Controls

- `1`-`4` choose a ninja type; click the lawn to place or select one.
- Right click cancels placement. Select a placed ninja to upgrade, sell or retarget.
- `Esc` pauses or resumes. `N` skips to the next level in singleplayer.
- Menus: Up/Down or W/S, Enter to confirm, Esc or Backspace to go back.

## Teaching notes

Use Ninjix as a final-project reference for state-machine size, the input and
application boundary, rendering helpers, serial protocols in a game, and keeping
a project demoable and testable. It is evidence of target scale, not the starter
template or minimum expected project.

## In the game

<ExampleGallery app="ninjix" />

Source: [examples/ninjix/](https://github.com/rodrgds/lcom-ng/tree/main/examples/ninjix/).

Refresh captures with the [screenshot workflow](./screenshots).
