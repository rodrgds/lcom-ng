# lcom-minix-driver-game

A game for the Minix 3 operating system that talks straight to the hardware through device drivers written by hand, with no game engine and no OS conveniences.

## What it does

A graphical game controlled by the keyboard, running on Minix 3. Everything the game needs is driven through low-level device drivers written from scratch:

- **Keyboard** — the i8042 controller, reading scancodes through interrupts.
- **Timer** — the i8254, for the frame rate and timing.
- **Video card** — direct writes to graphics memory to draw the game.

The `lab0`–`lab5` folders are the driver exercises that build up to the game in `proj/`.

## Stack

C, on Minix 3, using the course's `lcom` toolchain. Interrupt-driven drivers, no external libraries.

## How to run

This runs only on a Minix 3 virtual machine with the LCOM toolchain installed. Inside `proj/`, build with the course build tools (`lcom_run proj`). It cannot run on a normal Linux or Windows host, because it depends on the Minix driver interfaces.

## What I built

Group project of four for the Computer Lab course (2024/25), built through pair programming — we wrote the code together at one keyboard, so the git history does not split cleanly by author. My contribution is in the driver and game code alongside my teammates, not in separate commits.

## What I would do differently

Separate the game logic from the drawing code more strictly, so the game state could be tested without a display, and replace the fixed-size buffers in the drawing routines with bounds-checked helpers.
