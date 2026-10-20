# Lab 1 context: why I/O looked this way

Early PC hardware used simple buses and small controller chips. A device exposed
registers, not objects, file descriptors, or driver APIs. Software selected a
register, read a byte, checked a status bit, and repeated until the device was
ready.

The original LCOM/Minix path used privileged services because ordinary programs
should not freely touch hardware ports. lcom-ng keeps the controller shape and
virtualizes the port reads and writes.

## The transferable idea

The addresses `0x70` and `0x71` are historical details. The lesson is that a
controller has a small public contract: a status bit says whether data is valid,
a configuration bit changes how the next byte is interpreted, and a write to one
register may affect a later read from another.

The pattern returns in every later lab, whether the device is a timer, keyboard,
mouse, video card, audio buffer, or serial port.

## What lcom-ng removes

You do not configure VirtualBox, register a Minix service, request I/O
privileges, or restore VM drivers. Treat this lab as a device-driver exercise
with the dangerous parts removed: mistakes become test failures and trace
output, not a broken guest.
