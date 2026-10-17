# FEUP - Computer Laboratory - 2024/2025
> Curricular Unit: LCOM - [Laboratório de Computadores](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=541883)
## 2nd Year - 2nd Semester Project
### Brief description:
This project presents an advanced implementation of the classic Snake game developed for the MINIX operating system. Built using low-level programming techniques with direct hardware interaction, it offers a rich gaming experience with multiple game modes, power-ups, and sophisticated graphics rendering. The application demonstrates comprehensive integration with keyboard (i8042), timer (i8254), and graphics (VBE) controllers through a well-structured layered architecture.

Unlike traditional Snake implementations, this project features four distinct game modes including dynamic wall generation, inverted controls, and a power-up system with special fruits. The game includes a complete menu navigation system, efficient selective rendering that updates only changed screen portions, and robust collision detection. Technical highlights include XPM sprite graphics, real-time score display, and intelligent resource management optimized for the MINIX environment.

The project successfully demonstrates mastery of computer systems fundamentals, device driver development, and advanced game programming concepts while maintaining clean code architecture and optimal performance.

I hope you find it useful!

---

# Snake Game for MINIX 🐍

> An advanced implementation of the classic Snake game developed for the MINIX operating system, featuring multiple game modes, power-ups, and efficient hardware integration.

## 📋 Project Information

- **Course:** LCOM - Laboratório de Computadores
- **Institution:** Faculdade de Engenharia da Universidade do Porto (FEUP)
- **Academic Year:** 2024/2025
- **Class:** Turma 14, Grupo 5

## 👥 Team Members

| Name | Email | Student ID |
|------|-------|------------|
| Diogo Ferreira | up202205295@up.pt | up202205295 |
| Diogo Pinheiro | up202306461@up.pt | up202306461 |
| Diogo Silva | up202303713@up.pt | up202303713 |
| Luísa Monteiro | up202305199@up.pt | up202305199 |

## 🎮 Game Overview

This Snake implementation goes beyond traditional versions, offering a rich gaming experience with multiple modes and advanced features. Players control a snake on a 40x30 cell grid, consuming food to grow while avoiding collisions.

### 🎯 Game Modes

1. **Classic Mode** 🕹️
   - Traditional Snake gameplay
   - Basic collision detection with walls and self

2. **Walls Mode** 🧱
   - Dynamic wall spawning after each food consumption
   - Creates evolving obstacles (maximum 100 walls)
   - Increased strategic complexity

3. **Inverted Arrows Mode** ↕️
   - Complete control inversion (UP↔DOWN, LEFT↔RIGHT)
   - Challenges player coordination and muscle memory

4. **Fruit Power Mode** 🍓
   - Special power-up system with three fruit types:
     - **Normal Fruit** (70%): Standard growth (+10 points)
     - **Speed Fruit** (20%): Temporary speed boost for 500 ticks (+20 points)
     - **Size Fruit** (10%): Instant growth of 5 segments (+40 points)

## ✨ Key Features

### 🎨 Visual & Interface
- **Checkerboard Pattern Board**: Enhanced visibility and visual appeal
- **XPM Graphics**: Custom sprites for interface elements and buttons
- **Interactive Menus**: Complete navigation system with visual feedback
- **Real-time Score Display**: Numeric sprites for score visualization

### ⚡ Performance Optimizations
- **Selective Rendering**: Only redraws changed screen portions
- **Intelligent Update System**: Tracks previous positions for efficient clearing
- **CPU Optimization**: Significantly reduced processing overhead

### 🕹️ Gameplay Mechanics
- **Food Lifespan**: Limited food lifetime (500 ticks) adds strategic depth
- **Collision Detection**: Robust system for walls, self-collision, and obstacles
- **State Management**: Six distinct game states for smooth transitions

### 🔧 Technical Architecture

```
┌─────────────────────────────────┐
│        Application Layer        │
│     (snake.c, snake.h)         │
│           main.c               │
└─────────────────┬───────────────┘
                  │
┌─────────────────┴───────────────┐
│      Controller Layer           │
│  ┌─────────┬─────────┬────────┐ │
│  │   KBC   │  Timer  │  VBE   │ │
│  │(i8042)  │(i8254)  │(VESA)  │ │
│  └─────────┴─────────┴────────┘ │
└─────────────────┬───────────────┘
                  │
┌─────────────────┴───────────────┐
│       Hardware Layer            │
│    MINIX OS + Hardware x86      │
└─────────────────────────────────┘
```

## 🔧 Technical Components

### Device Controllers
- **Keyboard Controller (KBC)**: Handles user input through i8042 controller
- **Timer Controller**: Manages game timing, speed control, and special effects
- **Graphics Controller (VBE)**: Provides video mode initialization and rendering functions

### Core Files
```
src/
├── snake.c, snake.h      # Core game logic and state management
├── main.c                # Program entry point and LCF configuration
├── KBC.c, KBC.h         # Keyboard controller implementation
├── timer.c               # Timer controller for game timing
├── VBE.c, VBE.h         # Graphics controller for video output
├── utils.c               # Auxiliary functions for I/O operations
├── button_pixmap.h       # XPM graphics definitions
└── i8042.h, i8254.h     # Hardware interface definitions
```

## 🎮 Controls

| Key | Action |
|-----|--------|
| ↑↓←→ | Control snake direction (normal/inverted based on mode) |
| Enter | Select menu options |
| ESC | Return to menu / Exit game |
| Q | Navigate between screens (rules/mode selection) |

## 🚀 How to Run

1. **Compile the project** on MINIX:
   ```bash
   make
   ```

2. **Run the game**:
   ```bash
   lcom_run snake
   ```

3. **Navigate the menu**:
   - Use arrow keys to select options
   - Press Enter to confirm selections
   - Choose your preferred game mode
   - Enjoy the game! 🎮

## 🎯 Game States

The game manages six distinct states:
- **MENU**: Main menu navigation
- **MODE_SELECT**: Game mode selection
- **RULES**: Rules and instructions display
- **PLAYING**: Active gameplay
- **END**: Game over screen with final score
- **EXIT**: Clean program termination

## 🏆 Educational Objectives

This project demonstrates mastery of:
- **Low-level Programming**: Direct hardware interaction in MINIX
- **Device Driver Development**: Implementation of keyboard, timer, and graphics controllers
- **Game Development**: Advanced game mechanics and state management
- **Software Architecture**: Layered design with clear separation of concerns
- **Performance Optimization**: Efficient rendering and resource management

## 📈 Advanced Features

- **Dynamic Obstacle Generation**: Walls mode creates evolving challenges
- **Probabilistic Power-up System**: Balanced distribution of special fruits
- **Temporal Game Mechanics**: Food lifespan adds time pressure
- **Efficient Memory Management**: Optimized for MINIX environment
- **Modular Codebase**: Easy maintenance and potential platform porting

---

**🎓 Academic Achievement**: This project successfully demonstrates comprehensive understanding of computer systems fundamentals and results in a high-quality technical application that exceeds basic requirements.

*Developed as part of the Computer Laboratory course at FEUP, showcasing practical application of low-level programming concepts and hardware integration skills.*
