# Instructor guide

lcom-ng works as a course module, workshop, independent study track, or semester
lab section. Students get a generated workspace. Instructors get lab guides,
reference tests, examples, rubrics, and a portable runtime with no VM setup.

Students implement small C libraries for device-shaped interfaces, then reuse
them in a final interactive project. See the [syllabus map](/instructors/syllabus)
for topics, outcomes, and a week-by-week plan.

## Course shape

| Phase | Material | Outcome |
| --- | --- | --- |
| Week 1 | Lab 1 | bit reasoning and RTC reads |
| Week 2 | Lab 2 | PIT divisors and timer event loops |
| Weeks 3-4 | Labs 3-4 | keyboard and mouse packet handling |
| Weeks 5-6 | Lab 5 | framebuffer graphics and sprites |
| Weeks 7-8 | Labs 6-7 | audio buffers and UART protocols |
| Weeks 9-13 | Project studio | interactive C application, optionally published in a class Arcade |

## Teaching materials

| Resource | Use |
| --- | --- |
| [Lab track](/labs/) | student-facing lab sequence |
| [Syllabus map](/instructors/syllabus) | timing, outcomes, weekly plan |
| [Adoption guide](/instructors/adoption) | first-week setup and course decisions |
| [Project rubric](/instructors/rubric) | assessment weights and milestones |
| [Game collections](/instructors/game-collections) | class Arcade, metadata, and browser/Android export |
| [Install guide](/students/install) | release archives for student machines |

## What to publish

- Release archive or install page.
- Generated workspace instructions.
- Lab schedule, collaboration policy, and grading rubric.
- Project milestones and expected artifacts.

Artifacts can include test output, traces, screenshots, WAV captures, replay
scripts, videos, or bundles.

## What to keep private

| Path | Reason |
| --- | --- |
| `dev/lab-solutions/` | reference implementations |
| `dev/lab-tests/` | can reveal expected behavior |
| upstream CMake/runtime internals | not part of normal student work |

Students start from `lcom-ng setup`, not the upstream repository. The upstream
repository is for maintainers and contributors.

## First class flow

Show the short demo, generate a workspace live, run `make`, run a failing lab
test, and implement one tiny bit helper together. Stop before solving the whole
lab. The point is to show that tests are feedback for missing student code, not
broken infrastructure.

Next: [adopt lcom-ng in one week](/instructors/adoption).
