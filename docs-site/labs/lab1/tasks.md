# Lab 1 tasks

Work in `labs/rtc/`. Turn the protocol into clear C code. Keep helpers boring: a
function that builds a mask should build a mask, not hide controller reads or
formatting.

## Requested functions

- `uint8_t bit_clear(uint8_t value, uint8_t bit)`
- `uint8_t bit_set(uint8_t value, uint8_t bit)`
- `int bit_is_set(uint8_t value, uint8_t bit)`
- `uint8_t bit_lsb(uint16_t value)`
- `uint8_t bit_msb(uint16_t value)`
- `uint8_t bit_mask(unsigned first_bit, ...)`
- `int rtc_read_date(lcom_rtc_date_t *date)`
- `int rtc_read_time(lcom_rtc_time_t *time)`

## Guided gaps

- Build bit masks from bit positions and handle out-of-range bits predictably.
- End the variadic `bit_mask` argument list with `BIT_MASK_END`.
- Poll `RTC_REG_A` until `RTC_UIP` is clear.
- Keep BCD conversion small and inspectable.
- Check output pointers before writing.

Decide which helpers are exercise-only and which deserve to become reusable
library code. A `bcd_to_binary` helper likely stays near the RTC implementation.
Generic bit helpers may serve several labs if their names and behavior are
precise.

> [!IMPORTANT]
> Do not change RTC configuration. Interpret the mode reported by the device
> instead of forcing the device into the mode you prefer.

## Common mistakes

Treating BCD as binary, forgetting that bit positions start at zero, returning a
mask when the API asks for boolean-like output, and writing a partial structure
after a failed read. Tests should lead you to these bugs if each helper stays
focused.

Next: [checks and references](/labs/lab1/check).
