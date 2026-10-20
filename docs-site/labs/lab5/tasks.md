# Lab 5 tasks

Work in `labs/graphics/`. Build a small graphics library safe enough for final
projects. A function that works only for one hardcoded mode or one on-screen
rectangle is not enough.

## Requested functions

- `int video_set_mode(uint16_t mode)`
- `int video_map_framebuffer(void)`
- `int video_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)`
- `int video_draw_xpm(const char *const *xpm, int16_t x, int16_t y)`
- `int video_present(void)`

## Guided gaps

Store mode information, decide repeated-setup behavior, clip before writing
pixels, keep color packing clear, and parse enough XPM for course sprites. Project
debugging often starts by asking whether the framebuffer helper or application
state is wrong.

> [!IMPORTANT]
> Drawing past the framebuffer is memory corruption. Clip rectangles and sprites
> before writing.

## Common mistakes

Assuming pitch equals visible row width, forgetting to map before drawing,
writing colors in the wrong byte order, and ignoring negative sprite coordinates.
If a rectangle works at the top-left but not near the right edge, inspect pitch
and clipping first.

Next: [checks and references](/labs/lab5/check).
