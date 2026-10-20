# Pocket Composer

A four-track step sequencer with a piano-roll grid, transport, track mutes and a
visible playhead.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/music_maker build/pattern.mlpat
```

## Controls

| Control | Action |
| --- | --- |
| Space / P | Play or pause |
| Click / arrows + Enter | Write or toggle a note |
| 1-4 / Tab | Select instrument |
| M / track M button | Mute selected track |
| - / + or transport buttons | Change tempo |
| Backspace / Delete | Remove current note |
| C / R | Clear track / restore demo pattern |
| Cmd/Ctrl+S / S / F2 | Save pattern |
| Cmd/Ctrl+Z / Cmd/Ctrl+Shift+Z (Ctrl+Y) | Undo / redo |
| F1 | Shortcuts |

## In the app

<ExampleGallery app="music-maker" />

## What it demonstrates

PIT timing, keyboard and mouse handling, AC97 audio, PCM buffers, note
sequencing and pattern persistence. Pattern files store notes and tempo, not
recorded audio.

Source: [examples/music_maker.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/music_maker.c).

Refresh captures with the [screenshot workflow](./screenshots).
