# Focus Clock

A focus timer with separate focus and break choices, clear countdown numerals
and one start or pause action.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/pomodoro_clock
```

## Controls

- `Space`: start, pause or resume. `R`: reset.
- `F` / `B`: focus / break. `Tab`: switch session.
- `D`: quick demo. `M`: sound. `F1`: help.
- The Start, Reset and Switch buttons also work with the mouse.

## In the app

<ExampleGallery app="pomodoro-clock" />

## What it demonstrates

RTC wall time, monotonic PIT elapsed time, timer state transitions, input and
optional AC97 feedback. Wall-clock corrections do not change elapsed focus time.
Sound is available only when the audio device starts successfully.

Source: [examples/pomodoro_clock.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/pomodoro_clock.c).

Refresh captures with the [screenshot workflow](./screenshots).
