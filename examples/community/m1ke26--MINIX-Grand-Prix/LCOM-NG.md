# MINIX Grand Prix

Original student project from [m1ke26/MINIX-Grand-Prix](https://github.com/m1ke26/MINIX-Grand-Prix), pinned at `c246fb4ba19820940f399155d23eb814b961a9cf`.

Build from `project` with `lcom-ng compat build .`, or export using
`lcom-ng export web .` / `lcom-ng export android .`. The adjacent `lcom.json`
provides launcher identity, assets and mobile controls.

Mobile uses a steering wheel for A/D, separate W acceleration and S brake/reverse
pedals, Space drift and Shift boost. Tap Start in the menu, then use Start race
for the Enter countdown. Direct touch follows the menu cursor at its verified
400×300 start within the 800×600 framebuffer.

Original game assets remain unchanged. The driving-input correction consumes key
releases while paused, preventing stuck controls after Resume. `UPSTREAM.json`
records original and corrected hashes; `SOURCE_PATCH.diff` preserves the exact
change and reverses to the pinned upstream source.

No license found in upstream tree; attribution retained, redistribution permission not established.
