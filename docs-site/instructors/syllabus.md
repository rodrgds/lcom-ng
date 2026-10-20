# Syllabus map

A starting point, not a required contract: device labs first, project studio
second.

## Course summary

Students write structured C against machine-like device interfaces: bit-level
protocols, polling, interrupt-style events, mapped buffers, byte streams,
systematic debugging, and state-machine design. The outcome is a final
interactive program that combines several device areas.

A full course can be 6 ECTS with about 52 contact hours and 162 total hours. A
typical week has two hours of theory or discussion plus two hours of laboratory
work; the rest is independent study and project work.

## Prerequisites

Introductory Programming, Computer Architecture, and Operating Systems. Students
should compile C programs, read pointer-based APIs, understand basic memory
layout, and reason conceptually about registers, interrupts, and system calls.

If C is weak, spend the first session on bit operations, pointers, structs,
`make`, and compiler errors. If systems background is strong, move faster
through Lab 1 and spend more time on project architecture and deterministic
replay.

## Learning outcomes

- Use device-shaped programmatic interfaces.
- Structure low-level C libraries.
- Debug with reproducible experiments.
- Combine asynchronous input sources into a reactive application with a clear
  state model.
- Use compiler diagnostics, Makefiles, static libraries, Git, generated tests,
  and frame or audio artifacts.

## Weekly map

| Week | Student work | Instructor focus | Evidence |
| --- | --- | --- | --- |
| 1 | setup, bit helpers, RTC date/time | workspace smoke test, C bit review | failing then passing `rtc` checks |
| 2 | PIT divisors and timer interrupts | event loops and timing policy | `timer` tests and tick logs |
| 3 | keyboard status and scancodes | polling vs interrupts, partial data | scancode output or replay |
| 4 | mouse packets | synchronization and signed deltas | packet output |
| 5 | graphics mode and rectangles | memory mapping, pitch, clipping | framebuffer dump |
| 6 | sprites and project sketch | rendering helpers, project scope | visual prototype |
| 7 | PCM audio | buffers, sample rate, generated sound | WAV artifact |
| 8 | UART | FIFOs, loopback, pair runtime | serial trace or two-peer demo |
| 9 | project proposal | scope control and risk review | idea, devices, smallest loop, risks |
| 10 | first usable loop | state machine and deterministic replay | replay script |
| 11 | systems pass | helper boundaries, error handling, timing | code review |
| 12 | polish and packaging | artifacts, demo script, rubric check, optional Arcade metadata | source, docs, report draft, video draft, `lcom.json` |
| 13 | final demo | presentation and review | last assessed commit plus artifacts |

Shorter modules can skip Labs 6 and 7, or use Lab 5 plus a small project as a
graphics unit. Longer offerings can add lectures on MMIO, USB HID, graphics
APIs, audio scheduling, or embedded Linux.

For a class Arcade, collect each project's `description`, `preview`, and
`instructions` in its `lcom.json` during the packaging week. See
[game collections](/instructors/game-collections).

## Topic coverage

| Topic | Where it appears |
| --- | --- |
| I/O peripherals and operation modes | Labs 1-7 |
| Polling and interrupt-style handling | Labs 2-4, project loop |
| IA-32/PC historical device interfaces | Labs 1-5, UART |
| Direct memory mapping ideas | Lab 5 framebuffer, Lab 6 audio buffer |
| C structure and parameter passing | every lab API |
| Event-driven programming and state machines | Labs 2-5, project |
| Static libraries and reusable helpers | Labs 2 onward |
| Systematic debugging | tests, traces, dumps, WAVs, replays |
| Tooling | `cc`, `make`, Git, generated tests, docs |

## Recommended bibliography

Elecia White, *Making Embedded Systems*, for embedded software as design work.
Derek Molloy, *Exploring Raspberry Pi*, for a bridge to Linux hardware
interfacing. Selected chapters from *Operating Systems: Three Easy Pieces* for
processes, virtualization, and I/O. The lab pages link to focused references for
CMOS/RTC, PIT timers, PS/2 input, VBE graphics, PCM audio, and UART programming.

## Assessment

Suggested split: 40% theory, 60% project. Theory checks device access, C
behavior, event loops, and debugging. The project rewards integration, clear
architecture, reproducible artifacts, and meaningful use of several device
areas.

Expected delivery is application source, build and run instructions,
documentation, a short architecture report, and a short demo video. Teams share
the artifact; each student is assessed on their own progress and understanding.

See the [project rubric](/instructors/rubric) for weights, milestones, and AI
policy.
