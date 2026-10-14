---
layout: home
hero:
  name: lcom-ng
  text: Run your LCOM project. Skip the VM.
  tagline: Build the same C sources on your computer, or start fresh with the native SDK. Labs, games, keyboard, mouse and audio.
  image:
    src: /assets/screenshots/word-processor.png
    alt: Word processor running in lcom-ng
  actions:
    - theme: brand
      text: Download and install
      link: /students/install
    - theme: alt
      text: Run an existing project
      link: /students/compatibility
    - theme: alt
      text: Start native labs
      link: /students/
---

## From source to a running window

Install a release archive, then choose your starting point. lcom-ng brings the
course's ports, interrupts, framebuffers and serial links to your own computer.

<div class="quickstart-grid">
<section>

### Your existing Minix project

Keep the original source tree and imports. In the folder containing the Makefile:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Use the binary path printed by the builder, named after `PROG` in your Makefile.
The same workflow runs original labs with their usual test arguments.

[Run your project or lab →](/students/compatibility)

</section>
<section>

### A new native project

Generate the native course workspace and build it:

```sh
lcom-ng setup student
cd student && make
lcom-ng run -- build/student_project
```

Write your app in `proj/`, implement the labs in `labs/`, and run `lcom-ng test rtc`.
Fresh TODO labs fail checks until you implement them.

[Start the native lab track →](/students/)

</section>
</div>

## Built for the programs you make in class

<div class="demo-grid">
  <figure>
    <img src="/assets/screenshots/music-maker.png" alt="A three-track music sequencer running in lcom-ng" loading="lazy">
    <figcaption>Timers, keyboard input and PCM audio.</figcaption>
  </figure>
  <figure>
    <img src="/assets/screenshots/breakout.png" alt="Breakout with a paddle, bricks and score running in lcom-ng" loading="lazy">
    <figcaption>Graphics, interrupts and an interactive game loop.</figcaption>
  </figure>
</div>

<div class="demo-video">
  <video controls preload="none" poster="/assets/screenshots/word-processor.png" aria-label="lcom-ng demonstration" src="/assets/demo/machine-lab-demo.mp4"></video>
</div>

## Learn the devices, then build a project

The seven native labs cover RTC, timers, keyboard, mouse, graphics, audio and UART.
Existing Minix labs use the compatibility builder. Pinned student repositories
provide practical compatibility evidence, including documented source defects and limits.

| What you need | Where to start |
| --- | --- |
| A working installation | [Download and install](/students/install) |
| An existing game or original lab | [Compatibility quick start](/students/compatibility) |
| Native exercises and checks | [Lab track](/labs/) |
| Ideas for a final project | [Examples](/examples/) |
| Material for a class | [Instructor adoption guide](/instructors/adoption) |
| Runtime and release development | [Developer guide](/developers/) |

lcom-ng is courseware for computer architecture and systems programming.
Its native SDK is the path for new code; the separate compatibility SDK preserves
Minix/LCF conventions for existing projects.
