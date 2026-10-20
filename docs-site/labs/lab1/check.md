# Lab 1 check

Run the focused RTC test first. It is faster than launching an example and gives
a direct signal about the helper contracts. Then run the headless example with a
fixed virtual RTC time so output is deterministic.

```sh
lcom-ng test rtc
lcom-ng run --headless --rtc 2026-06-16T12:34:56 -- build/examples/rtc_date
```

## Discussion prompts

- Why does `rtc_read_date` receive a pointer instead of returning a struct?
- What goes wrong if the date changes between reading day, month, and year?
- Which helpers belong in `labs/rtc/` rather than a reusable `lib/rtc/` module?

## External reading

The LCOM [bitwise chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab1/protocols-bitwise.html)
and [RTC chapter](https://pages.up.pt/~up748353/classes/lcom/lab-guides/lab1/rtc.html)
are useful companions. For C details, review the GNU C notes on
[bitwise operations](https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Bitwise-Operations.html),
[shift operations](https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Shift-Operations.html),
and [variadic functions](https://www.gnu.org/software/libc/manual/html_node/Variadic-Functions.html).
For the operating-system view, read the
[OSDev CMOS overview](https://wiki.osdev.org/CMOS).
