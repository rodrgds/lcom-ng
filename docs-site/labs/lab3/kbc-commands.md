# Managing KBC commands

The controller also accepts commands. A command write is a small protocol, not a
single unconditional store. A safe path checks whether the input buffer is
available, whether the command was accepted, whether a response byte should be
read, and whether a retry is appropriate.

Command paths are where input libraries become brittle. The controller may be
busy, the command may produce an acknowledgement, and the same output path may
also carry scancode bytes. The mouse lab uses a similar command and ACK pattern,
so this work prepares you for Lab 4. Keep command responses separate from normal
input.

## Common command bugs

Writing while the input buffer is full, ignoring timeout or error bits, retrying
forever, and mixing command responses with scancodes. Keep command helpers
narrow: one that writes a controller command should not also parse scancodes or
update application state.
