# The i8254 timer controller

The i8254 PIT has several counters. Lab 2 focuses on Timer 0, the periodic
interrupt source.

| Field | Purpose |
| --- | --- |
| timer select | choose which counter is programmed |
| access mode | decide low byte, high byte, or both |
| operating mode | choose counting behavior |
| BCD/binary | choose encoding |
| divisor | convert base frequency into output frequency |

The base frequency is divided by the programmed divisor. A lower divisor means
more frequent interrupts. You do not write the desired frequency; you compute
the divisor that produces approximately that frequency.

## Control words

A control word is a byte made from fields. Lab 1 matters here: construct the
value by shifting and OR-ing fields, not by guessing a decimal constant.

Online examples often use fixed hexadecimal control words. That is fine for a
narrow demo, but here build the byte from named fields so the timer, access mode,
operating mode, and BCD setting are visible.

> [!IMPORTANT]
> Timer programming has order. If the access mode says low byte then high byte,
> write the low byte first.
