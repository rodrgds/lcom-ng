# Lab 4: PS/2 mouse packets

Generated folder: `labs/mouse/`

Lab 4 extends the i8042 controller idea to the auxiliary PS/2 mouse. The mouse
reports packets containing button state, movement deltas, sign bits, and
overflow flags.

The change from Lab 3 is that one device event is now a group of bytes. If you
lose synchronization, every following byte can be misread until the parser finds
the start of a packet. That makes the mouse a compact but serious protocol
exercise.

Read [why pointing devices changed interfaces](/labs/lab4/context), then the
[mouse controller path](/labs/lab4/mouse-controller) and
[packet parsing rules](/labs/lab4/packets). Complete the
[tasks](/labs/lab4/tasks), validate with the [checks](/labs/lab4/check), and
compare with [modern pointer input](/labs/lab4/modern).

> [!TIP]
> Mouse input is the first lab where losing one byte can corrupt every following
> interpretation until you resynchronize.
