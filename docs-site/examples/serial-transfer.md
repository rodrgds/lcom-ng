# Serial Transfer

A file-transfer dashboard that shows connection, block delivery,
acknowledgement and verification.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run-pair build/examples/serial_transfer send \
  --right build/examples/serial_transfer receive
```

## Controls

- `Space`: keep the result open. `F1`: help.
- `Esc` or `Cmd/Ctrl+W`: close the peer.

## In the app

<ExampleGallery app="serial-transfer" />

## What it demonstrates

UART negotiation, framed messages, block indices, checksums, acknowledgements,
retry deadlines and terminal error states. The example transfers its built-in
16 × 16 indexed sprite. A progress bar at 100% does not mean delivery is verified;
the final CRC result does.

Source: [examples/serial_transfer.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/serial_transfer.c).

Refresh captures with the [screenshot workflow](./screenshots).
