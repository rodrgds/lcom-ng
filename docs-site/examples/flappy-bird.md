# Flappy Bird

The original pixel landscape and bird, with menus, help, pause and visible retry
actions. Mouse and keyboard reach the same game states.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/flappy_bird
```

## Controls

- Space, Up, or a mouse click flaps.
- P pauses; Space, Enter, P, or a click resumes.
- F1 or H shows instructions. R retries a crash. Escape quits.
- Up/Down or Tab select a menu option; Enter or click activates it.

## In the app

<ExampleGallery app="flappy-bird" />

## What it demonstrates

Input edges, PIT motion, collision detection, scoring, state transitions and
audio hooks. Release the key between flaps in deterministic scripts.

Source: [examples/flappy/](https://github.com/rodrgds/lcom-ng/tree/main/examples/flappy/).

Refresh captures with the [screenshot workflow](./screenshots).
