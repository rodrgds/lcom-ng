# Reactive graphical applications

A graphical program is usually an event loop:

```text
while running:
  wait for timer/input events
  update application state
  draw current state
  present frame
```

Do not draw randomly from many places. Keep a clear state model and a rendering
pass. Input changes state. Time advances state. Rendering reads state and draws a
frame. This is a correctness issue once a project has menus, animations, moving
objects, text, sound, and replay scripts.

## A useful separation

Device helpers own scancodes, mouse packets, timer ticks, framebuffer writes, and
audio buffers. Application state owns menus, entities, cursors, documents, or
song patterns. The renderer draws that state. Replay and test code provide
deterministic input timelines.

When responsibilities mix, every change becomes risky. A keyboard shortcut
should not know how a sprite is clipped. A replay script should not depend on
human typing speed.

## Why this matters

The separation makes projects easier to test, record, debug, and divide among
teammates. It also makes demos stable: a reactive program should run from live
input or a scripted trace and produce the same visible behavior.
