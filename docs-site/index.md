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

<div class="quickstart-grid">
<section>

### Your existing Minix project

Keep the original source tree. In the folder containing the Makefile:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Use the binary path printed by the builder, named after `PROG` in your Makefile.

[Run your project or lab →](/students/compatibility)

</section>
<section>

### A new native project

```sh
lcom-ng setup student
cd student && make
lcom-ng run -- build/student_project
```

Write in `proj/`, implement `labs/`, and run `lcom-ng test rtc`; fresh TODO labs
fail until implemented.

[Start the native lab track →](/students/)

</section>
</div>

## Built for the programs you make in class

<div class="demo-grid">
  <figure>
    <img src="/assets/screenshots/music-maker.png" alt="A three-track music sequencer running in lcom-ng" loading="lazy">
  </figure>
  <figure>
    <img src="/assets/screenshots/breakout.png" alt="Breakout with a paddle, bricks and score running in lcom-ng" loading="lazy">
  </figure>
</div>

<div class="demo-video">
  <video controls preload="none" poster="/assets/screenshots/word-processor.png" aria-label="lcom-ng demonstration" src="/assets/demo/machine-lab-demo.mp4"></video>
</div>

## Learn the devices, then build a project

Seven native labs cover the computer architecture and devices behind RTC,
timers, keyboard, mouse, graphics, audio and UART.

| What you need | Where to start |
| --- | --- |
| A working installation | [Download and install](/students/install) |
| An existing game or original lab | [Compatibility quick start](/students/compatibility) |
| Native exercises and checks | [Lab track](/labs/) |
| Ideas for a final project | [Examples](/examples/) |
| Material for a class | [Instructor adoption guide](/instructors/adoption) |
| Runtime and release development | [Developer guide](/developers/) |
