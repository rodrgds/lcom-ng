# Lab 2 tasks

Work in `labs/timer/`. This code becomes infrastructure for later labs, so take
naming and state ownership seriously.

## Requested functions

- `int timer_set_frequency(uint8_t timer, uint32_t freq)`
- `int timer_get_conf(uint8_t timer, uint8_t *status)`
- `int timer_subscribe(lcom_irq_t *irq)`
- `int timer_unsubscribe(lcom_irq_t *irq)`
- `void timer_ih(void)`
- `uint32_t timer_ticks(void)`

## Guided gaps

Reject invalid timer numbers and impossible frequencies before writing the
control word. Build the control word from named fields, split the 16-bit divisor
into low and high bytes, and track ticks without wall-clock time. Subscribe and
unsubscribe should be symmetric, with a clear cleanup path that does not depend
on unrelated state.

Once the functions pass, review them as project code. The tick counter,
elapsed-time helpers, and subscription wrappers probably belong in `lib/timer/`.
Lab formatting and one-off harness behavior stay outside the library.

> [!IMPORTANT]
> Interrupt handlers should not contain large application logic. Record the
> event, then let the main loop decide what to do.

## Common mistakes

Dividing the wrong way, accepting a zero divisor, writing one divisor byte, and
counting ticks in the main loop. If tests report a timing mismatch, inspect the
divisor and byte order before rewriting.

Next: [checks and references](/labs/lab2/check).
