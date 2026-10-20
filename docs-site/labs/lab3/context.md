# Lab 3 context: the PC keyboard

The keyboard still shapes how systems are controlled. Shortcuts, shells, games,
recovery menus, BIOS setup, and accessibility flows all depend on reliable
keyboard input.

At this level, a keyboard does not send letters. It sends scancode bytes. A
program must decide whether a byte is available, whether the controller reported
an error, whether the byte starts an extended sequence, and whether the key was
pressed or released. Only then should higher-level code decide whether the event
means "type A", "move left", "open a menu", or "exit".

The lab asks you to print scancodes before building applications because the
byte stream has to be trusted first. A parser that drops an extended prefix or
mistakes a break code for a make code makes every layer above it unreliable.

## Why this matters

Final projects need stable input. A wrong scancode layer breaks menus, controls,
editor shortcuts, and debug consoles. It also affects collaboration: one student
can work on menus while another works on movement, both relying on the same
keyboard library.
