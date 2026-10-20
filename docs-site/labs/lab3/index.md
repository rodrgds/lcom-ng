# Lab 3: keyboard and i8042

Generated folder: `labs/kbd/`

Lab 3 moves from periodic events to human input. A key press is not a character;
it is a device event that may arrive as one or more scancode bytes. Characters
belong to text input, layouts, modifiers, and application policy. Scancodes
belong to the device stream.

You work below the text-input layer to see how a system learns that a physical
key changed state. That knowledge serves games, editors, shells, debug consoles,
and any project that needs responsive controls.

Read [why keyboards are controller streams](/labs/lab3/context), then the
[keyboard controller](/labs/lab3/keyboard) and
[scancode rules](/labs/lab3/scancodes). Compare
[interrupts and polling](/labs/lab3/sync), learn the
[KBC commands](/labs/lab3/kbc-commands) used by the lab, and combine keyboard and
timer events in [multiple devices](/labs/lab3/multiple-devices). Complete the
[tasks](/labs/lab3/tasks), then compare with
[modern keyboards](/labs/lab3/modern).

> [!TIP]
> Treat the keyboard as a byte stream first. Turn bytes into application actions
> only after the low-level parser is correct.
