# Bitwise operations

Registers are compact. One byte may contain several independent fields:

```text
Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0
------+-------+-------+-------+-------+-------+-------+-------
 UIP  |  DV2  |  DV1  |  DV0  |  RS3  |  RS2  |  RS1  |  RS0
```

`UIP` is bit 7. To isolate it:

```c
uint8_t uip = reg_a & 0x80;
```

To build the mask without hardcoding:

```c
uint8_t mask = (uint8_t)(1u << 7);
```

## Read values

Use `&` to test whether a bit is present. A mask with one bit set asks a
yes-or-no question about that position. In C, any non-zero result is true. A
helper such as `bit_is_set` should therefore document whether it returns the raw
mask or a normalized boolean.

```c
if (value & BIT(7)) {
  /* bit 7 is set */
}
```

`BIT(n)` is defined in the public SDK header as `(1u << (n))`.

## Construct write values

Use `|` to set bits and `& ~mask` to clear them. These operations change one
field while preserving its neighbors.

```c
value |= BIT(2);
value &= (uint8_t)~BIT(2);
```

This lab mostly reads the RTC. Later labs use the same idea for timer control
words, UART line control, and mouse or keyboard commands.

When debugging, write the value in binary beside the test. Most mistakes are
position mistakes, not arithmetic mistakes. Bit 0 is the least significant bit.

> [!TIP]
> Build masks from bit positions, then name them. Named masks document the device
> protocol and reduce off-by-one errors.
