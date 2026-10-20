# How it works today: graphics

Modern graphics stacks are much larger than VBE and linear framebuffer writes.
The CPU usually does not draw every pixel into visible display memory. It submits
commands, buffers, textures, and shaders to a GPU through a driver and graphics
API.

## What changed

Modern systems use GPU drivers, command buffers, shaders, compositors, window
systems, and APIs such as Vulkan, Metal, Direct3D, OpenGL, or WebGPU. The final
image may pass through multiple buffers. A compositor may combine windows, apply
scaling, and present according to display timing.

The stack exists because GPUs are powerful and parallel. Drawing every pixel on
the CPU would waste hardware that can transform vertices, shade pixels, sample
textures, and blend layers.

## What stayed useful

Framebuffer thinking still matters. Images are arrays of pixels, pitch and stride
exist in modern textures, clipping and coordinates still matter, double-buffering
and presentation remain core ideas, and rendering should be separated from
application state.

lcom-ng uses VBE-shaped graphics because it exposes those ideas without a full
GPU stack.
