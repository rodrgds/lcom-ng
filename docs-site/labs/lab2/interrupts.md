# Interrupts

Polling repeatedly checks a status bit. Interrupts let a device notify the
program when something happened.

| Mechanism | Strength | Cost |
| --- | --- | --- |
| Polling | simple, predictable control flow | wastes work when events are rare |
| Interrupts | efficient for sporadic events | requires event-loop structure |

The original PC path used programmable interrupt controllers and IRQ lines.
Modern controllers differ, but the idea is recognizable: subscribe, receive an
event, do minimal handler work, return to main logic.

Polling is not wrong. It can be simplest when the program has nothing else to do
or expects an event immediately, but it scales poorly when the CPU could work
while the device is quiet.

## Handler discipline

A handler should acknowledge or record the event, update a small counter or
flag, and avoid rendering, parsing, allocation, or blocking calls. The handler
learns that an event happened; the application loop reacts to it.

This separation keeps the program understandable once several devices are
involved. In Lab 3, keyboard and timer events arrive through the same loop, and
each handler must leave enough information for the main logic.

> [!IMPORTANT]
> A timer interrupt is not a license to run the whole program inside the
> handler. Keep the handler short.
