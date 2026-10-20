# Drawing rectangles

Rectangles are the first useful drawing primitive: backgrounds, panels, buttons,
health bars, cursors, tile blocks, selection regions, and debug overlays.

Clip the rectangle to the screen, compute the row start once per row, write
pixels left to right, and present after the draw or batch. The important part is
correct bounds and address calculation before the loop writes memory.

## Building up slowly

Draw one pixel. Then a horizontal row. Then a small rectangle fully inside the
screen. Then test rectangles that touch or cross the edges. This localizes bugs:
one pixel tests address calculation, one row tests horizontal iteration, and a
rectangle tests row stepping through pitch.

## Common bugs

Off-by-one width or height loops, writing past the right edge, using
`width * bytes_per_pixel` instead of pitch, and presenting inside every inner
loop. That last bug may produce the correct image but teaches the wrong model.
Draw a frame, then present it.

> [!TIP]
> Draw a one-pixel rectangle, then a row, then a full rectangle. Each step tests
> one level of address calculation.
