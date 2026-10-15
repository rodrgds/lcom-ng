# Upstream provenance

Original repository: https://github.com/DiogoSilva11/LCOM

Pinned revision: `be191c1e409efecbe5ef6ea2ddad795c6f63e1e8`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

proj/src/mouse.c: Original mouse_cmd waited for an ACK after controller prefix0xD4, which has no reply. Original menu exit failed time-out/status1 in mouse_disable; now only the device command argument waits for its ACK.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original menu1 play/0 quit. W/A/S/D move, mouse aims crosshair and left-button release shoots. Relative pointer retains source crosshair behavior.
