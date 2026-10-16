
Cursor rendering correction: Keep the entire loaded cursor sprite visible at each edge by clamping its draw origin, preserving logical click coordinates. Verified by `test_teclas_draws_complete_cursor_without_changing_hotspot` with the actual student rendering owner.
