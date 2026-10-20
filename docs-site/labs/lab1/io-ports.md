# I/O ports and privileged operations

The RTC uses two ports. One selects which internal register is visible; the
other is the data window for that register.

| Port | Role |
| --- | --- |
| `RTC_ADDR_REG` (`0x70`) | select the RTC register |
| `RTC_DATA_REG` (`0x71`) | read or write the selected register |

The protocol is a two-step conversation:

```c
lcom_port_write8(RTC_ADDR_REG, RTC_REG_A);
lcom_port_read8(RTC_DATA_REG, &value);
```

The controller has more internal registers than external port addresses, so one
port selects the register and the other carries data.

The sequence also matters. If another piece of code selected a different
register between the address write and the data read, the read would mean
something else. Real drivers protect these critical sections with locks,
interrupt discipline, or a single service that owns the device.

## Read before you trust

The RTC has status registers as well as data registers. Before reading date or
time fields, check whether an update is in progress. Otherwise you can assemble
fields from two different moments. Every byte may look valid while the
combination never existed.

> [!IMPORTANT]
> A successful port read only means a byte was read. It does not mean the byte is
> semantically safe to use. Device protocols define when data is valid.
