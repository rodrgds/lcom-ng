# Student guide

The public student guide at <https://lcom.rgo.pt/students/> is canonical. This
page is a package-local reference for the same workspace.

## Start here

```sh
lcom-ng setup student
cd student
make
lcom-ng test rtc
```

Fresh lab stubs compile but fail checks until you implement the requested
functions. A failing first test is expected.

> [!NOTE]
> Your workspace is intentionally smaller than the upstream repository. It
> contains the public headers, starter lab files, tests, and a small project
> area. Runtime internals and release tooling are not part of the normal student
> workflow.

## Edit these

| Path | Use |
| --- | --- |
| `labs/<device>/*_lab.c` | required lab implementations |
| `labs/<device>/include/` | function contracts for that lab |
| `lib/<device>/` | optional reusable helpers you build over time |
| `proj/` | final-project or experiment code |
| `Makefile` | build wiring, only when your instructor asks for it |

## Usually ignore these

| Path | Why |
| --- | --- |
| `include/lcom/` | copied SDK headers; treat as read-only |
| `.mlab/` | generated tests and build artifacts |
| `build/` | generated object files and binaries |
| upstream `runtime/`, `common/`, `.github/` | lcom-ng developer internals |
| `dev/lab-solutions/` in the upstream repo | reference code, not starter work |

## Commands

```sh
make                                  # build the labs and project
lcom-ng test rtc                      # run one lab check
lcom-ng test lab1                     # same lab by number
lcom-ng run -- build/student_project  # open the project
make run                              # build and run the project
```

Lab names are `rtc`, `timer`, `kbd`, `mouse`, `graphics`, `audio`, and `uart`, or
`lab1` through `lab7`. Use `--project <dir>` when the workspace is not the
current directory.

Captured artifacts:

```sh
lcom-ng run --headless --dump-frame build/frame.ppm -- build/student_project
lcom-ng run --headless --audio-wav build/out.wav -- build/student_project
```

## Debugging

- Read the first failing check, not only the last line.
- Print intermediate register values in hexadecimal.
- Confirm you are not reading or writing outside a buffer.
- Add a tiny helper only when it makes the next call clearer.

## Public API boundary

Student code should include headers from `include/lcom/` and lab headers. It
should not include SDL, runtime C++ headers, lcom-ng internal protocol files, or
files from `dev/lab-solutions/`.

## Help requests

Include:

- the lab name;
- the command you ran;
- the first failing test output;
- the function you think is wrong;
- a short explanation of what you expected.

This lets another person help without taking over the implementation.
