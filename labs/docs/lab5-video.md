# Lab 5: VBE Framebuffer And XPM

Generated folder: `labs/graphics/`

Lab 5 introduces graphics through VBE mode selection and linear framebuffer
mapping. Students draw directly into virtual video memory, compute pixel
offsets, handle different bytes-per-pixel modes, parse small XPM sprites, and
present frames through the runtime.

## Student Files

- `labs/graphics/include/video_lib.h`
- `labs/graphics/graphics_lab.c`

## Requested Functions

- `int video_set_mode(uint16_t mode)`
- `int video_map_framebuffer(void)`
- `int video_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)`
- `int video_draw_xpm(const char *const *xpm, int16_t x, int16_t y)`
- `int video_present(void)`

## What Students Learn

- VBE mode info and linear framebuffer mapping with `lcom_phys_map`.
- Pixel offset calculation from `x`, `y`, pitch, and bytes per pixel.
- Indexed and direct-color thinking, even when a lab focuses on one mode.
- Rectangle drawing, clipping, sprite parsing, and transparency.
- The difference between drawing into memory and presenting a frame.

## Test And Reference

```sh
lcom test graphics
lcom run --headless --dump-frame build/rectangle.ppm -- build/examples/vbe_rectangle
```
