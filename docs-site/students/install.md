# Download and install

Download your computer's archive from
[GitHub Releases](https://github.com/rodrgds/lcom-ng/releases). Keep `bin/`, `sdk/`
and the bundled desktop libraries intact.

| Computer | Archive |
| --- | --- |
| Apple Silicon Mac | `lcom-ng-macos-arm64.tar.gz` |
| Intel Mac | `lcom-ng-macos-x86_64.tar.gz` |
| Linux Intel / AMD 64-bit | `lcom-ng-linux-x86_64.tar.gz` |
| Linux ARM 64-bit | `lcom-ng-linux-arm64.tar.gz` |
| Windows Intel / AMD 64-bit, MSYS2 | `lcom-ng-windows-msys2-x86_64.tar.gz` |

Archives include the runtime, both SDKs, lab starters and checks, examples and SDL
libraries. GitHub Actions builds and tests them; a matching `.sha256` verifies each
download.

## macOS

Install Apple's compiler tools once:

```sh
xcode-select --install
```

On Apple Silicon, with the archive in Downloads:

```sh
mkdir -p "$HOME/.local/share"
tar -xzf "$HOME/Downloads/lcom-ng-macos-arm64.tar.gz" -C "$HOME/.local/share"
export PATH="$HOME/.local/share/lcom-ng-macos-arm64/bin:$PATH"
lcom-ng --help
```

On Intel Mac, replace `arm64` with `x86_64`. Add the `export PATH=...` line to
`~/.zshrc`. Check `python3 --version` is 3.9 or newer; install Python from
[python.org](https://www.python.org/downloads/macos/) if missing.

Archives are ad-hoc signed and not notarized. If macOS blocks a download, use
Privacy & Security. See the [macOS guide](/students/macos) for troubleshooting.

## Linux

Targets Ubuntu 24.04 or compatible distributions with glibc 2.39 or newer. Use a
desktop session for interactive games. On Ubuntu, install the build tools:

```sh
sudo apt-get update
sudo apt-get install build-essential python3
mkdir -p "$HOME/.local/share"
tar -xzf "$HOME/Downloads/lcom-ng-linux-x86_64.tar.gz" -C "$HOME/.local/share"
export PATH="$HOME/.local/share/lcom-ng-linux-x86_64/bin:$PATH"
lcom-ng --help
```

Replace `x86_64` with `arm64` on ARM. Add the export line to `~/.bashrc` or
`~/.zshrc`. SDL and the terminal font are bundled; do not build lcom-ng or install
SDL development packages.

## Windows with MSYS2

Use the Windows archive in an [MSYS2](https://www.msys2.org/) **MSYS** terminal.
This release uses MSYS2's POSIX runtime, not a standalone PowerShell executable. Do
not mix it with a MinGW or UCRT native compiler.

Install MSYS2, open the MSYS terminal, then:

```sh
pacman -Syu
```

If asked to close the terminal, reopen it and finish the update. Then:

```sh
pacman -S --needed base-devel gcc python
mkdir -p "$HOME/.local/share"
tar -xzf /c/Users/YOUR_NAME/Downloads/lcom-ng-windows-msys2-x86_64.tar.gz -C "$HOME/.local/share"
export PATH="$HOME/.local/share/lcom-ng-windows-msys2-x86_64/bin:$PATH"
lcom-ng --help
```

Replace `YOUR_NAME` with your Windows account directory and add the export line to
`~/.bashrc`. Build and run in the same MSYS terminal. SDL and runtime DLLs are
included; Windows ARM is not a release target.

Use `lcom-ng compat build . --debug` for debug symbols. MSYS GCC lacks
AddressSanitizer; `--sanitize address` reports that the compiler cannot build with
it.

## Next steps

- Existing Minix project or lab:
  [compatibility quick start](/students/compatibility).
- Native lab workspace: [native student guide](/students/), then the
  [lab track](/labs/).
