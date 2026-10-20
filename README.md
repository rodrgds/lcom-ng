# lcom-ng

Run LCOM labs and C projects on your own computer, without a Minix VM.

[Download](https://github.com/rodrgds/lcom-ng/releases) · [Install guide](https://lcom.rgo.pt/students/install) · [Documentation](https://lcom.rgo.pt/)

The browser Arcade is the current focus at <https://arcade.rgo.pt/>. Android
builds, emulator qualification and publication are paused; the implementation and
signing keys are preserved for later builds.

## Quick start

Download the archive for your platform from **GitHub Releases**, extract it, and
add its `bin/` directory to `PATH`. Keep the whole folder: it includes the SDK and
desktop libraries. Building needs a C compiler, Python 3 and `make`. See the
[install guide](https://lcom.rgo.pt/students/install) for platform steps.

Commands below assume `lcom-ng` is on `PATH`. Inside a repository checkout,
prefix them with `devenv shell --`.

### Existing Minix / LCOM project

In the directory containing your original `Makefile`:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Use the binary path printed by the build command. Its name comes from `PROG`.
Your C files, includes and Makefile stay unchanged. Keep the complete project
tree, including assets and sibling lab libraries, and build again after edits.

Original labs use their usual LCF arguments:

```sh
cd labs/lab2
lcom-ng compat build .
lcom-ng run -- .lcom-ng/lab2 timer_test_int 2
```

[Compatibility quick start](https://lcom.rgo.pt/students/compatibility) ·
[Tested projects and known failures](docs/compatibility-recent.md)

### Native lcom-ng project

```sh
lcom-ng setup student
cd student
make
lcom-ng run -- build/student_project
lcom-ng test rtc
```

Edit `proj/` for your application and `labs/` for exercises. Fresh lab checks fail
until you implement the TODOs. The native API (`lcom_init`, `lcom_event_wait`) is
the path for new work that does not need Minix service and IPC conventions.

[Student guide](https://lcom.rgo.pt/students/) ·
[Lab track](https://lcom.rgo.pt/labs/)

### Browser games and the Arcade

Releases export a game as static HTML, JavaScript, WebAssembly and assets,
including a touch keyboard for mobile players:

```sh
lcom-ng web path/to/my-game
```

Use `lcom-ng web .` for your own project. To write static files instead of
serving them, use `lcom-ng export web . --output dist/game`. The
[browser guide](https://lcom.rgo.pt/students/browser) covers project layouts,
hosting and current limits.

The [LCOM Arcade](https://arcade.rgo.pt/) is the web showcase for student games
and the Ninjix showcase. Its source and manifest live in
[`collections/arcade/`](collections/arcade/README.md).

### Android apps (paused)

```sh
lcom-ng export android examples/flappy --output dist/flappy-android
```

See the [Android guide](https://lcom.rgo.pt/students/android) for the preserved
exporter, build tools, touch controls and verification limits.

## See it running

<p>
  <img src="docs/assets/screenshots/word-processor.png" width="49%" alt="Word processor running in lcom-ng">
  <img src="docs/assets/screenshots/music-maker.png" width="49%" alt="Music maker running in lcom-ng">
</p>
<p>
  <img src="docs/assets/screenshots/breakout.png" width="49%" alt="Breakout running in lcom-ng">
  <img src="docs/assets/screenshots/flappy.png" width="49%" alt="Flappy Bird running in lcom-ng">
</p>

[Watch the demo](docs/assets/demo/machine-lab-demo.mp4) · [More examples](examples/README.md)

## What it supports

| Area | Devices and workflow |
| --- | --- |
| Labs | RTC, timer, keyboard, mouse, graphics, audio and UART |
| Projects | Games, editors, sequencers and paired serial programs |
| Interactive use | SDL display, keyboard, mouse and audio |
| Automated checks | Headless runs, input replay, traces and captured frames |
| Existing course code | Separate Minix/LCF compatibility SDK and Makefile importer |
| Browser exports | Static web builds and playable game collections |

Compatibility is checked against pinned student projects and original lab code.
See the [results](docs/compatibility-results.md),
[expanded corpus](docs/compatibility-expanded.md) and
[course audit](docs/course-compatibility-audit.md) for coverage and limits. This
runtime models the course devices; it does not emulate Minix.

The CLI is `lcom-ng`. Existing `lcom`, `machinelab`, `lowlab` and `mlab` aliases
remain supported, along with the public `<lcom/...>` headers.

## For instructors and developers

[Adoption](https://lcom.rgo.pt/instructors/adoption) ·
[Instructor guide](https://lcom.rgo.pt/instructors/) ·
[Developer guide](https://lcom.rgo.pt/developers/) ·
[Contributing](CONTRIBUTING.md)

To build lcom-ng itself:

```sh
devenv shell -- lcom-ng-install
devenv shell -- lcom-ng-verify
```

The package-local [developer guide](docs/developer-guide.md) covers source
builds and repository structure.

## License

Code: [MIT](LICENSE). Course material, docs and media:
[CC BY 4.0](course/LICENSE.md).

Projects can add [`lcom.json`](https://lcom.rgo.pt/students/project-config)
for app names, icons and mobile controls.
