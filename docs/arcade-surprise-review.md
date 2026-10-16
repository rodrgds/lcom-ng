# Arcade roulette, sounds and keyboard review

Verified on 2026-10-16. Web source revision:
`486bc2c791230f9c7e0f9a44d83b448ab60f0bd4`.

## Delivered behavior

- **Surprise me** accelerates through actual game icons, then slows to a uniformly
  selected catalogue game. Play launches it; Spin again chooses another result.
  Skip, Escape, backdrop dismissal and closing stop the animation safely.
- Reduced motion reveals the result immediately. Resizing preserves the selected
  icon's alignment with the marker. Portrait and landscape keep actions visible.
- Cuelume 0.2.8 is bundled locally with its MIT notice and reproducible provenance.
  UI sounds default off, use volume 0.35, and remember the sound preference.
  Muting stops ringing voices and synchronizes both controls and other tabs.
  Reloading never autoplays. No audio downloads or remote dependencies are needed.
- Search has no heading or close button. Ctrl+K, Command+K and `/` open it.
  Escape or the backdrop dismiss it, preserving the query and restoring focus.
  Shortcuts do not interfere with guest games, typing `/`, or another modal.
- All four arrows navigate the rendered main mosaic and search grid. Featured
  cards use their resting geometry; secondary Repo links keep normal Tab access.
- Featured placement favors unused columns when utility tiles displace targets.
  Shuffle retains varied interior positions rather than repeating two columns.

## Verification

The focused browser suite passed all 13 tests against the isolated release
snapshot. It covers real animation geometry and timing, selection and launch,
return focus, cancellation, actual Web Audio output and immediate mute,
preference persistence, unavailable audio, reduced motion, touch resizing,
spatial arrows, shortcut ownership, previews and repeated shuffles.

```sh
devenv shell -- bunx playwright test tests/collection_web.spec.mjs tests/collection_navigation.spec.mjs
devenv shell -- bunx playwright test tests/web_export.spec.mjs -g 'collection|normal guest exits'
devenv shell -- python3 scripts/check_collection_sources.py --require-tracked
devenv shell -- bun run docs:build
```

The five export checks passed, including the installed SDK payload, guest exit,
loading cancellation/retry, phone icons and failed module recovery. The source
inventory verified 66 games, 7,757 pinned files and 37 recorded corrections across
83 files. The documentation build and owned-diff whitespace checks passed.

A landscape regression first reproduced actions below the dialog's visible
bounds, then passed after the compact layout. A release run also reproduced a
shuffle with only two featured columns; preferring unused columns fixed it.
The final suite checks five further shuffles. The earlier roulette and keyboard
regressions failed against the unchanged launcher before their implementations.

Concurrent Arcade expansion and runtime edits were preserved. The release used
an immutable base SDK plus only this change's files. Two Devenv startup calls
stalled during shared Nix contention; packaging and final verification used the
same resolved tools from `.devenv/profile/bin`.

## Publication

- Published to <https://arcade.rgo.pt/> with Worker version
  `30c394fa-93bb-4e3f-9d36-c333331cf5d8`.
- All 15 checked public launcher, catalogue, module and attribution files match
  the release package byte for byte.
- Live Chrome checks passed compact search, both shortcuts, spatial search arrows,
  query retention, roulette selection, synchronized mute, a real game's first
  frame and return focus. The selected game was Star Defender.
- Live touch checks passed portrait, landscape, reduced motion, visible actions
  and search. No page errors occurred.
- Manual in-app browser review confirmed the actual icons and compact search.
  Right, Down, Left and Up moved between the visible main-grid utility cells.

These changes affect the browser collection launcher. Game binaries and the
previously delivered Android APK retain their existing verification scope.
