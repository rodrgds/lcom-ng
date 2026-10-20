# Lab 4 context: why pointing devices matter

The mouse made direct manipulation practical. Windows, menus, drawing tools,
games, graphical editors, and desktop file managers depend on continuous
pointing input.

At the controller level, a mouse is a stream of packets. The format is small but
contains ideas that recur in protocols: synchronization bits, signed movement,
button state, overflow flags, and enable/disable commands. Unlike a keyboard
scancode, a mouse packet describes movement. The program decides how relative
deltas affect a cursor, camera, selection box, or game object.

## From keyboard to mouse

The mouse uses the same i8042-style controller, but its data model is richer. A
keyboard event can be one or two bytes. A mouse event is a packet whose bytes are
meaningful only as a group.

Lab 4 bridges input and graphics. You do not need a full pointer UI yet, but the
parser should be good enough for a final project. Keep the packet decoder
independent from screen coordinates so applications can choose their own
coordinate system, sensitivity, and clipping.
