# Lab 7: UART And Serial Ports

Generated folder: `labs/uart/`

Lab 7 covers 16550-style serial ports. Students configure baud rate through the
divisor latch, set line format with LCR, enable FIFOs and RX interrupts, test
local loopback, and exchange bytes through the virtual COM1/COM2 cable.

## Student Files

- `labs/uart/include/uart_lib.h`
- `labs/uart/uart_lab.c`

## Requested Functions

- `int uart_config(uint16_t base, uint32_t baud, uint8_t line_control)`
- `int uart_enable_fifo(uint16_t base)`
- `int uart_enable_rx_interrupt(uint16_t base)`
- `int uart_set_loopback(uint16_t base, int enabled)`
- `int uart_send_byte(uint16_t base, uint8_t byte)`
- `int uart_read_byte(uint16_t base, uint8_t *byte)`
- `int uart_subscribe(uint8_t irq, lcom_irq_t *out)`
- `int uart_unsubscribe(lcom_irq_t *irq)`

## What Students Learn

- DLAB, DLL/DLM divisor programming, and baud-rate calculation.
- LCR, FCR, IER, LSR, and MCR register responsibilities.
- FIFO setup and RX-ready interrupt handling.
- Local loopback versus paired virtual serial cables.
- Application-level protocol design for multiplayer/state synchronization.

## Test And Reference

```sh
lcom test uart
lcom run --headless -- build/examples/uart_pair
lcom run-pair build/examples/ninjix --right build/examples/ninjix
```
