# Upstream provenance

Original repository: https://github.com/Forreca05/LCOM-Snake-Game-Projeto-2025

Pinned revision: `a1da222e751d4e61ffa3f668baf3585ad9880d18`

Original sources and game art are retained. Exact source hashes and verification records are in UPSTREAM.json.

License: No license file found upstream. Public source availability does not imply a reuse license.


## Controls

Original menus require mouse and classic snake uses WASD.1/2 choose input route in alternate mode, ESC exits. controllers/KBC/mouse/mouse.c starts576,432, applies complete packet unit deltas once and clamps1151,831 using game/UTILS.h SCREEN_HEIGHT832. No guest cursor reset found; direct menu taps are source-verified. Original source needs initial VM mouse reporting.

## Cursor edge memory safety

The original32×32 cursor retains logical bounds1129,839 and can be partly clipped at a frame edge. GPU pixel writes now reject offscreen coordinates and copy exactly the current pixel format byte count, removing out-of-frame and24-bit final-pixel overwrites without changing logical pointer/aim positions or RGB packing. Guarded public GPU regressions reproduce both failures independently. Prior source corrections and original art remain retained.
