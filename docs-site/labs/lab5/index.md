# Lab 5: video card in graphics mode

Generated folder: `labs/graphics/`

Lab 5 turns the screen into memory. You select a VBE-style graphics mode, map a
linear framebuffer, compute pixel addresses, draw rectangles, parse small XPM
sprites, and present frames.

The earlier device work now looks like an application platform: timers provide
rhythm, keyboard and mouse provide events, and the video device provides pixels.
A final project is mostly the careful composition of those pieces with a state
model.

Read [why video cards look like memory devices](/labs/lab5/context), then the
[video card model](/labs/lab5/video-card), [VBE mode setup](/labs/lab5/vbe), and
[video memory](/labs/lab5/vram). Draw [rectangles](/labs/lab5/rectangles) and
[XPM sprites](/labs/lab5/xpm), then build
[reactive graphical applications](/labs/lab5/reactive-apps). Complete the
[tasks](/labs/lab5/tasks), validate with the [checks](/labs/lab5/check), and
compare with [modern graphics stacks](/labs/lab5/modern).

> [!TIP]
> Start with one pixel address. Rectangles, sprites, and UI widgets all build on
> that calculation.
