# Adopt lcom-ng

For instructors, teaching assistants, club organizers, and curriculum leads.
lcom-ng is a practical course kit: students learn C through device-shaped
interfaces, then combine them in a reactive project. It keeps the VirtualBox and
Minix setup off the critical path, so the material works in classrooms,
workshops, and independent study.

For credits, prerequisites, and the weekly plan, use the
[syllabus map](/instructors/syllabus).

## Adoption modes

| Mode | Duration | Recommended scope |
| --- | --- | --- |
| Workshop | 2 to 6 hours | Lab 1 plus one visual example |
| Course module | 2 to 4 weeks | Labs 1, 2, 3, 5 and a small project |
| Semester course | 10 to 14 weeks | Labs 1 to 7 and final project studio |
| Replacement lab track | existing systems course | Select labs that match your lecture order |

For a semester course, use labs in the first half and the project in the second.
For a short module, do not assign every lab. A four-week version can cover
RTC/bitwise work, timers, keyboard input, framebuffer graphics, and a small
scripted demo.

## LCOM-style project model

The final project uses the device areas studied during the term: timer, keyboard,
mouse, graphics, RTC, audio, and optionally a serial link. Games force timing,
input, rendering, and state management into one loop. Editors, drawing tools,
calendars, sequencers, simulations, and protocol demos also work.

The constraint is explainability, not size. Students describe what each device
does, how events enter and change state, how rendering happens, and how staff can
rebuild and demonstrate the result.

### Teams and individual assessment

Teams of four are a practical default: enough members for device ownership,
state, rendering, and documentation, and small enough that everyone understands
the whole program. The artifact is shared; the grade is individual. Each student
runs the project, explains part of the architecture, justifies device choices,
and discusses their own code.

### Proposal

Ask for a short proposal before the project studio starts:

- What are we building?
- Which devices will it use?
- What is the smallest finished loop that counts as working?
- What parts are risky?
- What will we cut first if scope grows?

Use the proposal to challenge projects that are vague, UI-heavy without device
integration, asset-dependent, or too ambitious for the available time.

### Final delivery

Require every artifact to be committed or uploaded before the cutoff. A simple
rule is that the assessed version is the last commit before the deadline. Use the
[project rubric](/instructors/rubric#suggested-milestones) for the delivery
contract, report length, and demo requirements, and publish those expectations
before project work begins.

If the class publishes a [game collection](/instructors/game-collections), ask
each team to add `description`, `preview`, and `instructions` to its `lcom.json`
so launcher text and keyboard help are accurate.

### Project entry point

Students should leave one obvious run path. A native lcom-ng workspace uses an
ordinary `main` and its `Makefile`; `make run` launches
`build/student_project`. A compatibility project keeps its original LCF entry
point:

```c
int proj_main_loop(int argc, char *argv[]) {
  /* application setup, event loop, cleanup */
  return 0;
}
```

Staff should not need to reverse-engineer how to start the program.

## Assessment and policy

Publish the lab schedule, late and collaboration policies, AI policy, project
milestones, artifact expectations, and rubric before project work begins.
Students should know the proposal and final deadlines, what counts as the
assessed commit, and where the video and report go.

Use the [project rubric](/instructors/rubric) for weights, AI wording, and
milestones, and the [syllabus map](/instructors/syllabus) for course shape. Adapt
numbers and dates locally.

## One-week adoption plan

### Day 1: pick the course shape

Decide whether lcom-ng is a workshop, a module, a semester lab track, or a
replacement for an existing practical sequence. Publish that scope early so
students know whether labs are practice, graded assignments, or project
building blocks.

Also decide what students submit: lab source, test output, traces, screenshots,
WAV files, replay scripts, or project bundles. Prefer reproducible artifacts.

### Day 2: verify the toolchain

Install a release archive from the [install guide](/students/install). On an
instructor machine, run:

```sh
lcom-ng --help
lcom-ng setup student-smoke --force
make -C student-smoke
lcom-ng test rtc --project student-smoke
```

The last command should fail for a fresh workspace because the TODO stubs are
incomplete. That is expected: students see the same initial state.

> [!IMPORTANT]
> Do not distribute the upstream repository as the normal student workspace.
> Generate workspaces with `lcom-ng setup` so students see the course surface
> rather than runtime internals.

### Day 3: pilot one lab

Have one staff member follow Lab 1 as a student, without reference solutions.
Read the guide, run the failing test, implement the bit helpers, rerun the test,
and record confusing steps. Patch the handout or setup instructions before
students see it. Most adoption failures are small mismatches: a stale command, an
unclear generated folder, or a missing compiler.

### Day 4: publish assessment and policy

Publish the assessment split, lab schedule, policies, milestones, artifacts, and
rubric described in [Assessment and policy](#assessment-and-policy). If adapting
an LCOM-style course, keep the requirement structure and rewrite dates,
submission channels, and the device list locally.

### Day 5: run the first class

Show a short demo, generate a workspace live, run `make`, run a failing lab
test, and implement one tiny bit helper together. Stop before solving the whole
lab.

## Mapping to a traditional computer laboratory course

lcom-ng covers the same territory as a peripheral-programming laboratory course
without a microkernel VM. Minix, privileged services, and I/O permissions remain
useful context for how operating systems mediate hardware access.

| Traditional objective | lcom-ng equivalent |
| --- | --- |
| Use hardware interfaces of common peripherals | Use RTC, PIT, i8042, VBE, audio, and UART-shaped APIs |
| Develop low-level C software | Implement small libraries with explicit error handling |
| Practice polling and interrupts | Use deterministic event loops and runtime IRQ-style delivery |
| Build static libraries for later reuse | Move lab helpers into reusable modules for the project |
| Debug systematically | Use tests, traces, frame dumps, WAVs, and replay scripts |
| Build a final interactive project | Combine timers, input, graphics, audio, and serial protocols |

## One-week checklist

- `lcom-ng` runs on every supported student platform.
- A generated workspace compiles with `make`.
- Staff can explain generated workspace versus upstream repository.
- First lab handout is published.
- Staff know which files are reference solutions.
- Help channel and issue-report format are published.
- A small demo is ready: word processor, music maker, or game replay.
- Project rubric is visible before students combine devices.
- Any class Arcade has permission to publish its games and screenshots.

## Reuse and attribution

Code is MIT licensed. Course docs, lab handouts, screenshots, and demo media are
CC BY 4.0. Universities may copy and adapt the material with credit to lcom-ng
and contributors.
