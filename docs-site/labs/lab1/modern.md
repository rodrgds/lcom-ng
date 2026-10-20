# How it works today: RTC

Modern machines keep compatibility paths for old PC interfaces, but most
high-performance hardware does not use isolated I/O ports.

## MMIO

Modern devices commonly expose registers through memory-mapped I/O. Software
reads and writes addresses in a physical memory range reserved for the device.
The syntax looks like pointer access, but the contract is the same: status bits
report readiness and errors, control bits configure behavior, read-modify-write
matters when fields share a word, and ordering rules still apply. The access
mechanism changed; the protocol discipline did not.

## Firmware and OS mediation

Normal programs call operating-system APIs such as `clock_gettime`. The OS,
firmware, and hardware abstraction layers choose between RTC state, HPET, APIC
timers, TSC deadline timers, or a paravirtualized clock. Applications want a
stable time API; device-specific work belongs below them.

## Why learn the old shape

The RTC is small enough to understand completely. It teaches register selection,
status-before-data, binary encoding choices, and output-parameter error handling
without a kernel driver framework. Those lessons transfer to MMIO drivers and to
ordinary code that parses small binary protocols.
