# Sushi Slash

Sushi Slash is a singleplayer arcade game inspired by Fruit Ninja, written in C for the MINIX environment used in **Laboratório de Computadores (LCOM)** at FEUP. Slice flying sushi with the mouse, avoid bombs, and defeat Boss Sushi by typing the displayed keys.

This repository contains both the final game in [`proj/`](proj/) and the laboratory exercises that provide its hardware drivers.

## Gameplay

- Start with **three lives**. Missing a regular sushi piece costs one life; slicing a bomb ends the run immediately.
- Sliced sushi splits into two fragments with independent motion.
- Chain at least **three cuts**, with no more than **25 timer ticks** between consecutive cuts, to earn a combo bonus equal to the chain's base points when the combo window closes.
- Waves become faster and more demanding as your score increases. Difficulty changes at **15, 30, 45, and 100 points**.
- Boss encounters begin at **50 points**, then at each additional 50-point milestone. Complete the keyboard sequence before the overall or per-key timer expires. A wrong key resets sequence progress; timing out costs one life. Completing the sequence awards two points per key.
- The HUD shows score, lives, and elapsed play time. The main menu also displays the RTC date/time and FPS.
- The game-over screen shows the final score, maximum combo, sushi cut and missed, bombs avoided, time survived, and the run's seed. Choose **restart same seed** to try again.

### Controls

| Context | Input | Action |
| --- | --- | --- |
| Menus | Left click | Select a button |
| Gameplay | Hold the left mouse button and move quickly across sushi | Slash |
| Gameplay | Esc | Pause |
| Boss encounter | Type the displayed letter | Advance the sequence |
| Seed entry | Number keys | Enter a decimal seed |
| Seed entry | Backspace | Delete the last digit |
| Seed entry | Enter or Esc | Finish editing |

Resume, return to the main menu, or exit using the pause menu's buttons. Gameplay time stops while paused.

### Seeds

Click **set seed** before starting to enter a value from `1` to `4294967295`. An empty seed or `0` generates a seed from RTC date/time and timer ticks. Sushi/wave generation and boss generation maintain separate pseudo-random states initialized from the run's seed.

Reusing a seed lets you retry the same starting random setup. Progression also depends on your inputs and score, so runs can diverge as you play differently.

## Requirements

Build and run the game inside the course-provided **MINIX/LCOM environment**, with:

- The C compiler and BSD-style `make` used by the course.
- LCOM's LCF headers/library (`lcom/lcf.h`, `liblcf`) and `minix.lcom.mk`.
- Keyboard, PS/2 mouse, timer, RTC, and VBE graphics support in the VM.
- VBE mode `0x115`: **800 × 600, 24-bit color**, with support for page flipping.

The game assumes **60 timer ticks per second**. It uses MINIX interrupt notifications, BIOS calls, and physical video-memory mapping, so a normal Linux/macOS build environment cannot run it directly.

Doxygen is optional and is only needed to generate API documentation.

## Build and run

Place the repository at `/home/lcom/labs` in the MINIX VM, preserving the sibling `lab1/` through `lab5/` and `proj/` directories. The entry point currently writes LCF logs to `/home/lcom/labs/proj/`; if you use another location, update the paths in [`proj/proj.c`](proj/proj.c).

Build the four driver libraries first, then the game:

```sh
cd /home/lcom/labs
make -C lab2 lib
make -C lab3 lib
make -C lab4 lib
make -C lab5 lib
make -C proj
```

These targets produce `libtimer.a`, `libkbd.a`, `libmouse.a`, and `libgraphics.a`, followed by the game executable `proj/proj`. RTC code from `lab1/rtc.c` is compiled directly into the game; building the Lab 1 executable is unnecessary.

Launch with the course environment's LCOM launcher:

```sh
cd /home/lcom/labs/proj
lcom_run proj
```

The launcher must be provided by your MINIX/LCOM setup; it is not included in this repository. The game's LCF entry point is `proj_main_loop`, and no gameplay arguments are required.

To remove generated binaries and libraries:

```sh
cd /home/lcom/labs
make -C proj clean
make -C lab5 clean
make -C lab4 clean
make -C lab3 clean
make -C lab2 clean
```

After changing a driver, rebuild its library and then rebuild the game. If the executable does not pick up a library change, clean and rebuild `proj/`.

## Repository layout

