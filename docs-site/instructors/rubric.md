# Project rubric

A strong project is a coherent reactive program that combines device areas, has a
clear state model, and can be demonstrated reproducibly.

## Suggested weights

For a full semester, use 40% theory and 60% project. Within the project
component, either grade broad outcomes or use a device-weighted rubric.

| Area | Weight | Looks for |
| --- | --- | --- |
| Prototype milestone | 15% | minimum finished loop, realistic scope, deterministic replay |
| Final implementation | 35% | device integration, event loop, state management, polish |
| Implementation note | 10% | clear explanation of devices, timing, data flow, tradeoffs |
| Demo artifact | 5% | screenshot, WAV, video, bundle, or trace that can be reproduced |
| Individual contribution adjustment | variable | fair accounting within group work |

These are relative shares of the project component. Grade lab checks or practical
progress separately if the course uses them.

An LCOM-style rubric can be concrete about devices:

| Area | Weight | Looks for |
| --- | ---: | --- |
| Practical-class individual progress | 10% | each student can run, explain, and advance the work |
| RTC | 5% | purposeful use such as timestamps, calendars, logs, saves, or time-aware behavior |
| Timer | 5% | stable timing policy, animation ticks, debouncing, scheduling, or elapsed-time logic |
| Keyboard | 10% | correct event handling, key state, shortcuts, text input, or command control |
| Mouse | 10% | packet/state handling, coordinates, buttons, dragging, pointing, or gesture logic |
| Graphics card / framebuffer | 20% | correct drawing, clipping, buffering, animation, and visual polish |
| Code structure | 10% | modular C, clear helpers, documented architecture, build hygiene |
| Demonstration and discussion | 20% | convincing live demo and individual technical understanding |
| Report and video | 10% | concise architecture report and reproducible demo artifact |
| Optional serial-port extension | up to 10% extra | real paired feature, protocol, multiplayer, console, or visualizer |

Graphics carries the largest weight because it dominates the visible program.
Keyboard and mouse test different input models. Timer credit should reward real
timing policy, not sleeping. RTC credit should reward purposeful use. Fewer
devices used well beats touching every API superficially. Serial extra credit
should enable a real paired feature.

If labs are graded directly, reserve a separate lab component. If formative,
require reuse of lab libraries in the project.

## Strong final projects

| Criterion | Strong submission |
| --- | --- |
| Device use | combines at least three device areas meaningfully |
| Event loop | handles time, input, rendering, and audio without blocking surprises |
| Architecture | separates device helpers, application state, and presentation |
| Reliability | has deterministic replay or scripted smoke path |
| Finish | has a coherent start, interaction loop, and end state |
| Evidence | includes source, build instructions, and reproducible artifact |

## Suggested milestones

The proposal states the idea, devices, smallest finished loop, and risks. The
prototype runs and responds to input. The final submission has source, build
instructions, a short implementation note, and one reproducible artifact.

For a larger course, require source, documentation, build and run instructions, a
report of at most five pages, and a demo video of at most five minutes. The report
covers structure, devices, and what separates the project from a minimal
exercise. The video proves the submitted version runs; it does not replace source
or report.

Assess the shared artifact and each member's contribution; marks may differ.

If the class publishes a [game collection](/instructors/game-collections), an
Arcade entry may count as the demo artifact if the game's `lcom.json` title,
description, preview, and instruction rows match the build. Keyboard rows are
display help and must name keys the game reads.

## AI use

State whether AI is allowed and make students responsible for every submitted
file. They must explain code, verify output, protect confidential data, avoid
submitting generated solutions as unaided work, and describe tool use if asked.
Test this in discussion: explain a buffering decision, walk through a parser, or
justify timing. Grade understanding and artifact, not the tool.

## What to penalize

Blocking sleeps, unreviewable device code, builds staff cannot reproduce, no
reproducible demo path, and features without a stable core loop.
