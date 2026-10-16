# Memory Match

Original student project: https://github.com/iamaccosta/FEUP-LCOM

Pinned revision: `977a5cee882ba8016f0c6535512611a44bb64e12`.

Student source and required art are preserved byte for byte except explicitly recorded corrections below. lcom.json and icon.png are collection additions. Unrelated reports, generated documentation, build products and Git history are omitted.

License: No license file was supplied by this upstream revision; source availability does not grant redistribution permission.

Cursor corrections: consume each complete packet once, preserve the original half-speed motion, clamp its applied coordinates and clip the original cursor sprite at frame edges. `SOURCE_PATCH.diff` and `UPSTREAM.json` record the exact changes and original hashes. Browser desktop calibration is explicit in `lcom.json`; mobile relative touch remains available.

The shorter background image now repeats its final edge across uncovered framebuffer pixels, so moving the cursor away from the bottom edge clears its previous pixels. Original image assets are unchanged.
