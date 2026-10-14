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
macOS bundles and relocates dylibs, then applies ad-hoc signatures. Its minimum
OS version is recorded in `bundle.json`; these archives are not notarized.
Windows includes runtime DLLs but uses POSIX process support from MSYS2. It is
not a standalone native Windows compiler target. Windows ARM is not supported.

## Make a candidate

1. Finish the changes and run `devenv shell -- lcom-ng-verify`.
2. Keep `CMakeLists.txt` and `package.json` versions equal.
3. Commit and push the intended release revision.
4. Push a matching version tag, such as `v0.2.0` when both files say `0.2.0`.

```sh
git tag -a v0.2.0 -m "lcom-ng 0.2.0"
git push origin v0.2.0
```

Choose the next unused version for later releases. The workflow rejects a tag
that does not match the package version. All platform jobs must pass before it
creates a **draft GitHub release** with archives, individual `.sha256` files and
`SHA256SUMS`. Review the draft and publish it in GitHub Releases. Published assets
are not replaced by rerunning the workflow.

## Build without publishing

Open **Actions → Build student releases → Run workflow**. Leave publishing disabled
for a candidate build. Download its platform archives from the successful run.
Manual publication requires selecting a matching release tag; it also creates a
draft after all checks pass. Pull requests and main-branch pushes build and test
without creating a release.

## What the gates prove

Each job builds with SDL enabled, runs the runtime and lab suite, packages the
student payload, extracts it elsewhere, and tests the installed CLI. The archive
smoke builds and runs original Minix-style lab code, builds a native starter,
checks useful fresh TODO failures, and starts SDL with dummy/software drivers.

The local arm64 macOS trial additionally exercised real keyboard input and games.
A successful dummy-display smoke is a dependency and startup check, not evidence
of physical input responsiveness or visual quality on every platform. Linux,
Intel macOS and Windows need their own successful Actions runs before shipping.

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

Packaging includes only examples declared by CMake's `student-examples.txt`.
It must not collect stale executables left in a developer's build folder.
