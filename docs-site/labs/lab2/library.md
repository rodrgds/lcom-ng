# Creating a timer library

Lab 2 is the first place where reusable helpers make sense. Later labs and final
projects need timing for animation, idle timeouts, keyboard and mouse loops,
audio scheduling, and replay-friendly demos.

## What belongs in the library

Good candidates are subscribe and unsubscribe wrappers, tick counters,
elapsed-second helpers, frequency setup helpers, and small state reset functions
for tests. These describe the timer itself and make later code easier to read.

Poor candidates are project-specific game state, drawing code, keyboard or mouse
parsing, and hardcoded lab test output. If a function only makes sense for one
exercise transcript, keep it out of the library.

Aim for the middle: small functions whose names explain the policy they
implement. A library that exposes every register is not much of a library, but
one that hides all timing decisions prevents students from learning how
timer-driven programs work.
