# Run an existing LCOM project

[Install lcom-ng](/students/install), then open a terminal in the folder with your
project's original Makefile. Keep the full source tree, including assets and shared
code in sibling labs.

## Build and run

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

The builder prints the executable path, named from `PROG` in the Makefile; replace
`proj` if needed. Run from the project directory so relative assets resolve.

A window opens for graphical programs. Click to capture the mouse; Right Option on
Mac or Right Ctrl releases it. Keyboard, mouse and timers produce the expected
virtual interrupts. Rebuild and restart after edits.

Sources, `<lcom/lcf.h>` imports and the Makefile stay unchanged. lcom-ng rebuilds
the sources declared in the Makefile. Existing Minix executables and libraries need
rebuilding.

## Run original labs

Pass the LCF function and arguments after the executable:

```sh
cd labs/lab2
lcom-ng compat build .
lcom-ng run -- .lcom-ng/lab2 timer_test_int 2
```

For a keyboard lab:

```sh
cd ../lab3
lcom-ng compat build .
lcom-ng run -- .lcom-ng/lab3 kbd_test_scan
```

Focus the window and press Escape to finish. Use your assignment's function names
and arguments. Native `lcom-ng test` checks belong to the
[generated workspace](/students/), not Minix callbacks.

## Try a game

With Git installed:

```sh
git clone https://github.com/teixeiraa05/lcom-sushi-slash.git
cd lcom-sushi-slash/proj
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Choose Singleplayer; keep the sibling lab folders from the clone.

See the [compatibility reference](/developers/from-minix) for unusual Makefiles,
libraries, asset paths, APIs and limits.
