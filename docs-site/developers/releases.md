# Build a release

GitHub Actions builds the student archives. A release contains the SDL runtime,
compatibility builder, both SDKs, native lab starters and checks, examples,
desktop libraries and license notices. Students install an archive and follow the
[quick start](/students/install).

## Release targets

| Archive | Build environment |
| --- | --- |
| `lcom-ng-linux-x86_64.tar.gz` | Ubuntu 24.04, x86_64 |
| `lcom-ng-linux-arm64.tar.gz` | Ubuntu 24.04, ARM64 |
| `lcom-ng-macos-x86_64.tar.gz` | Intel macOS runner |
| `lcom-ng-macos-arm64.tar.gz` | Apple Silicon macOS runner |
| `lcom-ng-windows-msys2-x86_64.tar.gz` | Windows, MSYS2 MSYS ABI |

Linux bundles non-system shared libraries and uses the host glibc and loader.
macOS relocates dylibs, applies ad-hoc signatures, and records its minimum OS
version in `bundle.json`; archives are not notarized. Windows includes runtime
DLLs but uses MSYS2 POSIX process support, so it is not a standalone native
Windows compiler target. Windows ARM is not supported.

## Make a candidate

1. Finish the changes and run `devenv shell -- lcom-ng-verify`.
2. Keep `CMakeLists.txt` and `package.json` versions equal. CMake enforces this.
3. Commit and push the intended release revision.
4. Push an annotated tag named `v` plus that version, such as `v0.3.1` when both
   files say `0.2.0`.

```sh
git tag -a v0.3.1 -m "lcom-ng 0.3.1"
git push origin v0.3.1
```

A release build starts from a `v*` tag and rejects a tag that does not match the
package version. All platform jobs must pass before it creates a **draft GitHub
release** with archives, individual `.sha256` files and `SHA256SUMS`. Review the
draft and publish it.

## Build without publishing

Open **Actions → Build student releases → Run workflow**. Leave publishing
disabled for a candidate build, then download its archives from the successful
run.

Manual publication requires an existing tag that matches the package version; it
also creates a draft after all checks pass. Pull requests and main-branch pushes
build and test without creating a release.

Published releases are immutable. Rerunning a published tag fails, and a draft is
updated only while it stays a draft. Use a new version tag for follow-up changes.

## What the gates prove

Each job builds with SDL enabled, runs the runtime and lab suite, packages the
student payload, extracts it elsewhere, and tests the installed CLI. The archive
smoke builds and runs original Minix-style lab code, builds a native starter,
checks fresh TODO failures, and starts SDL with dummy/software drivers.

A local arm64 macOS check additionally exercised real keyboard input and games.
It is not a CI gate. A dummy-display smoke does not prove physical input
responsiveness or visual quality. Linux, Intel macOS and Windows need their own
successful Actions runs before shipping.

## Packaging owners

| File | Purpose |
| --- | --- |
| `.github/workflows/release.yml` | Platform builds and draft release publication |
| `scripts/package_student.py` | Shared SDK, docs and declared example contents |
| `scripts/package_desktop.py` | Linux / Windows dependencies and archive |
| `scripts/build_release_sdl.sh` | Pinned SDL3 dependency build on Linux |
| `tests/student_bundle_smoke.py` | Relocated Linux / Windows SDK and SDL check |
| `scripts/package_macos.py` | Relocatable macOS student archive |
| `tests/macos_bundle_smoke.py` | Installed macOS archive verification |
| `.github/workflows/docs.yml` | Build and deploy the verified website |

Packaging includes only examples declared by CMake's `student-examples.txt`. It
must not collect stale executables from a developer's build folder.
