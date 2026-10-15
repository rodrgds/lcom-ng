# LCOM Project - Bomberman

LCOM Project for group GRUPO\_2LEIC04\_5.

## Group members

1. Nuno Gomes ([up202306826@edu.fe.up.pt](mailto:up202306826@edu.fe.up.pt))
2. Pedro Castro ([up202200044@edu.fe.up.pt](mailto:up202200044@edu.fe.up.pt))
3. Vasco Lemos ([up202306905@edu.fe.up.pt](mailto:up202306905@edu.fe.up.pt))

## The game

![Bomberman logo](proj/doc/images/logo.png)

Our project is a recreation of the classic arcade game Bomberman.

### Gameplay

Your mission is simple: clear each level by eliminating all enemies using bombs. Navigate through maze-like levels filled with destructible bricks and indestructible walls. Place bombs strategically to trap and defeat enemies, but watch out – your own bombs can hurt you too!

### Features

- **Multiple levels** with increasing difficulty
- **Smooth animations** for player movement, bomb explosions, and enemy movement
- **Power-ups** hidden behind destroyed bricks (extra lives)
- **Score tracking** - earn points for destroying bricks and defeating enemies

## Controls

**Keyboard:**

- **Arrow keys** or **WASD** - Move your character and navigate menus
- **Spacebar** - Drop a bomb at your current location
- **Enter** - Pause game / Select menu option
- **Escape** - Pause game

**Mouse (optional):**

- **Click** - Drop bombs around your character (including diagonally)
- **Click** - Select menu options

## Game mechanics

- **Bombs explode after 3 seconds** with a cross-shaped blast pattern
- **Explosions destroy bricks** and can reveal power-ups or the exit door
- **Chain reactions** happen when explosions hit other bombs
- **Complete a level** by eliminating all enemies and reaching the door
- **Lose a life** when hit by explosions or touching enemies
- **Game over** when all lives are lost

## How to play

1. **Move** around the maze using arrow keys or WASD
2. **Collect** power-ups (hearts) for extra lives
3. **Place bombs** with the spacebar or the mouse to destroy bricks, reveal the exit door and defeat enemies
4. **Avoid** your own explosions and enemy contact
5. **Reach the door** to advance to the next level

## Running the project

To run the project in your MINIX-LCOM virtual machine:

```bash
cd /home/lcom/labs/proj/src
make
lcom_run proj
```

If you encounter any issues:

```bash
lcom_stop proj
make clean && make
lcom_run proj
```

## Documentation

To generate the full documentation (including Doxygen and the report), run on your host machine:

```bash
cd proj/doc
make
```

Make sure you have `doxygen` and `pdflatex` installed.

Enjoy!
