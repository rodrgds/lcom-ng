# Ambient Aquarium

An animated habitat with controls for feeding, lighting, bubbles, pause and
sound. The view follows the RTC phase.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/ambient_aquarium
```

## Controls

- `Space` or `P`: pause or resume. `F`: feed. `L`: cycle lighting. `A`: follow the clock.
- `1`-`4`: dawn, day, sunset, night. `B`: bubbles. `M` or `S`: sound. `R`: reset.
- `F1`: help. The control bar also supports mouse clicks.

## In the app

<ExampleGallery app="ambient-aquarium" />

## What it demonstrates

Deterministic animation, RTC-driven color changes, mouse targets, PIT updates
and optional audio.

Source: [examples/ambient_aquarium.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/ambient_aquarium.c).

Refresh captures with the [screenshot workflow](./screenshots).
