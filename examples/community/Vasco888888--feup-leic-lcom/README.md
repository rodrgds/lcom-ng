# Dr. Minix - Tiles LAB

Dr. Minix - Tiles LAB is a high-performance, rhythm-based graphical game developed for the MINIX operating system. This project was created as part of the LCOM (Laboratório de Computadores) course to explore low-level system programming, hardware device interaction, and advanced graphics rendering techniques.

## Features

- **Rhythm Gameplay**: Reactive tile-based gameplay with both short and long notes.
- **Dynamic Difficulty**: Adjustable spawn intervals and speed settings.
- **Customizable Graphics**: Support for light and dark modes, and multiple screen resolutions (600p/1024p).
- **Persistent Highscores**: A record-keeping system to track your best performances.
- **Hardware Integration**: Direct interfacing with the i8254 Timer, i8042 Keyboard Controller (KBC), and VESA Bios Extensions (VBE) for graphics.

## Project Deliverables

- [**Demonstration Video**](media/video_lcom_proj_grupo_2leic02_3.mkv)
- [**Final Report**](docs/relatorio_lcom_proj_grupo_2leic02_3.pdf)

## Project Structure

The repository is organized into the following directory structure:

- `src/`: Core source code.
    - `game/`: Tile logic, collision detection, and gameplay state management.
    - `video/`: VBE graphics drivers and frame buffer management.
    - `keyboard/`: KBC handlers and scancode processing.
    - `menu/`: Main menu UI and configuration options.
    - `timer/`: i8254 timer configuration and interrupt handling.
    - `highscore/`: Highscore recording and display system.
    - `utils/`: Low-level utility functions for I/O and bit manipulation.
- `docs/`: Project documentation and reports.
- `media/`: Video demonstrations and visual assets.

## Build and Run Instructions

### Prerequisites

- MINIX 3 Operating System.
- LCOM Library (liblcf).

### Compiling

To compile the project, navigate to the `src/` directory and use the provided Makefile:

```bash
cd src
make
```

### Execution

After successful compilation, the game can be started using:

```bash
lcom_run proj
```

## Contributors

- **Mário Pereira** (up202304965@up.pt)
- **Vasco Sá** (up202306731@up.pt)
- **João Júnior** (up202306719@up.pt)
- **Ana Santos** (up201905129@up.pt)

## Acknowledgments

- Developed for the **Laboratório de Computadores (LCOM)** course.
- **Faculty of Engineering of the University of Porto (FEUP)**.
- Special thanks to the LCOM teaching team for the support and the hardware/software framework (liblcf).
