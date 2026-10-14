# Run an existing LCOM project

[Install lcom-ng](/students/install), then open a terminal in the folder containing
your project's original Makefile. Keep the complete source tree, including assets
and shared code in sibling lab directories.

## Build and run

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

The builder prints the executable path. Its filename comes from `PROG` in the
original Makefile, so replace `proj` if your project uses another name. Run from
the project directory so relative asset paths continue to work.

A window opens for graphical programs. Click inside it to capture the mouse.
Keyboard, mouse and timers produce the virtual device interrupts your program expects.
Edit your C code, run `compat build` again, and restart the program.

Your sources, `<lcom/lcf.h>` imports and Makefile stay unchanged. lcom-ng reads
the Makefile's source declarations and rebuilds them for your computer. Existing
Minix executables and compiled libraries need rebuilding.

## Run original labs

Pass the usual LCF function and arguments after the executable:

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

Focus the window and press Escape to finish. Function names and arguments follow
your original lab assignment. Native `lcom-ng test` checks belong to the
[generated native workspace](/students/), not the original Minix lab callbacks.

## Try a game

With Git installed:

```sh
git clone https://github.com/teixeiraa05/lcom-sushi-slash.git
cd lcom-sushi-slash/proj
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Choose Singleplayer in the game window. Keep the sibling lab folders from the clone.

See the [full compatibility reference](/developers/from-minix) for unusual
Makefiles, local libraries, legacy asset paths, supported APIs and known limits.
