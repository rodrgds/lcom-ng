# Accessing video memory

The linear framebuffer stores rows sequentially. The top-left pixel is near the
start of the mapped region; later rows follow earlier rows.

```c
uint8_t *pixel = framebuffer + y * pitch + x * bytes_per_pixel;
```

Before writing, confirm the framebuffer is mapped, `x` and `y` are inside the
visible area, and color packing matches the current mode. If any assumption is
false, the write becomes memory corruption instead of drawing.

## Color packing

A pixel is not always a `uint32_t` in the layout you want. Some modes use 15, 16,
24, or 32 bits per pixel, and channels can sit in different positions. lcom-ng
supports a limited set, but the habit is the same as a real driver: read the mode
information and pack colors for that format.

## Clipping

Clipping trims a draw operation to the visible screen. It is safer than writing
and hoping coordinates are valid. A rectangle that begins partly off-screen can
still have a visible portion, and a sprite with negative coordinates may still
draw the pixels that overlap.

Every final project needs clipping. Menus slide, sprites move, cursors reach
edges, and cameras show part of a larger world.
