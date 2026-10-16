
Cursor rendering correction: Keep the entire mouse sprite visible by clamping only its draw origin. Preserve logical slot/tower targeting and other sprites. Verified by `test_tower_defense_draws_cursor_inside_screen_without_moving_slot_hotspot` with the actual student rendering owner.
