# Lab 7 check

Start with loopback before running two programs. Loopback proves one endpoint can
configure itself, send a byte, and receive it back through the runtime. Then
`run-pair` checks two independent machines through a bridged serial link.

```sh
lcom-ng test uart
lcom-ng run --headless -- build/examples/uart_loopback
lcom-ng run-pair --headless build/examples/uart_peer_sender --right build/examples/uart_peer_receiver
```

## Discussion prompts

- What fields would you add to a packet protocol built on UART bytes?
- How should a receiver recover if it starts mid-packet?
- Why is loopback useful before two-machine communication?

## External reading

Read OSDev on [serial ports](https://wiki.osdev.org/Serial_Ports) and
[UARTs](https://wiki.osdev.org/UART), plus the Wikibooks guide to
[8250/16550 UART programming](https://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming).
