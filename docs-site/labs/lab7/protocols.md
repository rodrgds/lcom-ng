# Serial protocols

A UART sends and receives bytes. A protocol gives those bytes meaning. Without
one, the receiver sees a stream with no message boundary, payload length,
checksum, or recovery rule.

## UART configuration

Both sides must agree on baud rate, word length, stop bits, parity, and whether
FIFOs and interrupts are enabled. A mismatch can produce data that looks random
or is only occasionally wrong. Configuration is part of the protocol: both
programs must agree on byte representation and readiness before messages flow.

## From bytes to messages

A robust protocol usually needs a start marker or length, payload bytes, a
checksum or sequence number, acknowledgement or retry behavior, and timeout
rules. Lab 7 starts with raw UART helpers. Final projects can build higher-level
protocols on top.

## `run-pair`

`run-pair` creates two independent runtime machines and bridges their serial
ports. Unlike loopback inside one machine, this is two programs connected by a
cable, each with its own state, timeouts, and failure modes.
