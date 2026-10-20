# Lab 5 check

Run the graphics tests before relying on visual inspection. A framebuffer bug can
look like a design mistake, but the tests check mode setup, address calculation,
clipping, and dumped frame contents directly.

```sh
lcom-ng test graphics
lcom-ng run --headless --dump-frame build/rectangle.ppm -- build/examples/vbe_rectangle
```

## Discussion prompts

- Why is pitch not always `width * bytes_per_pixel`?
- What changes when you double-buffer?
- Which rendering helpers would you want before starting a game, editor,
  tracker, or visualizer?

## External reading

The LCOM [video card chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab5/pc-video-card.html),
[VBE chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab5/vbe.html),
and [reactive applications chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab5/reactive-graphical-applications.html)
are useful companions. For deeper reference, read OSDev on
[VESA video modes](https://wiki.osdev.org/VESA_Video_Modes), the
[VBE 2.0 specification](https://www.phatcode.net/res/221/files/vbe20.pdf), and
the [XPM overview](https://en.wikipedia.org/wiki/X_PixMap).
