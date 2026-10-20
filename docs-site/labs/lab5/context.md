# Lab 5 context: why video hardware looks this way

Text mode suits shells, but graphical applications need pixels. A graphics mode
turns the screen into a two-dimensional array, and the program writes the right
bytes for the right pixels.

Older PC graphics APIs exposed mode numbers and framebuffers. VBE standardized
enough of that behavior to select higher-resolution modes and access linear video
memory. lcom-ng keeps that shape because it is small enough to understand:
choose a mode, map memory, compute addresses, write colors, present.

Graphics at the lowest useful level begins with memory layout, not a large
engine. Sprites, text, UI panels, tile maps, particles, and games all become
writes into an image buffer or commands that produce one.

## What this lab teaches

Graphics mode selection, framebuffer mapping, pitch and pixel address
calculation, clipping, simple sprite formats, and event-driven rendering loops.
These support games, editors, trackers, drawing tools, and visualizers.
