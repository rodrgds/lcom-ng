# Lab 3 tasks

Work in `labs/kbd/`. Build a reliable keyboard layer before application
behavior, and keep controller reads, parser state, and output separate.

## Requested functions

- `int kbc_read_status(uint8_t *status)`
- `int kbc_read_output(uint8_t *byte)`
- `int kbc_write_command(uint8_t command)`
- `int kbd_process_byte(uint8_t byte)`
- `int kbd_get_scancode(uint8_t bytes[2], uint8_t *size, int *make)`

## Guided gaps

Check output pointers, ignore reads without data, detect error bits, store a
pending `0xE0` prefix, and report make/break state without losing bytes. Keep
command and ACK handling separate from parsing, because a response is not a key
event.

Decide how callers observe "no complete scancode yet." It may mean the parser is
waiting for the second byte of an extended sequence, not that an error occurred.

> [!IMPORTANT]
> Do not treat every byte as a complete scancode. Extended scancodes must be
> assembled before the API reports an event.

## Common mistakes

Clearing parser state too early, reporting a scancode before all bytes arrive,
treating break codes as presses, and reading output without checking status.
Most are ownership mistakes: the program loses track of the next byte, parser
state, or subscribed IRQ.

Next: [checks and references](/labs/lab3/check).
