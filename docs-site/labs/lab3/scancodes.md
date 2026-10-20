# Reading scancodes

Scancodes are device-level key events, not ASCII. For simple keys, the break code
is often the make code with the high bit set. Check the scancode set and
extended-byte cases before treating that as a universal rule.

| Byte pattern | Meaning |
| --- | --- |
| `0x1e` | A key make code |
| `0x9e` | A key break code |
| `0xE0 ...` | extended sequence |
| `0x81` | ESC break code in these tests |

Make means key press. Break means key release. Some keys arrive as multi-byte
sequences, and the parser must not report a scancode until the sequence is
complete.

## Parser state

A correct parser needs state: idle with no prefix, waiting after `0xE0`, or
holding a complete scancode for the caller. That state belongs in the keyboard
library, not in every application loop.

This is the first place where one byte is not always one message. The same idea
returns in mouse packets, UART protocols, and file formats: the parser must know
when it has enough bytes to describe one event.

## Application state

Higher-level code can build "key is currently down" tables, text input, menu
actions, or game movement. The low-level parser reports what happened at the
device boundary without deciding what the application should do.
