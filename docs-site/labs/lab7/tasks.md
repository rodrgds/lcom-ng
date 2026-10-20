# Lab 7 tasks

Work in `labs/uart/`. Configure the UART, send and receive bytes reliably, and
prove the endpoint works before `run-pair` connects two programs.

## Requested functions

- `int uart_config(uint16_t base, uint32_t baud, uint8_t line_control)`
- `int uart_enable_fifo(uint16_t base)`
- `int uart_enable_rx_interrupt(uint16_t base)`
- `int uart_set_loopback(uint16_t base, int enabled)`
- `int uart_send_byte(uint16_t base, uint8_t byte)`
- `int uart_read_byte(uint16_t base, uint8_t *byte)`
- `int uart_subscribe(uint8_t irq, lcom_irq_t *out)`
- `int uart_unsubscribe(lcom_irq_t *irq)`

## Guided gaps

Validate base port and baud, compute the divisor from the UART base clock,
preserve line-control bits around divisor-latch access, add send and read
timeouts, and keep loopback setup separate. The divisor-latch bit changes which
logical register later accesses use.

> [!IMPORTANT]
> Always restore line-control state after touching the divisor latch. Otherwise
> later reads and writes may go to the wrong logical register.

## Common mistakes

Missing divisor-latch access before the divisor write, failing to clear it after,
sending before transmitter-ready, and treating "no byte" as zero. In a byte
stream, these become protocol bugs.

Next: [checks and references](/labs/lab7/check).
