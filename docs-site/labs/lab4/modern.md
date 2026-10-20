# How it works today: pointer input

Modern pointing devices usually use USB HID, Bluetooth HID, touchpads, or
integrated controllers rather than PS/2 packet streams.

## What changed

Applications often receive higher-level events with position, button state,
scroll wheels, gestures, touch contacts, pressure, or device identity. A desktop
application may never see the raw packet format used by the device.

Operating systems apply policy above the raw layer. Pointer acceleration,
coordinate transformation, palm rejection, gesture recognition, and compositor
routing usually happen before application code receives an event.

## What stayed useful

Packet thinking still matters. Input arrives incrementally, synchronization can
be lost, relative movement differs from absolute position, and low-level parsing
should be separated from UI policy. The same lessons apply to gamepads, tablets,
network messages, and replay streams.

lcom-ng keeps the PS/2-shaped mouse because it is small enough to inspect and
rich enough to teach synchronization and signed deltas.
