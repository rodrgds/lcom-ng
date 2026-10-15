# LCOM Project - GROUP_2LEIC18_4

This repository contains the project developed for LCOM (FEUP), where a Snake Game was implemented in the Minix/LCOM environment with a focus on low-level systems programming and direct hardware interaction. The code follows a modular, event-driven approach based on interrupts, keeping game logic, menu logic, and device controllers clearly separated.

Repo structure:
- [Lab 2](https://github.com/mariajvieira/feup-lcom/tree/main/lab2): Timer (PIT): Drives the main game loop using periodic interrupts, regulating update rate and snake speed across different difficulty levels.

- [Lab 3](https://github.com/mariajvieira/feup-lcom/tree/main/lab3): Keyboard (PS/2 / i8042): Real-time input handled via interrupts, decoding scan codes to control both menu navigation and in-game movement.
- [Lab 4](https://github.com/mariajvieira/feup-lcom/tree/main/lab4):  Mouse (PS/2): Interrupt-driven mouse packet handling used for user interaction (e.g., menu navigation and/or game interaction, depending on the implementation).​
- [Lab 5](https://github.com/mariajvieira/feup-lcom/tree/main/lab5): Graphics card (VGA / VBE): Initializes a graphics mode and renders directly to video memory (including double buffering to reduce flickering), drawing menus, gameplay, score, and the game-over screen.
- [Proj](https://github.com/mariajvieira/feup-lcom/tree/main/proj): Snake game: Final integrated project combining timer, keyboard, mouse, and graphics for a complete interactive game.
​​

Group members:
1. Duarte Marques (up202204973@up.pt)
2. Maria Vieira (up202204802@up.pt)
3. Marta Cruz (up202205028@up.pt)

