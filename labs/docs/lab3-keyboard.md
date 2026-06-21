# Lab 3: Keyboard And i8042

Generated folder: `labs/kbd/`

Lab 3 works with the keyboard side of the i8042 controller. Students read
status/data ports, handle IRQ1, distinguish make and break codes, and assemble
one-byte and two-byte Set 1 PS/2 scancodes.

## Student Files

- `labs/kbd/include/keyboard_lib.h`
- `labs/kbd/keyboard_lab.c`

## Requested Functions

- `int kbc_read_status(uint8_t *status)`
- `int kbc_read_output(uint8_t *byte)`
- `int kbc_write_command(uint8_t command)`
- `int kbd_process_byte(uint8_t byte)`
- `int kbd_get_scancode(uint8_t bytes[2], uint8_t *size, int *make)`

## What Students Learn

- i8042 status bits, output-buffer checks, and error filtering.
- Command-port writes and controller command flow.
- Make/break scancode interpretation.
- Stateful parsing for `0xE0` two-byte scancodes.
- Keeping keyboard parsing independent from menu/game behavior.

## Test And Reference

```sh
lcom test kbd
lcom run --headless --script scripts/type_a_esc.lcomscript -- build/examples/keyboard_scan
```
