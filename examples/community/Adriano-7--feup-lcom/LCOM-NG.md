# Space Invaders

Original student project from [Adriano-7/feup-lcom](https://github.com/Adriano-7/feup-lcom), pinned at `e0b46670ed2122e2ad303a7d5ed83e51b517d215`.

Build from `proj/src` with `lcom-ng compat build .`, or export using
`lcom-ng export web .` / `lcom-ng export android .`. The adjacent `lcom.json`
provides launcher identity, assets and mobile controls.

Left/right move and Space fires. Cursor has nonzero minimum margin and resets for game-over, so relative mouse menu.

Original game assets remain byte-identical. `SOURCE_PATCH.diff` records the minimal
confirmed animation initialization fix; all other source files are unchanged. `UPSTREAM.json`
records their hashes, selected upstream revision and omitted development files.

No license found in upstream tree; attribution retained, redistribution permission not established.
