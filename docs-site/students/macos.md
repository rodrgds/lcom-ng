# Run LCOM on a Mac

[Download the macOS archive](https://github.com/rodrgds/lcom-ng/releases) for your processor. It includes the runtime, both C SDKs, native lab checks and SDL libraries. Keep the extracted directory intact. The [general install guide](/students/install) covers all platforms.

The arm64 trial bundle was exercised on macOS 26.6.2 and declared macOS 14 as its minimum. Check `bundle.json` for the minimum version of the archive you download. Intel and other platform builds have their own release workflow gates; the local arm64 trial does not prove those platforms.

## Install once

Install [Apple's Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools/):

```sh
xcode-select --install
```

Finish the installer, then verify the host compiler, make, and Python:

```sh
clang --version
make --version
python3 --version
```

The compatibility builder uses Python 3.9 or newer and its standard library.
Apple's Python 3.9.6 was tested with a clean system PATH. No Python packages,
Homebrew, Nix, Devenv, SDL installation, or Minix VM are needed for this bundle.
[Python's macOS guide](https://docs.python.org/3.13/using/mac.html) describes the
Apple development-tools Python. If your tools omit Python 3, install it from
[python.org](https://www.python.org/downloads/macos/).

With the archive in Downloads:

```sh
mkdir -p "$HOME/.local/share" "$HOME/.local/bin"
tar -xzf "$HOME/Downloads/lcom-ng-macos-arm64.tar.gz" -C "$HOME/.local/share"
ln -sfn "$HOME/.local/share/lcom-ng-macos-arm64/bin/lcom-ng" "$HOME/.local/bin/lcom-ng"
export PATH="$HOME/.local/bin:$PATH"
lcom-ng --help
```

Add the `export PATH=...` line to `~/.zshrc` for future Terminal sessions. Intel
Mac users substitute `x86_64` for `arm64` in the archive and directory names.
The bundle uses ad-hoc signatures and has not been notarized. If macOS blocks a
trusted download, approve it through Privacy & Security before
running it again.

## Run an existing Minix lab

Pass the directory containing the original lab Makefile. The C files, includes,
and Makefile stay unchanged:

```sh
lcom-ng compat build "$HOME/lcom/labs/lab2"
lcom-ng run -- "$HOME/lcom/labs/lab2/.lcom-ng/lab2" timer_test_read_config 0 0
lcom-ng run -- "$HOME/lcom/labs/lab2/.lcom-ng/lab2" timer_test_int 2
```

The first argument after the binary selects the original LCF test function; the remaining values
are its arguments. Use the lab's original function name. The generated binary
name comes from `PROG` in its Makefile. For a keyboard lab:

```sh
lcom-ng compat build "$HOME/lcom/labs/lab3"
lcom-ng run -- "$HOME/lcom/labs/lab3/.lcom-ng/lab3" kbd_test_scan
```

Focus the SDL window, press keys, and press Escape to finish the keyboard test.
Edit your implementation and repeat `compat build` after each change. Use
`--headless` for automated runs, with a replay script for input-dependent tests.
See the [compatibility guide](/developers/from-minix) for unusual Makefiles and API limits.

## Run an existing final project

```sh
lcom-ng compat build "$HOME/lcom/labs/proj"
lcom-ng run -- "$HOME/lcom/labs/proj/.lcom-ng/proj"
```

The runtime opens the SDL display. Keyboard and mouse events become the same
virtual device interrupts the original C program expects. Keep project assets
beside the original sources. The builder rebases `/home/lcom/labs/` asset paths
to the inferred workspace. Use `--asset-root /path/to/labs` at build time when
the source layout needs an explicit root.

## Run two serial peers

Build each original program first, then connect their virtual UARTs:

```sh
lcom-ng run-pair --realtime --timeout 60 /path/to/peer1 --right /path/to/peer2
```

Use the original program's arguments after each binary. The paired runtime
connects COM1 to COM1 and COM2 to COM2. `--realtime` advances with host time,
which is useful for interactive programs and peers with polling loops.

## Use the native course labs

For the lcom-ng course starter and predefined checks:

```sh
lcom-ng setup "$HOME/lcom-student"
cd "$HOME/lcom-student"
make
lcom-ng test rtc
```

The starter compiles. Fresh TODO implementations fail checks with the specific
C assertion they need to satisfy. Implement the requested functions, then run
that device's check:

```sh
lcom-ng test timer
lcom-ng test kbd
lcom-ng test mouse
lcom-ng test graphics
lcom-ng test audio
lcom-ng test uart
```

The generated workspace contains its native headers, static library, predefined
tests, and input scripts. It builds independently of the developer checkout.
The [student guide](/students/) describes the edit/test loop. These native
course labs have their own API contracts; existing FEUP Minix labs use the
`compat build` route above.

## Diagnose a build or crash

Compiler errors identify the source or link stage. Runtime crashes name the
terminating signal. Add `--timeout 30` to a run to stop busy loops after 30 host
seconds with exit code 124. `--max-ticks` bounds virtual time separately. For source diagnostics, rebuild and run:

```sh
lcom-ng compat build "$HOME/lcom/labs/proj" --debug --sanitize undefined
lcom-ng run -- "$HOME/lcom/labs/proj/.lcom-ng/proj"
```

Use `--debug` alone for a debugger build. The builder checks sanitizer startup
and reports unsupported host runtimes before running student code.

## What this trial verified

A relocated arm64 bundle, with PATH restricted to `/usr/bin:/bin:/usr/sbin:/sbin`,
compiled and ran the supplied Ninjix Lab 1 and timer lab, and the original Minixsweeper game
with Apple's clang and Python. The native starter built, all seven fresh TODO checks
failed at useful assertions, and all seven native lab checks passed with the reference
implementations. The original Ninjix keyboard lab received real SDL A make/break
codes `0x1E`/`0x9E` and Escape break `0x81`, then exited normally. Its LCF output
file also contains those same messages. The bundle's 17 SDL/font dependency libraries use relative
load paths, and no load command points into Nix or Homebrew. No Intel or older
macOS machine was available for a live trial.
