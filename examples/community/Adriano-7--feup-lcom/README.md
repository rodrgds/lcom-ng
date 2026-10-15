***

# Space Invaders for Minix 3

> **Project**
> <br />
> Course Unit: [Computer Laboratory (LCOM)](https://sigarra.up.pt/feup/pt/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=520323), 2nd year
> <br />
> Course: Master in Informatics and Computing Engineering (LEIC/MIEIC)
> <br />
> Faculty: **FEUP** (Faculty of Engineering of the University of Porto)
> <br />
> Project evaluation: **19**/20

---

## Project Goals

The objective of this project was to develop a complete, fully functional game from scratch by writing low-level hardware device drivers for the **Minix 3** operating system. 

Space Invaders was built bypassing standard operating system APIs and instead interacting directly with the PC's I/O ports and interrupt controllers. 

### Game Screenshots:
<table>
  <tr>
    <td><img src="proj/doc/1.png" width=500></td>
    <td><img src="proj/doc/2.png" width=500></td>
  </tr>
 </table>

---

## Technical Approach

The project was structured using the **Model-View-Controller (MVC)** architectural pattern to separate the game logic, game state, and low-level hardware interactions. 

### 1. Hardware Interfacing (Device Drivers)
All hardware interaction was written in C, handling hardware interrupts (IRQs) and I/O port reading/writing:
* **Video Card (VBE):** Configured the VBE graphics mode `0x105` (1024x768). We implemented functions to map physical memory to the process's virtual address space, handle double-buffering, and draw `XPM` sprite images pixel-by-pixel.
* **Keyboard (KBC):** Implemented an interrupt handler to read scancodes from the keyboard output buffer. This provides real-time, zero-latency inputs for moving the spaceship (Left/Right arrows) and firing lasers (Spacebar).
* **Mouse (PS/2):** Configured the mouse to enable data reporting, parsing the 3-byte data packets upon interrupt. This allows the player to seamlessly navigate and click through the game's menus.
* **Timer (i8254):** Programmed the programmable interval timer to generate interrupts at a specific frequency (sys_hz), driving the game loop, frame rendering, and entity movement updates at a steady frame rate.
* **Real-Time Clock (RTC):** Handled RTC interrupts and read from its registers to keep track of the current real-world date and time. This data was used to timestamp high scores in the Leaderboard system.

### 2. Game Architecture & Logic
* **Model (`src/model`):** Contains the game state and data structures. It stores the `Player`, `Monster`, `Bullet`, `Cursor`, and `Menu` states. Every visual element inherits a base `DrawableObject` structure with position coordinates and an assigned XPM image.
* **Controllers (`src/controllers`):** Centralizes the core gameplay loop. It handles state transitions (Main Menu $\rightarrow$ Game $\rightarrow$ Game Over $\rightarrow$ Leaderboard), processes hardware interrupts, updates entity positions, and calculates bounding-box collisions between lasers, monsters, and the player.
* **View (`src/devices/video.c`):** Renders the current state of the Model to the screen on every timer tick, clearing previous frames and drawing updated XPMs to give the illusion of smooth movement.

---

## Running the Game

To play the game, you need to compile and run it within a Minix 3 virtual machine with the LCOM framework installed.

1. Open the terminal and navigate to the directory containing the `Makefile` (`proj/src/`).
2. Clean previous build files:
   ```bash
   make clean
   ```
3. Compile the project:
   ```bash
   make
   ```
4. Run the compiled executable using the LCF framework:
   ```bash
   lcom_run proj
   ```

---

## Tech Stack

**Language:** C
<br>
**OS:** Minix 3.1.8
<br>
**Tools:** LCF (LCOM Framework), Make, Doxygen (Documentation)

## Team (T14-G05)

- Adriano Machado (up202105352@fe.up.pt)
- José João (up202108818@fe.up.pt)
- Luís Cunha (up201709375@up.pt)
- Rodrigo Ribeiro (up202108679@fe.up.pt)
