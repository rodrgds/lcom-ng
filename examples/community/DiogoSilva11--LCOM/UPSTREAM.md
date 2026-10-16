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

## Cursor edge correction

Clamp only the14x13 crosshair drawing origin inside the framebuffer. Preserve full-screen logical aim and shooting coordinates. The original pixel renderer writes all supplied pixels without clipping. Public moveCrosshair then drawCrosshair submits draw1023,767 before correction, which extends past1024x768 allocation. Corrected draw fits1010,755 while crosshairCenter remains1030,773. Signed return-to-origin is also verified. Original game art and all previous source correction records are retained.
