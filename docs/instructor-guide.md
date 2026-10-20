# Instructor guide

Repository operations for instructors. Course design, syllabus, grading, and
first-week adoption live on the public website.

## Public guides

| Topic | Public page |
| --- | --- |
| Course shape and learning outcomes | [Instructor guide](https://lcom.rgo.pt/instructors/) |
| First-week adoption | [Adoption](https://lcom.rgo.pt/instructors/adoption) |
| Week-by-week plan | [Syllabus](https://lcom.rgo.pt/instructors/syllabus) |
| Project weights and criteria | [Project rubric](https://lcom.rgo.pt/instructors/rubric) |
| Class game collections | [Game collections](https://lcom.rgo.pt/instructors/game-collections) |
| Lab handouts | [Lab Track](https://lcom.rgo.pt/labs/) |

The browser Arcade is the public showcase for student games. Android app and
collection builds are paused.

## What to give students

- the release archive for their platform, or the public
  [install instructions](https://lcom.rgo.pt/students/install);
- a generated workspace from `lcom-ng setup`;
- selected handouts from [course/labs/docs](../course/labs/docs);
- your project brief, collaboration policy, and grading rubric.

Use the upstream repository as a student workspace only when students are also
contributing to lcom-ng.

## Instructor-owned surfaces

| Path | Use | Visibility |
| --- | --- | --- |
| `course/README.md` | course flow and project studio notes | publish |
| `course/labs/docs/` | student-facing handouts | publish |
| `course/labs/function-requests.json` | structured lab index used by CLI/help | maintainer |
| `examples/` | demos and project seeds | publish |
| `scripts/` | deterministic replays for demos and tests | publish |
| `docs/video-demo.md` | short presentation script | publish |
| `docs/adoption.md` | package-local adoption notes | publish |
| `dev/lab-solutions/` | reference implementations | keep private |
| `dev/lab-tests/` | predefined checks that can reveal expected behavior | selected |
| `tests/integration.sh` | upstream regression gate | keep private |

Publish selected tests if your course policy allows. Keep reference
implementations private until the course ends.

## Adoption checklist

From a repository checkout:

- `devenv shell -- lcom-ng-test` on one instructor machine.
- `devenv shell -- lcom-ng setup build/student-smoke --force`.
- `devenv shell -- make -C build/student-smoke`.
- `devenv shell -- lcom-ng test rtc --project build/student-smoke`.

Then prepare the course:

- confirm one TODO lab fails as expected in the generated workspace;
- pick the labs you will assign and hide the rest if the course is shorter;
- publish a project rubric before students combine devices;
- prepare one demo: word processor, music maker, or game replay;
- decide how students will submit artifacts.

## Assessment artifacts

Prefer artifacts students can reproduce:

- predefined lab test output;
- short replay scripts;
- screenshots or WAV captures;
- final-project source;
- short implementation notes on device use and architecture.

## Running a demo

```sh
lcom-ng run --headless --script scripts/write_note.mlabscript \
  --dump-frame build/write.ppm -- build/examples/word_processor

lcom-ng run --headless --script scripts/music_maker_demo.mlabscript \
  --audio-wav build/music.wav --dump-frame build/music.ppm -- build/examples/music_maker
```

## Reuse policy

Course materials are CC BY 4.0: adapt, translate, reschedule, and reuse
screenshots or demo media with attribution. Code is MIT licensed. See
[course/LICENSE.md](../course/LICENSE.md) and [LICENSE](../LICENSE).
