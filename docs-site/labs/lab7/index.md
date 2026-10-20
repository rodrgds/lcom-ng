# Lab 7: UART and serial ports

Generated folder: `labs/uart/`

Lab 7 introduces serial communication through a 16550-style UART. A UART turns
bytes into a stream and receives a stream back into bytes. It fits in a lab but
contains the reliability questions of larger communication systems.

| Register idea | Role |
| --- | --- |
| line control | word length, stop bits, parity, divisor access |
| divisor latch | baud-rate divisor |
| FIFO control | receive/transmit buffering |
| line status | data-ready and transmitter-ready bits |
| interrupt enable | which events raise IRQs |
| loopback | transmit is routed back to receive |

Read [serial protocol thinking](/labs/lab7/protocols), complete the
[tasks](/labs/lab7/tasks), run the [checks](/labs/lab7/check), and compare with
[modern serial links](/labs/lab7/modern).

> [!TIP]
> Loopback proves one endpoint before `run-pair` asks you to reason about two
> independent programs.
