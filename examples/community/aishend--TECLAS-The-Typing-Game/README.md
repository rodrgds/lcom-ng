# Teclas

**Teclas** is a fast-paced typing game developed for the LCOM course at [FEUP](https://sigarra.up.pt/feup/en/). Inspired by sites like [MonkeyType](https://monkeytype.com/) and [10FastFingers](https://10fastfingers.com/), the game runs entirely within the LCOM/Minix environment, built on top of the low-level I/O device drivers developed progressively throughout the semester's lab assignments.

---

## Game Modes

### Normal
Classic typing speed mode. A sequence of words appears on screen and the player must type them as fast and accurately as possible. The run is timed and the result is saved to a **leaderboard**, sorted from highest to lowest score.

### Dino
Inspired by the Google Chrome offline dinosaur game, but with a twist — instead of pressing a key to jump or duck, the player must type the displayed words to execute those movements. Type the right word in time to jump over or duck under obstacles and survive as long as possible.

### Survival
A "monster" chases your cursor across the screen. Type words correctly to keep moving and run as far away from it as possible. If the "monster" catches the cursor, it's game over.

---

## How to Run

The project runs inside the LCOM/Minix virtual environment. You must compile all labs and the main project before running it.

### Build all labs and run the project automatically

```sh
# From the repository root — builds all labs + project, then launches it
sh make_all.sh
```

Or step by step:

```sh
cd lab1 && make && cd ..
cd lab2 && make && cd ..
cd lab3 && make && cd ..
cd lab4 && make && cd ..
cd lab5 && make && cd ..
cd teclas && make && cd ..
lcom_run teclas/proj
```

If the game hangs or you need to stop it forcefully:

```sh
lcom_stop proj
```

---

## Project Structure

```
shared/
├── lab1/          # Timer lab
├── lab2/          # Keyboard lab
├── lab3/          # Mouse lab
├── lab4/          # Graphics card lab
├── lab5/          # RTC lab
├── teclas/        # Main project
│   ├── src/       # Source files
│   │   └── states/    # Game state machines
│   ├── xpm/       # Sprites and assets
│   ├── docs/      # Generated Doxygen documentation
│   └── results/   # Leaderboard save files
├── make_all.sh    # Build all labs + project
└── README.md
```

---

## Devices Used

| Device        | Purpose |
|---------------|---------|
| **Keyboard**  | Core input — detect keypresses for all game modes |
| **Mouse**     | Navigate menus |
| **Timer**     | Game loop timing, word speed control |
| **Graphics Card** | Rendering sprites, text, backgrounds, and animations |
| **RTC**       | Timestamps for leaderboard entries |

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## Declaration of Responsible AI Use

We declare that:

1. We are responsible for all code and documentation in this repository, and we understand that we must be able to explain and justify any part of it on request.
2. We have used AI-based tools (e.g., code assistants, chatbots, or generators) only to support our learning, not to bypass the intended learning outcomes or any assessment rules.
3. Wherever AI tools contributed to this work, we have:
   - Used them within the limits set by the course policies and institutional regulations.
   - Reviewed, tested, and, where necessary, edited the outputs, taking full responsibility for their correctness, originality, and legality.
   - Ensured that no confidential, personal, or sensitive data were shared with AI tools.
4. We have not used AI tools to generate complete solutions that we present as entirely our own unaided work, and we have avoided plagiarism, whether from AI outputs or other sources.
5. If asked, we will provide details of which tools we used, for which files or parts of the project, and how we verified and adapted their outputs.

Signed: `David Leão`, `Leandro Moreira`, `Nicholas Oliveira`, `Tomás Silva`  
Date: `05/06/2026`

---

## Authors

LCOM Project — Group **GRUPO_2LEIC03_05**

| Name | Email |
|------|-------|
| David Leão | up202404312@up.pt |
| Leandro Moreira | up202405528@up.pt |
| Nicholas Oliveira | up202403429@up.pt |
| Tomás Silva | up202403435@up.pt |
