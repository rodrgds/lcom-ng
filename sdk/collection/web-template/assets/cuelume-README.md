# Arcade UI sounds

`../cuelume.js` is a browser ESM bundle of the MIT-licensed Cuelume 0.2.8
package. It contains all fourteen current cues and four themes. It has no
external imports, audio downloads, or runtime dependencies. The collection
exporter and installed SDK copy it and these attribution files automatically.

The package's license is in `cuelume-LICENSE.txt`. Its npm integrity, upstream
revision, bundle hash and build command are in `cuelume-provenance.json`.

## App integration

Import from `../audio.js`:

- `initSound(button)`: bind a sound toggle once. Multiple buttons share state.
  Optional `svg[data-sound-on]`, `svg[data-sound-off]` and a label `span` update
  together. Buttons expose `aria-pressed` and Enable/Mute UI sounds labels.
- `cue(name, options)`: request a current Cuelume cue, such as `tap`, `select`,
  `open`, `close`, `navigate`, `ready` or `count`. Options pass through unchanged.
- `unlockSound()`: call in the native gesture starting an interaction. Resolves
  whether sound can run. Muted users return false without creating an audio context.
- `setSoundEnabled(boolean)`: change, synchronize and persist the preference.

Sound defaults off. The preference is stored as `true` or `false` under
`lcom-arcade-sound`. Storage failures keep a working choice for the current visit.
Changes in another tab synchronize through the storage event. Browsers without
Web Audio show a disabled unavailable sound toggle; the other controls still work.
An unsuccessful unlock resets sound off instead of claiming that audio is active.
Muting also fades any Cuelume voices already ringing. UI sound volume is 0.35.

The toggle plays its own subtle `toggle` confirmation after audio unlocks. Do
not also bind a generic tap to it. Callers own meaningful cue selection and
cadence. Roulette crossings use `select` with `theme: 'mech'` and
`emphasis: 'subtle'`, at most eight per second. A landing can use `ready` with
`theme: 'bubble'`. Avoid cues for ordinary hover or background work.

The current upstream guide is <https://cuelume.dev/agents.md>.

## Rebuild the vendored module

Run from the repository root. Dependencies stay in an ignored build folder:

```sh
mkdir -p build/arcade-cuelume
printf '%s\n' '{"private":true,"type":"module"}' > build/arcade-cuelume/package.json
devenv shell -- bun add --cwd build/arcade-cuelume --exact cuelume@0.2.8
printf '%s\n' 'export {bind, play, press, release, setEnabled, setTheme, setVolume, sounds, themes, unlock} from "cuelume";' > build/arcade-cuelume/browser-entry.js
devenv shell -- bun build build/arcade-cuelume/browser-entry.js --target browser --format esm --minify --outfile sdk/collection/web-template/cuelume.js
cp build/arcade-cuelume/node_modules/cuelume/LICENSE sdk/collection/web-template/assets/cuelume-LICENSE.txt
```

Use the project's Bun 1.3.13. Bundle the explicit browser entry, rather than the
package's internal re-export file directly. Verify the exported cue/theme lists,
check the bundle hash against provenance, and run the collection browser tests.
For an intentional upgrade, update the pinned package version, integrity,
upstream revision and generated bundle hash together.
