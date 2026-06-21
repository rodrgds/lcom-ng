# Lab 1: Bitwise Helpers And RTC

Generated folder: `labs/rtc/`

Lab 1 starts with the C skills that all later device work depends on: bit masks,
byte extraction, fixed-width integer types, output pointers, and careful
conversion between binary and BCD values. The device side reads the virtual
CMOS/RTC through ports `RTC_ADDR_REG` (`0x70`) and `RTC_DATA_REG` (`0x71`).

## Student Files

- `labs/rtc/include/bitwise.h`
- `labs/rtc/include/rtc_lib.h`
- `labs/rtc/bitwise.c`
- `labs/rtc/rtc_lab.c`

## Requested Functions

- `uint8_t bit_clear(uint8_t value, uint8_t bit)`
- `uint8_t bit_set(uint8_t value, uint8_t bit)`
- `int bit_is_set(uint8_t value, uint8_t bit)`
- `uint8_t bit_lsb(uint16_t value)`
- `uint8_t bit_msb(uint16_t value)`
- `uint8_t bit_mask(unsigned first_bit, ...)`
- `int rtc_read_date(lcom_rtc_date_t *date)`
- `int rtc_read_time(lcom_rtc_time_t *time)`

## What Students Learn

- Bitwise operators, shifts, masks, and sentinel-terminated varargs.
- Integer promotion traps around `uint8_t` and `uint16_t`.
- Returning structured results through pointer arguments.
- Selecting CMOS registers and reading date/time fields.
- BCD to binary conversion and 24-hour date/time representation.

## Test And Reference

```sh
lcom test rtc
lcom run --headless --rtc 2026-06-16T12:34:56 -- build/examples/rtc_date
```
