# Handling multiple I/O devices

Real reactive programs wait for more than one event source. Lab 3 combines
keyboard input with timer events. The loop answers whether the keyboard produced
a byte, whether the timer ticked, whether the program should stop because ESC was
released, and whether it should stop because no key arrived for too long.

This is the first place where independent device events become one policy.
Keyboard events reset the idle counter. Timer events advance it. The loop
terminates only when a stated condition becomes true.

## Why the timer matters

Keyboard input alone cannot measure idle time. Timer interrupts give the loop a
heartbeat so it can terminate after a quiet interval. Games and editors use the
same pattern: input changes state, time advances state, rendering presents state.

Reusable libraries pay off here. The timer should not know about scancodes, and
the keyboard should not know about idle seconds. The application loop turns both
into policy.
