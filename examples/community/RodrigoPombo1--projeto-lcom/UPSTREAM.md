
Cursor rendering correction: Clamp the centered32x32 cursor draw origin on initial and interrupt-driven redraws, preserving logical menu/game coordinates and full original cursor visibility. Verified by `test_leiria_cursor_is_visible_at_both_logical_corners` with the actual student rendering owner.
