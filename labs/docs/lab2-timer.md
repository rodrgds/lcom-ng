# Lab 2: PIT And Timer IRQs

Generated folder: `labs/timer/`

Lab 2 introduces the i8254 PIT as the first interrupt-driven device. Students
program timer divisors, inspect timer status with read-back commands, subscribe
to IRQ0, and use timer ticks as the heartbeat for later animations and games.

## Student Files

- `labs/timer/include/timer_lib.h`
- `labs/timer/timer_lab.c`

## Requested Functions

- `int timer_set_frequency(uint8_t timer, uint32_t freq)`
- `int timer_get_conf(uint8_t timer, uint8_t *status)`
- `int timer_subscribe(lcom_irq_t *irq)`
- `int timer_unsubscribe(lcom_irq_t *irq)`
- `void timer_ih(void)`
- `uint32_t timer_ticks(void)`

## What Students Learn

- PIT control words, channel selection, and LSB/MSB programming.
- Divisor calculation and edge cases around `TIMER_FREQ`.
- Read-back status commands.
- IRQ subscription and event-loop dispatch with `lcom_event_wait`.
- Keeping interrupt handler state small and explicit.

## Test And Reference

```sh
lcom test timer
lcom run --headless -- build/examples/timer_int 3
```
