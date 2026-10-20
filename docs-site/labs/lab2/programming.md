# Reading and programming timers

The core operation is divisor programming:

```text
divisor = base_frequency / requested_frequency
```

After computing the divisor, write a control word, then the divisor low byte,
then the divisor high byte. The arithmetic chooses the value; the protocol
delivers it to the controller.

## Validation

Reject impossible requests before touching the device: an invalid timer number,
zero frequency, a divisor outside the 16-bit range, or a null output pointer for
a status read. This keeps tests deterministic and prevents a failed half-write
from leaving surprising controller state.

## Status reads

Status reads prove that your code can ask the controller what it is configured
to do, not only write new configuration. Good low-level code inspects status,
explains current state, and uses that information to recover from errors.

## Practical debugging

When stuck, print the requested frequency, computed divisor, low byte, high
byte, and control word in hexadecimal. If the values do not match your
expectation, inspect field construction, integer division, and byte order.
