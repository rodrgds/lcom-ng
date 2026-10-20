# Run LCOM on a Mac

[Download the macOS archive](https://github.com/rodrgds/lcom-ng/releases) for your
processor. It includes the runtime, both SDKs, lab checks and SDL libraries; keep
the directory intact. The [general install guide](/students/install) covers all
platforms.

The arm64 build requires macOS 14; check `bundle.json` for your archive. The trial
does not prove Intel or other platforms.

## Install once

Install
[Apple's Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools/):

```sh
xcode-select --install
```

Verify the toolchain:

```sh
clang --version
make --version
python3 --version
```

The compatibility builder needs Python 3.9 or newer and its standard library. No
Homebrew, Nix, Devenv, SDL install or Minix VM is needed. If your tools omit
Python 3, install it from
[python.org](https://www.python.org/downloads/macos/).

With the archive in Downloads:

```sh
mkdir -p "$HOME/.local/share" "$HOME/.local/bin"
tar -xzf "$HOME/Downloads/lcom-ng-macos-arm64.tar.gz" -C "$HOME/.local/share"
ln -sfn "$HOME/.local/share/lcom-ng-macos-arm64/bin/lcom-ng" "$HOME/.local/bin/lcom-ng"
export PATH="$HOME/.local/bin:$PATH"
lcom-ng --help
```

Add the `export PATH=...` line to `~/.zshrc`. On Intel, substitute `x86_64` for
`arm64`. The bundle uses ad-hoc signatures and is not notarized; approve a blocked
download through Privacy & Security.

## Run an existing Minix lab

Pass the directory containing the lab Makefile. C files, includes and the Makefile
stay unchanged:

```sh
lcom-ng compat build "$HOME/lcom/labs/lab2"
lcom-ng run -- "$HOME/lcom/labs/lab2/.lcom-ng/lab2" timer_test_read_config 0 0
lcom-ng run -- "$HOME/lcom/labs/lab2/.lcom-ng/lab2" timer_test_int 2
```

The first argument after the binary selects the original LCF function; the rest are
its arguments. The binary name comes from `PROG` in the Makefile. For a keyboard
lab:

```sh
lcom-ng compat build "$HOME/lcom/labs/lab3"
lcom-ng run -- "$HOME/lcom/labs/lab3/.lcom-ng/lab3" kbd_test_scan
```

Focus the window, press keys, and press Escape to finish. Rebuild after each change.
Use `--headless` with a replay script for automated input tests. See the
[compatibility guide](/developers/from-minix) for unusual Makefiles and API limits.

## Run an existing final project

```sh
lcom-ng compat build "$HOME/lcom/labs/proj"
lcom-ng run -- "$HOME/lcom/labs/proj/.lcom-ng/proj"
```

Keyboard and mouse events become the expected virtual interrupts. Keep assets beside
the sources. The builder rebases `/home/lcom/labs/` paths; use
`--asset-root /path/to/labs` when needed.

## Run two serial peers

Build each program first, then connect their virtual UARTs:

```sh
lcom-ng run-pair --realtime --timeout 60 /path/to/peer1 --right /path/to/peer2
```

Use each program's arguments after its binary. The paired runtime connects COM1 to
COM1 and COM2 to COM2. `--realtime` advances with host time, which suits
interactive programs and polling peers.

## Use the native course labs

For the native starter and predefined checks:

```sh
lcom-ng setup "$HOME/lcom-student"
cd "$HOME/lcom-student"
make
lcom-ng test rtc
```

Fresh TODO implementations fail checks with the assertion they must satisfy. After
implementing, run that device's check:

```sh
lcom-ng test timer
lcom-ng test kbd
lcom-ng test mouse
lcom-ng test graphics
lcom-ng test audio
lcom-ng test uart
```

The workspace contains native headers, a static library, tests and input scripts,
and builds without the developer checkout. These labs have their own API contracts;
FEUP Minix labs use `compat build`. See the [student guide](/students/) for the
edit/test loop.

## Diagnose a build or crash

Compiler errors identify the source or link stage. Runtime crashes name the
terminating signal. `--timeout 30` stops busy loops after 30 host seconds with exit
code 124; `--max-ticks` bounds virtual time separately. For source diagnostics:

```sh
lcom-ng compat build "$HOME/lcom/labs/proj" --debug --sanitize undefined
lcom-ng run -- "$HOME/lcom/labs/proj/.lcom-ng/proj"
```

Use `--debug` alone for a debugger build. The builder checks sanitizer startup and
reports unsupported host runtimes before running student code.

## What this trial verified

A relocated arm64 bundle with PATH restricted to `/usr/bin:/bin:/usr/sbin:/sbin`
built and ran Ninjix Lab 1, the timer lab and Minixsweeper. All seven fresh TODO
checks failed at useful assertions; all passed with reference implementations. The
keyboard lab received make/break codes `0x1E`/`0x9E` and Escape `0x81`. All 17
SDL/font libraries use relative load paths. No Intel or older Mac was available for
a live trial.
