# How it works today: serial links

Traditional UARTs are less visible on consumer laptops, but serial thinking is
common. Firmware consoles, embedded boards, USB-to-serial adapters, debug ports,
bootloaders, industrial devices, and virtual serial ports keep the model alive.

USB CDC and other transports may hide the physical UART, but software often still
sees a byte stream. Both sides must agree on configuration, receivers must handle
partial messages, and protocols need to recover from lost synchronization.

## What stayed useful

Configuration agreement, status-before-read and write, buffering, loopback
testing, framing and resynchronization, and timeout and retry thinking. Those
lessons transfer to network protocols and file formats as much as to serial
ports.

The old UART is valuable because it is small enough to understand. Once you can
build a reliable byte-stream helper here, larger systems are the same concerns
with more layers and failure modes.
