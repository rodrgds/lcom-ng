# Upstream provenance

Original repository: https://github.com/Joanaigs/Lcom-Snake-Game

Pinned revision: `6ebf0b9669845718b630cd8ea5707f5f0c216054`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.

## Recorded source corrections

proj/src/graphics.c: Native menu reproducibly SIGSEGV in draw_xpm. Original graphics code truncates heap/framebuffer pointers to 32-bit unsigned int before address arithmetic. Use uintptr_t for the three existing pointer offsets, preserving all game behavior.

Exact original/patched SHA256 and SOURCE_PATCH.diff are recorded in UPSTREAM.json.

## Controls

Original menus are mouse clicks. WASD moves snake, ESC ends round, ENTER continues after game over. Source cursor is local per menu and has no clamping, so relative pointer. Original enable function sends controller prefix but returns before F4; project environment supplies initial reporting.

## Cursor corrections

The original40×40 cursor now remains inside the framebuffer in menus, multiplayer and game-over. Each original phase still starts at200,200 with unit signed movement. The mouse reporting helper now sends the actual enable/disable command and consumes its ACK with bounded polling. Public CLI cursor captures cover both corners through raw packets and normal mouse input. The existing graphics pointer-width correction and original art are retained.
