# Pacocalypse

Pacocalypse is a Pac-Man-inspired arcade game developed in C for the MINIX operating system as part of the LCOM course at the University of Porto.

The project implements a complete game loop with keyboard and mouse input, timer-driven updates, VGA graphics, collectible power-ups, score tracking, lives, and ghost AI with chase, scatter, frightened, and respawning states.

## Features

- Tile-based map with collision detection and portals
- Player movement, animation, score, lives, and power-ups
- Four ghosts with independent identities and movement states
- Main menu, pause menu, and run summary information
- Timer, keyboard, mouse, RTC, and video-card drivers
- Model-controller-view organization

## Controls

| Input | Action |
| --- | --- |
| `W` | Move up |
| `A` | Move left |
| `S` | Move down |
| `D` | Move right |
| `ESC` | Pause or resume the game |
| Left mouse button | Select menu actions or activate a power-up |

## Project structure

```text
.
├── lab1/                 # RTC and bitwise-operation exercises
├── lab2/                 # Timer driver
├── lab3/                 # Keyboard driver
├── lab4/                 # Mouse driver
├── lab5/                 # Video-card driver
└── Pacocalypse/
   ├── model/            # Game, map, player, and ghost state
   ├── controller/       # Input handling and game control
   ├── view/             # Renderer, UI, fonts, sprites, and resources
   ├── config.h          # Display and timing configuration
   ├── main.c            # Hardware setup and main loop
   └── Makefile          # MINIX/LCOM build configuration
```

## Requirements

This project targets the MINIX/LCOM environment and depends on the LCOM libraries and course driver libraries. It is not currently intended to build as a native Windows, macOS, or Linux application.

You will need:

- A configured MINIX/LCOM development environment
- The `minix.lcom.mk` build configuration
- The LCOM libraries used by `lab1` through `lab5`
- VGA-compatible graphics and keyboard/mouse input support

## Build

From the project directory inside the MINIX/LCOM environment:

```sh
cd Pacocalypse
make
```

The Makefile builds the `proj` MINIX service and links it against the driver libraries from the previous labs. Run the resulting program using the execution procedure provided by your LCOM environment.

## Configuration

The main display and timing settings are defined in `Pacocalypse/config.h`:

- Video mode: `0x14C`
- Resolution: `1152 x 864`
- Timer frequency: `60 Hz`

## Documentation

Generated API documentation is available in [`Pacocalypse/docs/html`](Pacocalypse/docs/html). To regenerate it, use the included [`Pacocalypse/Doxyfile`](Pacocalypse/Doxyfile) with Doxygen in the MINIX/LCOM environment.

## Authors

Developed for LCOM by Group `2LEIC03_3`:

- Filipe Cruz
- Pedro Vilaça
- Pedro Postiga

## Responsible AI use

We are responsible for all code and documentation in this repository. AI-based tools were used only within the limits of the course and institutional policies, as support for learning and development. All generated suggestions were reviewed, tested, and adapted by the authors, who remain responsible for the final implementation.

## License

This repository was created as coursework for the LCOM course at the University of Porto. No open-source license has currently been selected.

