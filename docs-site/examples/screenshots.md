# Updating the example screenshots

The galleries are real framebuffer captures from the same C programs you run
interactively. Each screenshot has a replay, a named task or result, and a
recorded source fingerprint. The capture command runs every replay in a fresh
fixture directory, so a file left by an earlier run cannot change the result.

## Regenerate

From the repository root:

```sh
devenv shell -- python3 scripts/capture_gallery.py
```

The command builds the runtime and examples, captures the states declared in
`scripts/showcase-gallery.json`, and writes PNGs plus capture metadata under
`docs/assets/screenshots/gallery/`. Review the images before committing them.
The docs site copies these assets during its normal build.

For a focused edit:

```sh
devenv shell -- python3 scripts/capture_gallery.py --app pixel-studio
devenv shell -- python3 scripts/capture_gallery.py --app pixel-studio --state help
```

Use `--list` for the available app and state names. Add `--no-build` when the
executables are already current. Use `--check` to validate image integrity and
source freshness without running the programs. A stale image reports which app
to regenerate.

## Add a useful state

Add a state to the app's entry in `scripts/showcase-gallery.json`. Give it a
stable `slug`, a short `title`, a task-specific `caption`, a finite `ticks`
budget, and a `script` containing ordinary runtime replay commands. Optional
`fixtures` create text files in that capture's working directory. `args` can
override the app's default arguments. Paired apps can also supply `right_script`,
`right_args`, and `capture_side`.

Capture an actual difference: an editing tool, meaningful data, a paused run,
a finished export, a recoverable error, or a protocol result. A new timestamp
alone does not need another screenshot. Keep captions literal and avoid
claiming a successful export or delivery from an unfinished state.

Snapshots use `--dump-presented` to retain a complete presented frame, excluding
unfinished drawing in shared memory. A snapshot can deliberately stop at the virtual tick guard to retain an open
help panel or paused state. The metadata calls this a **bounded snapshot**.
The capture command accepts that specific guard only when a valid framebuffer
exists. An authored failure case must declare its expected exit status and matching
output assertions. Other runtime failures fail the command. Normal program exit and
bounded snapshot evidence remain separate.

## Other canonical media

The overview screenshots and launch footage also exercise artifact and protocol
checks:

```sh
devenv shell -- sh scripts/capture_showcase_examples.sh
devenv shell -- sh scripts/capture_marketing_examples.sh
devenv shell -- sh scripts/render_public_demo.sh
```

See [the video production notes](https://github.com/rodrgds/lcom-ng/blob/main/docs/video-demo.md)
for the film's source and audio pipeline.
