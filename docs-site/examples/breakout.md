# Breakout

A complete arcade round with a deliberate serve, visible lives and score, pause, instructions and a clear next-life or round-end state.

## Run it

Build the examples with `devenv shell -- lcom-ng-build`, then run:

```sh
lcom-ng run -- build/examples/breakout_demo
```

## Try a complete workflow

Serve with Space or Enter, move the paddle, and return the ball into the brick wall. Each destroyed brick scores ten points. A missed ball costs a life and waits for another serve.

## Controls

- Left / Right or A / D move the paddle.
- Space or Enter serves. Space or P pauses; R restarts.
- F1 or H opens help; Escape closes help, then closes the game.

## In the app

These are actual runtime captures. Open a screenshot for a larger view; use
Left and Right to browse, and Escape to close.

<ExampleGallery app="breakout" />

## What it demonstrates

Held-key movement, timer-driven physics, collision handling, score and life state, pause and framebuffer drawing.

Help and pause freeze the round. Restart begins a fresh board and resets the score.

Source: [examples/breakout_demo.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/breakout_demo.c).

## Refresh these screenshots

```sh
devenv shell -- python3 scripts/capture_gallery.py --app breakout
```

See [the screenshot workflow](./screenshots) to add states or check freshness.
