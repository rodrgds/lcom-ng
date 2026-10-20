# The video card

A video card turns memory and commands into display output. Older educational
interfaces keep the visible model simple:

| Concept | Role |
| --- | --- |
| mode | resolution and pixel format |
| framebuffer | memory backing the visible image |
| pitch | bytes from one row start to the next |
| color format | how color channels are packed |
| present | make the prepared frame visible |

The mode describes dimensions and pixel format. The framebuffer stores pixels.
The pitch says how many bytes separate the start of one row from the next. These
values come from mode information, not guesses in drawing code.

## Text mode vs graphics mode

Text mode stores character cells. It is small and efficient for terminals because
firmware or hardware interprets bytes as glyphs and attributes. Graphics mode
stores pixels and gives applications control over every point on the screen.

## Why pitch exists

Rows may include padding for alignment or device constraints:

```text
offset = y * pitch + x * bytes_per_pixel
```

not:

```text
offset = (y * width + x) * bytes_per_pixel
```

If this formula is wrong, rectangles tear, sprites drift, and writes near the
right edge corrupt the next row.
