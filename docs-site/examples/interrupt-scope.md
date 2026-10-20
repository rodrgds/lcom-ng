# Interrupt Scope

A device monitor with separate lanes for timer, keyboard, mouse, UART and RTC.
Independent scales keep timer traffic from obscuring other devices.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/interrupt_scope
```

## Controls

- `Space`: freeze or resume the graph. `L`: UART loopback.
- `F1`: help. `C` clears counters and history.

## In the app

<ExampleGallery app="interrupt-scope" />

## What it demonstrates

IRQ coalescing, event rates, PS/2 parsing, UART loopback, RTC polling and
bounded history buffers.

Source: [examples/interrupt_scope.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/interrupt_scope.c).

Refresh captures with the [screenshot workflow](./screenshots).
