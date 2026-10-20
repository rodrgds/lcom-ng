# How it works today: keyboards

Modern keyboard stacks usually pass through USB HID, Bluetooth HID, operating
system input layers, keymaps, input methods, and accessibility services before
applications see input.

## What changed

Applications receive key events from SDL, browser events, Cocoa, Win32, X11,
Wayland, or terminal input modes. Those APIs translate device reports into
richer events with logical keys, text input, modifier state, repeat behavior,
focus, and layout-dependent characters.

## What stayed useful

Byte streams need careful decoding, command responses must not be confused with
data, and low-level input should be separated from UI behavior. These ideas apply
to PS/2, USB HID, Bluetooth, terminals, and virtual input backends.

lcom-ng uses the older keyboard shape because it is visible enough to teach.
Modern stacks handle international text and accessibility better, but are harder
to inspect end to end in a first systems course.
