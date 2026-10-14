# Minix compatibility trial

The acceptance target is unchanged C sources and imports for the two supplied
archives, the initial public projects, at least 15 additional repositories,
and original student labs. A compile pass, a bounded
runtime pass, and an inspected interactive result are separate verdicts.

## Work sequence

1. Record original source hashes, Makefile inputs, and missing APIs.
2. Add a separate Minix/LCF SDK and a declarative project builder. Preserve the
   native SDK and original project files.
3. Test IRQ hooks, register access, BIOS mode information, mapped VRAM, page
   flips, XPM formats, startup, and cleanup through their public interfaces.
4. Build and replay the supplied projects and pinned GitHub corpus. Inspect
   frames and the SDL path. Record failures without calling a compile a run.
5. Run the full verification command, publish the compatibility limits in the
   docs, and make lcom-ng the main name while retaining existing command aliases.

The course cross-check is `~/dev/resumos/src/content/lessons/lc/`, particularly
the interrupt, timer, keyboard, mouse, RTC, video, and serial lessons. It explains
device contracts; current assessment rules remain the instructor's responsibility.

The decision trail is [compatibility-decisions.tsv](compatibility-decisions.tsv).
Downloaded source and generated evidence belong under ignored `build/`.

## Expanded student trial

The second acceptance target adds at least 15 distinct pinned public repositories,
real Minix lab callbacks, and a relocated macOS distribution. Count successful
compilation, normal exits, bounded frames and interactive checks separately.
Keep upstream source defects in the results instead of editing downloaded code.

1. Download and pin additional repositories in ignored `build/compat-expanded/`.
2. Reproduce parser, legacy SDK and device failures at public command boundaries.
3. Exercise timer, keyboard, mouse stream/remote/gesture and graphics callbacks.
4. Package the SDL runtime with its shared libraries and SDK. Build unchanged
   projects and labs using the host toolchain outside Devenv.
5. Independently inspect captures, rerun existing corpus and final verification,
   and document the exact student installation and commands.
