# Arcade UI sounds

`../cuelume.js` is a browser ESM bundle of the MIT-licensed Cuelume 0.2.8
package. It contains all fourteen current cues and four themes. It has no
external imports, audio downloads, or runtime dependencies. The collection
exporter and installed SDK copy it and these attribution files automatically.

The package's license is in `cuelume-LICENSE.txt`. Its npm integrity, upstream
revision, bundle hash and build command are in `cuelume-provenance.json`.

## App integration

Import `cue(name, options)` and `unlockSound()` from `../audio.js`.
Cues pass their options to Cuelume. Call `unlockSound()` in the gesture that starts
an interaction. Browsers without Web Audio remain usable and silently skip cues.

UI sounds are enabled at volume 0.35. Audio starts after interaction, never on
page load. The browser tab's mute control is the user's sound preference. The
launcher has no separate sound toggle or stored setting.

Callers own meaningful cue selection and cadence. Roulette crossings use
`select` with `theme: 'mech'` and `emphasis: 'subtle'`, at most eight per second.
Landing uses `ready` with `theme: 'bubble'`. Ordinary hover and background work
remain silent. The current guide is <https://cuelume.dev/agents.md>.

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