| Path | Purpose |
| --- | --- |
| [`proj/proj.c`](proj/proj.c) | LCF entry point, graphics initialization, and final cleanup |
| [`proj/src/`](proj/src/) | Game loop, gameplay, waves, physics, bosses, combos, sprites, fonts, and assets initialization |
| [`proj/src/ui/`](proj/src/ui/) | Menu and game-over rendering, HUD, boss UI, mouse trail, and session timer |
| [`proj/include/game/`](proj/include/game/) | Gameplay types and public APIs, state containers, queues, and random generation |
| [`proj/include/ui/`](proj/include/ui/) | UI and session timer interfaces |
| [`proj/include/macros/`](proj/include/macros/) | Screen, menu, fixed-point physics, and slash constants |
| [`proj/assets/`](proj/assets/) | XPM backgrounds, sushi, fonts, cursor, and animation frames |
| [`proj/docs/`](proj/docs/) | Doxygen configuration, overview, and documentation theme |
| [`lab1/`](lab1/) | Introductory bitwise exercises and RTC access |
| [`lab2/`](lab2/) | Timer driver and utility functions; `libtimer.a` |
| [`lab3/`](lab3/) | Keyboard/controller driver; `libkbd.a` |
| [`lab4/`](lab4/) | Mouse driver, packet processing, and slash detection; `libmouse.a` |
| [`lab5/`](lab5/) | VBE graphics, drawing, and page flipping; `libgraphics.a` |
| [`io_experiment/`](io_experiment/) | Standalone MINIX I/O experiment |

### Runtime architecture

[`game_loop.c`](proj/src/game_loop.c) subscribes to keyboard, mouse, and timer interrupts and dispatches MINIX hardware notifications according to the active state: **menu → game → paused or game over**, with menu actions allowing exit or a new run.

Timer events advance waves, fixed-point physics, animations, boss deadlines, combo scoring, and session time, then draw and flip the video pages. Mouse events update the cursor and test slash segments against sushi. Keyboard events handle seed input, pausing, and boss sequences. The RTC is read for the menu clock and automatic seed generation.

[`game.c`](proj/src/game.c) owns the gameplay lifecycle and active objects. [`waves.c`](proj/src/waves.c) schedules spawn requests; [`sushi.c`](proj/src/sushi.c) handles objects, fragments, and collision detection. Boss keyboard sequences use [`boss.c`](proj/src/boss.c), [`combo.c`](proj/src/combo.c), and [`queue.c`](proj/src/queue.c); mouse-cut combos use the separate [`slash_combo.c`](proj/src/slash_combo.c) module. Shared assets are loaded through [`init.c`](proj/src/init.c), while UI drawing is organized by screen.

The implemented mode is singleplayer. Multiplayer-related enum values are placeholders; serial communication and multiplayer gameplay are not implemented.

## API documentation

From the repository root, initialize the documentation theme submodule if needed:

```sh
git submodule update --init --recursive
cd proj/docs
doxygen Doxyfile
```

Open `proj/docs/html/index.html` in a browser. Run Doxygen from `proj/docs/` because the configuration uses relative paths. The generated reference covers public headers in `proj/include/` and the overview in [`mainpage.dox`](proj/docs/mainpage.dox).

## Development and verification

The Makefiles enable strict compiler warnings through LCOM's build rules and add `-pedantic`. Uncomment `CFLAGS += -DDEBUG_LOG` in [`proj/Makefile`](proj/Makefile) to enable the project's conditional debug logging and LCF call tracing (`trace.txt`). Call tracing is disabled by default to reduce interrupt-handling overhead. Captured output remains configured in `proj/proj.c` as `output.txt`; generated logs and build artifacts are ignored by Git. The menu FPS counter measures successful page flips against elapsed monotonic time, independently of handled timer notifications.

There is no automated test suite in the repository. After building in MINIX, useful manual checks are:

1. Start a run with an explicit seed and verify sushi slicing, fragment motion, and combo scoring.
2. Miss sushi until all lives are lost, then restart with the same seed.
3. Slice a bomb and verify immediate game over.
4. Reach a boss milestone and check correct keys, wrong-key resets, and timeout penalties.
5. Pause and resume, checking that session time remains frozen during the pause.
6. Return to the menu and exit, checking that text mode and input devices are restored.

## Authors

LCOM group **GRUPO_2LEIC09_05**:

- Diana Oliveira — up202404022@up.pt
- Diogo Castro — up202404297@up.pt
- Rodrigo Teixeira — up202404802@up.pt

## Declaration of Responsible AI Use

We declare that:

1. We are responsible for all code and documentation in this repository, and we understand that we must be able to explain and justify any part of it on request.  
2. We have used AI-based tools (e.g., code assistants, chatbots, or generators) only to support my learning, not to bypass the intended learning outcomes or any assessment rules.  
3. Wherever AI tools contributed to this work, we have:  
   - Used them within the limits set by the course policies and institutional regulations.  
   - Reviewed, tested, and, where necessary, edited the outputs, taking full responsibility for their correctness, originality, and legality.  
   - Ensured that no confidential, personal, or sensitive data were shared with AI tools.  
4. We have not used AI tools to generate complete solutions that we present as entirely our own unaided work, and we have avoided plagiarism, whether from AI outputs or other sources.  
5. If asked, we will provide details of which tools we used, for which files or parts of the project, and how we verified and adapted their outputs.

Signed: Diana Oliveira, Diogo Castro, Rodrigo Teixeira
Date: 2026-03-02
