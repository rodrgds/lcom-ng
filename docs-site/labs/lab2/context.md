# Lab 2 context: why PC timers exist

The CPU is much faster than many I/O devices and than human-visible time. A
program needs to ask "has enough time passed?" without a busy loop. Older PCs used
timer hardware such as the i8254 PIT: program a divisor, receive periodic events,
and count ticks.

## What you learn

Hardware time derives from a base clock. You program a divisor so the counter
produces events at the requested frequency, then count events. Time becomes
measurable independently of CPU speed.

Programming a controller also means writing bytes in a required order. A divisor
is 16 bits, but the interface writes a low byte and a high byte. Wrong order
configures a different value while every port write succeeds.

## Why this comes before graphics

Animation needs a timing policy: when to update state, draw, wait for input, and
stop an idle loop. Without one, a graphical program runs as fast as the CPU or
sleeps in a way that is hard to test. The handler records ticks; the main program
decides what they mean.
