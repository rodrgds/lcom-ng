# Lab 3 check

Run the focused keyboard tests before using the library from an interactive
example. They verify the scancode parser, error handling, interrupt path, polling
path, and timed-scan behavior with deterministic input.

```sh
lcom-ng test kbd
lcom-ng run --headless --script scripts/type_a_esc.mlabscript -- build/examples/keyboard_scan
```

## Discussion prompts

- Why do input libraries report both key press and key release?
- What should happen on a parity or timeout error?
- How would you build a "key is currently down" table from the scancode stream?

## External reading

The LCOM [keyboard introduction](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab3/intro.html),
[scancode exercise](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab3/exercise1.html),
and [interrupts-vs-polling chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab3/sync_mechanisms.html)
match this lab closely. For broader reference, read OSDev on the
[PS/2 keyboard](https://wiki.osdev.org/PS/2_Keyboard) and
[PS/2 controller](https://wiki.osdev.org/%228042%22_PS/2_Controller), plus
Andries Brouwer's [scancode notes](https://aeb.win.tue.nl/linux/kbd/scancodes.html).
