# Star Defender

Original student project: https://github.com/luanalima27/LCOM-FEUP

Pinned revision: `8a51f4788f459f16e832668dd988ec4f42552fdf`.

Student source and required art are preserved byte for byte except explicitly recorded corrections below. lcom.json and icon.png are collection additions. Unrelated reports, generated documentation, build products and Git history are omitted.

License: No license file was supplied by this upstream revision; source availability does not grant redistribution permission.

Cursor edge correction: clamp to the same boundary on overshoot instead of moving five pixels back. Original and corrected hashes and the reversible patch are recorded in `UPSTREAM.json`.

Cursor rendering correction: Clip17x17 gameplay target blits at the framebuffer edges and reject equality bounds, preventing the actual ARM64 bottom-edge SIGSEGV while preserving logical aiming coordinates. Verified by `test_star_defender_xpm_clips_framebuffer_edges` with the actual student rendering owner.
