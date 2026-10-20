# Updating the example screenshots

Galleries are real framebuffer captures from the same C programs you run
interactively. Each screenshot has a replay, a named state, and a source
fingerprint. Captures run in fresh fixture directories.

## Regenerate

From the repository root:

```sh
devenv shell -- python3 scripts/capture_gallery.py
```

The command builds the runtime and examples, captures the states in
`scripts/showcase-gallery.json`, and writes PNGs plus metadata under
`docs/assets/screenshots/gallery/`. Review images before committing. The docs site
copies these assets during its normal build.

For a focused edit:

```sh
devenv shell -- python3 scripts/capture_gallery.py --app pixel-studio
devenv shell -- python3 scripts/capture_gallery.py --app pixel-studio --state help
```

Use `--list` for app and state names. Add `--no-build` when executables are
current. `--check` validates image integrity and source freshness without running
programs; a stale image reports which app to regenerate.

## Add a useful state

Add a state to the app's entry in `scripts/showcase-gallery.json`. Give it a
stable `slug`, short `title`, task-specific `caption`, finite `ticks` budget, and
a `script` of ordinary replay commands. Optional `fixtures` create text files in
the capture directory. `args` can override default arguments. Paired apps can
also supply `right_script`, `right_args`, and `capture_side`.

Capture an actual difference: an edit, meaningful data, a paused run, a finished
export, a recoverable error, or a protocol result. A new timestamp alone does not
need a screenshot. Keep captions literal.

Snapshots use `--dump-presented` to keep a complete presented frame, excluding
unfinished drawing in shared memory. A snapshot can stop at the virtual tick guard
to retain an open help panel or paused state; metadata calls this a **bounded
snapshot**. The command accepts that guard only when a valid framebuffer exists.
An authored failure case must declare its expected exit status and matching output
assertions. Other runtime failures fail the command. Normal exit and bounded
snapshot evidence remain separate.

## Other canonical media

```sh
devenv shell -- sh scripts/capture_showcase_examples.sh
devenv shell -- sh scripts/capture_marketing_examples.sh
devenv shell -- sh scripts/render_public_demo.sh
```

See [the video production notes](https://github.com/rodrgds/lcom-ng/blob/main/docs/video-demo.md)
for the film's source and audio pipeline.
