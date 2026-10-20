# Interrupts vs polling

Polling checks for data repeatedly. Interrupts react when a device signals that
data is available.

| Mechanism | Best use | Trade-off |
| --- | --- | --- |
| Polling | simple tests, predictable short waits | wastes CPU when events are rare |
| Interrupts | normal interactive input | needs event-loop structure |

Keyboard events are sporadic, so interrupts usually fit better. Polling is still
useful for understanding the controller and for small deterministic experiments.
Both versions exercise the same controller rules; only the decision about when
to ask for data changes.

## Timeout discipline

Polling loops need limits. A loop that waits forever for a key can hang a test,
a demo, or a grading run.

Timer-driven limits make behavior observable. "Wait until ESC is released or
until N seconds of idle time pass" is precise and testable. "Wait for some
input" can easily become impossible to test.

> [!TIP]
> Poll with a clear maximum number of attempts or a timer-driven timeout, then
> return an error the caller can handle.
