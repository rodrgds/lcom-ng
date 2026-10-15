# Upstream provenance

Original repository: https://github.com/leonardomagalhaes21/feup-LCOM

Pinned revision: `9f09dbb02f492c8db058ad1b1c56821f70282670`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

proj/src/devices/mouse/mouse.c: Original native startup reproducibly SIGTRAP in mouse_write_cmd before first frame. acknowledgment_byte is read by the while condition before initialization, undefined behavior. Initialize it to a non-ACK value so the intended first command is sent.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original game chooses resolution by mouse, mouse clicks start/aim/shoot, A/D move and W jumps, exposed as explicit quick keys; ESC quits. Cursor resets when resolution changes and rejects boundary steps, so relative pointer.
