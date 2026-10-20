# Pixel Studio

A compact sprite editor with a 16 × 16 canvas, named tools, a palette and a live
preview.

## Run it

Build examples with `devenv shell -- lcom-ng-build`, then:

```sh
lcom-ng run -- build/examples/pixel_studio build/pixel_studio_sprite.h
```

## Controls

| Control | Action |
| --- | --- |
| B / P / E / F / I | Pencil / eraser / fill / eyedropper |
| Mouse drag / right-click | Paint / pick canvas color |
| Arrows + Enter / Space | Move pixel cursor and apply tool |
| Delete | Erase the cursor pixel |
| [ / ] | Previous / next palette color |
| X / V | Flip horizontally / vertically |
| 1 / 2 / 3 or - / + | Zoom |
| G | Toggle grid |
| Cmd/Ctrl+Z / Cmd/Ctrl+Shift+Z (Ctrl+Y) | Undo / redo |
| Cmd/Ctrl+S / S / F2 | Export reusable C header |
| C / F1 | Clear / shortcuts |

## In the app

<ExampleGallery app="pixel-studio" />

## What it demonstrates

PS/2 gestures, stroke interpolation, flood fill, bounded undo history, RTC
timestamps and C asset generation. Export produces C data; it does not reopen
arbitrary image files.

Source: [examples/pixel_studio.c](https://github.com/rodrgds/lcom-ng/blob/main/examples/pixel_studio.c).

Refresh captures with the [screenshot workflow](./screenshots).
