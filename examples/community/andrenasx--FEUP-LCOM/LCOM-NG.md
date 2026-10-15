# Asteroids: UFO Attack

Original student project from [andrenasx/FEUP-LCOM](https://github.com/andrenasx/FEUP-LCOM), pinned at `4c34535b3ed40a3f60dba1d2c63fbe6aec946883`.

Build from `proj/src` with `lcom-ng compat build .`, or export using
`lcom-ng export web .` / `lcom-ng export android .`. The adjacent `lcom.json`
provides launcher identity, assets and mobile controls.

WASD drives the UFO, left mouse button shoots, and right look stick aims. Aim sprite wraps on border collision, so relative mouse aiming is required.

Original game assets remain byte-identical. `SOURCE_PATCH.diff` records the minimal
confirmed short-click menu fix; all other source files remain unchanged. `UPSTREAM.json`
records their hashes, selected upstream revision and omitted development files.

No license found in upstream tree; attribution retained, redistribution permission not established.
