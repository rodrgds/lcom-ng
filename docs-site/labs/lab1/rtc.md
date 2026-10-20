# Reading the RTC

The RTC exposes data registers with the current time fields and status registers
that say how to read them.

| Register | Meaning |
| --- | --- |
| `RTC_SECONDS` | seconds |
| `RTC_MINUTES` | minutes |
| `RTC_HOURS` | hours |
| `RTC_DAY` | day of month |
| `RTC_MONTH` | month |
| `RTC_YEAR` | two-digit year |
| `RTC_REG_A` | update-in-progress bit |
| `RTC_REG_B` | data-mode bit |

## Update-in-progress

`RTC_REG_A` contains the `UIP` bit. While it is set, the RTC is updating its
fields, so reads can be inconsistent.

The safe sequence is: read `RTC_REG_A`, wait while `RTC_UIP` is set, read
`RTC_REG_B`, then read the date and time fields. Convert from BCD afterward if
register B says the device is in BCD mode. Correctness matters more than saving
a few port operations.

## BCD

BCD stores decimal digits in nibbles:

| Raw value | Meaning |
| --- | --- |
| `0x09` | 9 |
| `0x16` | 16 |
| `0x42` | 42 |

Binary `0x42` is decimal `66`, so the conversion must be deliberate. BCD exists
for historical compatibility with decimal displays and firmware. Convert it to
an ordinary binary integer before the rest of the code uses it.

## Output structures

The lab functions receive output pointers:

```c
int rtc_read_date(lcom_rtc_date_t *date);
```

This lets the function return a status code while filling the caller's structure
only on success. Returning an `int` reports failure; the pointer supplies
structured output. Validate the pointer before writing and do not leave a
half-updated structure after a failed read.
