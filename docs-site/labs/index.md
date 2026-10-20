# Lab track

The lab track teaches machine-facing C through small device-shaped exercises.
Each lab has an overview of the device model, implementation tasks with
intentional gaps, and checks with commands and external reading. Every lab also
explains the controller protocol and its modern equivalent.

## Before you start

[Install lcom-ng](/students/install), then create a workspace:

```sh
lcom-ng setup student
cd student
make
```

Run lab checks from the workspace root. Commands that name `build/examples/` or
`scripts/` use the repository checkout. A generated workspace builds
`build/student_project` and keeps its scripts under `.mlab/scripts/`.

## Sequence

| Lab | Device idea | Check |
| --- | --- | --- |
| [Lab 1](/labs/lab1/) | bit masks, CMOS/RTC, BCD | `lcom-ng test rtc` |
| [Lab 2](/labs/lab2/) | PIT divisors and timer IRQs | `lcom-ng test timer` |
| [Lab 3](/labs/lab3/) | i8042 keyboard and scancodes | `lcom-ng test kbd` |
| [Lab 4](/labs/lab4/) | PS/2 mouse packets | `lcom-ng test mouse` |
| [Lab 5](/labs/lab5/) | VBE framebuffer and sprites | `lcom-ng test graphics` |
| [Lab 6](/labs/lab6/) | AC97-lite PCM audio | `lcom-ng test audio` |
| [Lab 7](/labs/lab7/) | 16550 UART and serial protocols | `lcom-ng test uart` |

## Method

1. Read the overview.
2. Run the failing starter test.
3. Implement the requested functions.
4. Keep reusable helpers small.
5. Produce one artifact: test output, screenshot, WAV, trace, or replay.

> [!TIP]
> Fresh stubs are expected to fail. The first failure is the starting point, not
> evidence that setup is broken.
