# Bomberman Game coded in C

Laboratório de Computadores - 2023/2024

Licenciatura em Engenharia Informática e Computação

# Running The Game
Our game supports modes 0x14C and 0x115 of the Minix Virtual Machine, thus, the user can select which mode they want to play by running the game with differing commands:

```
Command “lcom_run proj”: will run the game in VBE mode 0x115.
Command “lcom_run proj [any character]”: will run the game in VBE mode 0x14C.
```

<img alt="game title" src="images/title.jpg">

Upon starting the game the user will be presented with a menu screen, where they can read the game instructions, start the game or exit. This menu screen serves as the gateway for closing the game, whenever the user wins, loses or goes back, they will be directed to the menu.

<img align="left" alt="main menu" height="300px" src="images/instructions.jpg">

In our game the user can go to the instructions menu and learn how to play it. We used both keyboard and mouse to allow the user to move and deploy bombs.
The controls are: 

**Keyboard arrows** UP, DOWN, LEFT AND RIGHT to move.

**SPACE bar** deploys bombs.

**Steven (our hero)** moves with the mouse.

**Mouse-right-click** button to deploy bombs.

**ESC** to escape (go back to menu)

------------------------------------------------

Upon entering the game, the user will be able to see the map (that changes its configuration considering the time of the day) and some of the functionalities of our game:

<img align="left" alt="main menu" height="300px" src="images/game.jpg">

**Steven**: our hero that moves upon user input.

**Lives**: Steven starts with 3 lives that can be seen at any time on the screen.

**Green Monsters**: move randomly at a rate of one position per second.

**Purple Monsters**: move randomly at a rate of one position per 1/10 second.

**Breakable Walls**: purple blocks that can break by an explosion of a bomb. Inside each of them there is a 10% chance of having an upgrade (additional life or unlimited bomb upgrade).

**Unbreakable Walls**: pink blocks that stay the same throughout the game.

**Clock**: the Real-Time-Clock device counts the time displayed on the screen.


Whenever the user destroys a breakable well with a bomb, it has a 10% chance of getting an upgrade.

**Additional Life**

**Bomb Upgrade**

We hope you have a good time with our game!

Students:

Amanda Silva (up202211647@fe.up.pt)

Leonardo Garcia (up202200041@fe.up.pt)

Manoela Américo (up202201391@fe.up.pt)

Marcel Medeiros (up202200042@fe.up.pt)

