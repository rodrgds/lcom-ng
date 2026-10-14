# Exercise PS/2 error recovery

Replay a raw controller byte with `at <tick> ps2 <keyboard|mouse> <byte> <errors>`. The byte accepts decimal 0 to 255 or hexadecimal `0x00` to `0xff`. Errors are `ok`, `parity`, `timeout` or `parity,timeout`; `timeout,parity` also works. Invalid sources, bytes, flags and extra arguments fail with the script line number.

```text
at 1 ps2 keyboard 0x1e parity
at 2 ps2 keyboard 0x30 ok
at 3 ps2 mouse 8 timeout
at 4 ps2 mouse 9 ok
```

Use `--script` for `run`, or `--left-script` and `--right-script` for `run-pair`. The controller exposes bit 7 for parity, bit 6 for timeout and bit 5 for mouse data only while that byte is at the front of its output queue. Reading port `0x60` consumes the byte and reveals the next byte's status. A driver should read status first, consume the byte and discard it if either error flag is set. IRQ1 or IRQ12 follows the byte's source and the existing controller command-byte policy.

This event represents already-received controller output. It can replay a mouse byte while data reporting is disabled, without enabling reporting or changing any device mode. Ordinary `key`, `mouse`, command ACKs and packets keep their existing behavior. Input-buffer busy time and physical PS/2 transmission timing remain outside this replay model. [FEUP keyboard error-handling lecture](https://pages.up.pt/~up238172/aulas/lcom2014/at/5kbrd.pdf), [FEUP mouse/controller handout](https://pages.up.pt/~up238172/aulas/lcom2223/labs/lab4/lab4_3.html).
