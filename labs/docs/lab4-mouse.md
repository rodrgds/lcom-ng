# Lab 4: PS/2 Mouse Packets

Generated folder: `labs/mouse/`

Lab 4 uses the mouse side of the i8042 controller. Students send commands
through the mouse command path, handle ACK bytes, synchronize on the packet
sync bit, and decode three-byte packets into button and movement state.

## Student Files

- `labs/mouse/include/mouse_lib.h`
- `labs/mouse/mouse_lab.c`

## Requested Functions

- `int mouse_enable_data_reporting(void)`
- `int mouse_disable_data_reporting(void)`
- `int mouse_process_byte(uint8_t byte)`
- `int mouse_get_packet(mouse_packet_t *packet)`

## What Students Learn

- The `KBC_CMD_WRITE_MOUSE` command path.
- PS/2 ACK/NACK handling.
- Packet synchronization and resynchronization.
- Sign extension for X/Y deltas.
- Button state decoding and separating raw bytes from interpreted packets.

## Test And Reference

```sh
lcom test mouse
lcom run --headless --script scripts/mouse_move.lcomscript -- build/examples/mouse_packet
```
