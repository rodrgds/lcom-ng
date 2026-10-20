# The i8042 mouse controller

The PS/2 mouse is the auxiliary device behind the same controller family as the
keyboard. Status-checking and command-writing habits from Lab 3 still apply, but
command responses and movement packets travel through closely related paths.

## Command pattern

Before movement packets, software enables data reporting: wait until the
controller accepts a command, send the mouse command path, write the command,
read and validate the acknowledgement, and retry or fail cleanly. Command writes
are a protocol, not a single store.

The acknowledgement matters because the output stream is shared. If the program
mistakes an ACK byte for movement data, packet assembly starts one byte off and
the parser may stay confused until it finds a synchronization bit.

## Why disable reporting

Leave devices in a predictable state. If data reporting stays enabled after a
program exits, later input streams can start with leftover bytes.

A lab function cleans up the device state it changed, even on error or early
exit, keeping tests reliable and preventing contamination between experiments.

> [!IMPORTANT]
> Command responses and movement data share the byte stream. Do not let an ACK
> byte become packet byte 0.
