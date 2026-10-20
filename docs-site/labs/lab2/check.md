# Lab 2 check

Run the focused timer tests before using the timer from a larger program. They
verify divisor arithmetic, status reads, interrupt subscription, and tick
accounting. Then run a small headless example to confirm behavior in a loop.

```sh
lcom-ng test timer
lcom-ng run --headless -- build/examples/timer_int 3
```

## Discussion prompts

- Why does the timer API expose `timer_ticks()` instead of sleeping directly?
- What changes if the game updates at 60 Hz but the timer fires faster?
- Which timing values should be constants, and which should a final project
  configure?

## External reading

The LCOM chapters on the
[timer controller](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab2/timer-controller.html),
[interrupts](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab2/interrupts.html),
and [timer libraries](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab2/library.html)
are useful companions. For background, read OSDev on the
[Programmable Interval Timer](https://wiki.osdev.org/Programmable_Interval_Timer)
and [interrupts](https://wiki.osdev.org/Interrupts). The GNU C notes on
[integer types](https://www.gnu.org/software/libc/manual/html_node/Integers.html)
help when divisor arithmetic behaves unexpectedly.
