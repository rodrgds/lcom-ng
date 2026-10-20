# Reading mouse packets

A basic PS/2 packet has three bytes:

| Packet byte | Contains |
| --- | --- |
| byte 0 | buttons, sign bits, overflow bits, synchronization bit |
| byte 1 | X movement low byte |
| byte 2 | Y movement low byte |

Byte 0 includes a bit that should always be set for a normal packet. Use it to
recover when parsing starts mid-stream, like a start marker in a serial protocol.

## Signed movement

X and Y movement are signed. The sign bits are in byte 0; the low movement bytes
are bytes 1 and 2. Assemble the value deliberately.

Do not treat movement bytes as unsigned coordinates. A packet usually reports
relative movement since the previous packet, not an absolute position. The
application can add deltas to a cursor, clamp to the screen, or control a camera.

## Packet state

Track how many bytes have been collected, whether byte 0 is synchronized,
whether a complete packet is ready, and whether overflow flags invalidate or only
report movement. The parser receives one byte at a time and exposes a packet only
after all three bytes arrive.

This state-machine shape returns in UART framing, sprite parsing, and replay
scripts: partial input becomes meaningful only after enough bytes or tokens
arrive.

> [!TIP]
> Keep packet assembly separate from coordinate updates. The library parses
> packets; a drawing program uses the decoded deltas later.
