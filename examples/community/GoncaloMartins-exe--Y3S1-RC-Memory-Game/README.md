## Memory Game – LCOM Final Project 2025

## Project Description

**Memory Game** is a classic card-matching game developed in C for the **Minix 3** operating system. Players flip fruit-themed cards to find matching pairs, trying to complete the game in the shortest time with the fewest moves.

The game runs using low-level programming concepts studied in the LCOM course, including **interrupt handling**, **graphics rendering**, and **input device management**.

## Project Structure

- src/
- ├── controllers/ # Low-level device drivers (Timer, Keyboard, Video Card)
- ├── game_logic/ # Game mechanics: cards, board, game state, player logic
- ├── menu/ # Main menu and navigation UI
- ├── sprites/ # Sprite management and drawing
- ├── images/ # XPM images (cards, backgrounds, UI)
- ├── main.c # Entry point
- ├── Makefile # Build configuration

## How to run the game

The game was developed in C and requires a Minix environment configured on your computer.

### Commands

```bash
$ make            # Builds the project
$ lcom_run proj   # Runs the project
$ lcom_stop proj  # Stops the project manually (if needed)
```
##  Devices Used

### Timer (i8254)
- Generates regular interrupts for frame updates and animations.
- Tracks elapsed time during gameplay.

### Keyboard (i8042)
- Used to navigate the main menu (e.g., start game, exit).
- Allows keyboard-based player interaction.

### Graphics Card (VBE mode)
  - Displays all visual elements including:
  - Game board
  - Cards and animations
  - Menu and HUD


## How to play

- Use the keyboard arrows to navigate through the cards.

- Use enter to select a card to flip it and reveal its symbol.

- Flip two cards per turn. If the symbols match, the pair will not flip again.

- Try to match all pairs in the shortest time and with the fewest moves.

- The game ends when all pairs are matched.

- Use ESC to quit the game.

## Game Images

1. Entering the game

![alt text](proj/Images/Loading.png)

2. Menu

![alt text](proj/Images/Menu.png)

3. Game page - Initial state

![alt text](proj/Images/InitialState.png)


4. Game page - After few moves

![alt text](proj/Images/GamePage2.png)

5. Winning page

![alt text](proj/Images/Winning.png)


## Group: GRUPO_2LEIC13_7

## Course: Computer Lab

### Group members:

1. Aléssia Jorge (up202109242@fe.up.pt)
2. Gonçalo Da Cruz Martins (up202306967@fe.up.pt)
3. Henrique Santana Vilarinho (up202307037@fe.up.pt)
4. Mariia Hutsul (up202310202@fe.up.pt)

## Institution: FEUP
## Date: May 30, 2025
