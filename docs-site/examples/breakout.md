# Breakout

A complete arcade round with a deliberate serve, lives, score, pause,
instructions and a clear round-end state.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/breakout_demo
```

## Controls

- Left / Right or A / D move the paddle.
- Space or Enter serves. Space or P pauses; R restarts.
- F1 or H opens help; Escape closes help, then closes the game.

## In the app

<ExampleGallery app="breakout" />

## What it demonstrates

Held-key movement, timer-driven physics, collision handling, score and life
state, pause and framebuffer drawing.

Source: [examples/breakout_demo.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/breakout_demo.c).

Refresh captures with the [screenshot workflow](./screenshots).
