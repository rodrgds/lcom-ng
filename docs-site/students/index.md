# Native labs and projects

[Install lcom-ng](/students/install), then generate a native workspace. The original
LCOM repository is not required. For Minix labs or existing games, use the
[compatibility quick start](/students/compatibility).

## Start here

```sh
lcom-ng setup student
cd student
make
lcom-ng run -- build/student_project
lcom-ng test rtc
```

Fresh stubs compile but fail checks until you implement the functions; a failing
first test is expected.

## What you edit

| Path | Use |
| --- | --- |
| `labs/<device>/*.c` | required lab implementations |
| `labs/<device>/include/` | function contracts for that lab |
| `lib/<device>/` | reusable helpers you build over time |
| `proj/` | final-project or experiment code |
| `Makefile` | build wiring when your instructor asks for it |

## What you usually ignore

| Path | Why |
| --- | --- |
| `include/lcom/` | copied SDK headers; treat as read-only |
| `.mlab/` | generated tests and build artifacts |
| `build/` | generated object files and binaries |

## Lab loop

1. Read the [lab overview](/labs/).
2. Run the failing test once.
3. Find the requested function signatures.
4. Implement the smallest correct version.
5. Run the test again.
6. Move repeated logic into `lib/<device>/` only after the lab entry point works.

> [!TIP]
> Note each register's address, readable and writable bits, and status flags.

## Final project

Combine at least three device areas:

- timer + keyboard + framebuffer for an arcade loop;
- mouse + framebuffer for drawing or editing;
- audio + timer + keyboard for sequencers;
- UART + `run-pair` for two-player or protocol work.

Include source, a deterministic replay or scripted smoke path, and one artifact:
screenshot, WAV, video, trace or bundle.

Share it with the [browser exporter](/students/browser), which adds an on-screen
keyboard for mobile players. Add an optional
[`lcom.json`](/students/project-config) for title, icon and mobile controls.
