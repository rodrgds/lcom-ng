# Pixmaps and XPMs

XPM is a text-based image format that can be embedded directly in C. It is not a
modern asset pipeline, but it is good for teaching because the image data stays
inspectable.

For course sprites, support width, height, number of colors, characters per
pixel, the color table, transparent pixels, and pixel rows. A full image library
is unnecessary; a small parser keeps every part visible.

## Drawing sprites

Sprites are rectangles with per-pixel decisions. A transparent pixel must not
overwrite the destination. A sprite that begins partly off-screen should be
clipped, not rejected. Convert colors to the current mode's pixel format before
writing.

This bridges rectangles and applications. A game character, cursor, icon, tile,
note block, or caret can all be a small image positioned by application state.

> [!TIP]
> Keep XPM parsing separate from sprite drawing. Loading an image and drawing a
> loaded pixmap are different responsibilities.
