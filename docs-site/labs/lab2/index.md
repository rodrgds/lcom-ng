# Lab 2: PIT and timer IRQs

Generated folder: `labs/timer/`

Lab 2 introduces periodic hardware events. You configure a timer-like device
and react to interrupts instead of asking an OS clock for time. The program now
waits for a device to report that something happened.

## Device model

| Part | Role |
| --- | --- |
| Timer 0 | periodic interrupt source |
| Control word | selects timer, access mode, operating mode, BCD/binary |
| Divisor | converts base clock frequency into requested rate |
| IRQ line | event path from device to program |
| Interrupt handler | records that a tick happened |

The timer is simple, but it supports keyboard idle timeouts, mouse polling
windows, animation, audio scheduling, serial retries, and final project loops.

## Plan

Read [why PC timers exist](/labs/lab2/context), then the
[i8254 controller](/labs/lab2/timer-controller) and
[timer divisor arithmetic](/labs/lab2/programming). Compare
[polling and interrupts](/labs/lab2/interrupts), then turn the exercise into a
[reusable timer library](/labs/lab2/library). Complete the
[tasks](/labs/lab2/tasks), validate with the [checks](/labs/lab2/check), and
compare with [modern timers](/labs/lab2/modern).

> [!TIP]
> Timer code is infrastructure. Keep it small so the final project can use it
> without copying lab-specific logic.
