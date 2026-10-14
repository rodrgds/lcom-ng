# Download and install

Download the archive matching your computer from
[GitHub Releases](https://github.com/rodrgds/lcom-ng/releases). Keep the extracted
folder intact, including `bin/`, `sdk/` and the bundled desktop libraries.

| Computer | Archive |
| --- | --- |
| Apple Silicon Mac | `lcom-ng-macos-arm64.tar.gz` |
| Intel Mac | `lcom-ng-macos-x86_64.tar.gz` |
| Linux Intel / AMD 64-bit | `lcom-ng-linux-x86_64.tar.gz` |
| Linux ARM 64-bit | `lcom-ng-linux-arm64.tar.gz` |
| Windows Intel / AMD 64-bit, MSYS2 | `lcom-ng-windows-msys2-x86_64.tar.gz` |

Published archives include the runtime, both SDKs, native lab starters and checks,
examples, and SDL libraries. They are built and tested by GitHub Actions. A matching
`.sha256` file verifies each download. Before the first tagged release, maintainers
can supply the archives from a successful Actions build.

## macOS

Install Apple's compiler tools once, unless they are already installed:

```sh
xcode-select --install
```

For an Apple Silicon Mac, with the archive in Downloads:

```sh
mkdir -p "$HOME/.local/share"
tar -xzf "$HOME/Downloads/lcom-ng-macos-arm64.tar.gz" -C "$HOME/.local/share"
export PATH="$HOME/.local/share/lcom-ng-macos-arm64/bin:$PATH"
lcom-ng --help
```

On an Intel Mac, replace `arm64` with `x86_64`. Add the `export PATH=...` line to
`~/.zshrc` to keep the command available in new terminals. The build tools provide
clang and make. Verify `python3 --version` is 3.9 or newer; install Python from
[python.org](https://www.python.org/downloads/macos/) if it is missing.

The archives use ad-hoc signatures and are not notarized. If macOS blocks a trusted
download, follow its Privacy & Security approval flow. See the
[macOS guide](/students/macos) for troubleshooting and the local trial evidence.

## Linux

The Linux archives target Ubuntu 24.04 or a compatible distribution with glibc 2.39
or newer. Use a desktop session for interactive games. On Ubuntu, install the source
build tools:

```sh
sudo apt-get update
sudo apt-get install build-essential python3
mkdir -p "$HOME/.local/share"
tar -xzf "$HOME/Downloads/lcom-ng-linux-x86_64.tar.gz" -C "$HOME/.local/share"
export PATH="$HOME/.local/share/lcom-ng-linux-x86_64/bin:$PATH"
lcom-ng --help
```

Replace `x86_64` with `arm64` on ARM Linux. Add the export line to `~/.bashrc` or
`~/.zshrc`, depending on your shell. SDL libraries and the terminal font are bundled;
you do not need to build lcom-ng or install SDL development packages.

## Windows with MSYS2

Use the Windows archive in an [MSYS2](https://www.msys2.org/) **MSYS** terminal.
This release uses MSYS2's POSIX runtime, not a standalone PowerShell executable.
Do not mix it with a MinGW or UCRT native compiler.

Install MSYS2, open the MSYS terminal, and install the source build tools:

```sh
pacman -Syu
```

If MSYS2 asks you to close the terminal, reopen it and complete the update. Then:

```sh
pacman -S --needed base-devel gcc python
mkdir -p "$HOME/.local/share"
tar -xzf /c/Users/YOUR_NAME/Downloads/lcom-ng-windows-msys2-x86_64.tar.gz -C "$HOME/.local/share"
export PATH="$HOME/.local/share/lcom-ng-windows-msys2-x86_64/bin:$PATH"
lcom-ng --help
```

Replace `YOUR_NAME` with your Windows account directory. Add the export line to
`~/.bashrc`. Build and run inside this same MSYS terminal. SDL and runtime DLLs are
included in the archive. Windows ARM is not a release target yet.

## Run your code

For an existing Minix project, enter the folder containing its original Makefile:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Use the executable path printed by the build command. Follow the
[compatibility quick start](/students/compatibility) for games and original labs.

For a native workspace:

```sh
lcom-ng setup student
cd student
make
lcom-ng run -- build/student_project
lcom-ng test rtc
```

Fresh native lab checks fail until you implement the TODOs. Continue with the
[native student guide](/students/) and [lab track](/labs/).
