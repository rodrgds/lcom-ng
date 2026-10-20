# Lab 1: Minix-style I/O without the VM

Generated folder: `labs/rtc/`

Lab 1 introduces the course mental model. A program talks to a small controller
through registers, and those bytes only mean something once the programmer knows
which bits matter. The device is the RTC/CMOS clock: a tiny register set, an
observable protocol, and understandable errors.

The lab already teaches the habits used later: check status before trusting
data, decode values by a configuration bit, return errors separately from output
values, and keep C helpers small enough to inspect. lcom-ng keeps the
controller-facing work and moves hardware behind a safe runtime.

## What you will learn

- Why older PC devices used I/O ports.
- How to isolate and construct bit fields.
- Why a controller exposes status before data.
- How BCD differs from a binary integer.
- Why C APIs return a status code while filling caller-owned structures.

## Plan

Read [why I/O looked this way](/labs/lab1/context), then the
[port pair used by the RTC](/labs/lab1/io-ports). Implement the
[bitwise helpers](/labs/lab1/bitwise) before
[reading date and time](/labs/lab1/rtc). The [tasks](/labs/lab1/tasks) list the
required functions and the [checks](/labs/lab1/check) show how to validate them.
Finish with [modern hardware access](/labs/lab1/modern).

> [!TIP]
> Do the bit helpers first. They are pure C and build confidence before
> controller reads.
