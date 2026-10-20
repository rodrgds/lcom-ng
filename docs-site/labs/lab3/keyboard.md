# The keyboard controller

The i8042-style controller sits between the CPU and the keyboard. It exposes a
small register interface and raises an event when a scancode byte is waiting.

| Register | Role |
| --- | --- |
| status register | output-buffer state and error bits |
| output buffer | next scancode byte |
| command register | controller commands |
| IRQ line | keyboard event notification |

The safe read sequence is: read status, confirm the output buffer is full, check
error bits, read the output byte, and pass the byte to the scancode parser. The
status read prevents empty-buffer reads. The error checks prevent corrupted bytes
from becoming trusted input. The parser call keeps device I/O separate from
scancode assembly.

Avoid collapsing this into a single "get key" helper too early. At this layer
you get one byte from a controller stream. It may be a prefix, a command
response, or part of a larger event.

> [!IMPORTANT]
> Do not read output data just because your program wants input. Read only when
> the status register says data is available.
