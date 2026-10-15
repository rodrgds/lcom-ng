# LCOM Project - Space Shooter 🚀

LCOM Project for group GRUPO_2LEIC18_7;.

Group members:

1. Samuel Maciel (up202108697@up.pt)
2. Henrique Marques (up202208752@up.pt)
3. Diogo Faria (up202205201@up.pt)

## Game Description
This project is a space shooter game developed using the LCOM framework. The game features a player-controlled spaceship that can move and shoot asteroids, which are randomly generated. The objective is to destroy as many asteroids as possible while avoiding collisions. The game includes a menu system for starting the game and displaying the score.

## Goal
Our goal is to create a simple yet engaging space shooter game that demonstrates our understanding of the LCOM framework and its capabilities. We aim to implement basic game mechanics such as player movement, shooting, collision detection, and score tracking, while also providing a user-friendly interface.

## Architecture
The architecture of our game is designed on the LCOM framework, which provides a structured way to manage game components. The main components of our architecture include:

- **Game Loop**: The core of the game that handles updates and rendering.
- **Player**: Represents the spaceship controlled by the player, handling movement and shooting.
- **Asteroids**: Randomly generated obstacles that the player must destroy.
- **Bullets**: Projectiles fired by the spaceship to destroy asteroids.
- **Menu System**: An interactive menu for starting the game and displaying the score.
- **Collision Detection**: Logic to determine when bullets hit asteroids or when the spaceship collides with an asteroid.
- **Score Tracking**: Keeps track of the player's score based on the number of asteroids destroyed.

## Devices Used
The game is designed to run on the following devices:
- **Mouse**: Used to enter the game on menu.
- **Keyboard**: Used to control the spaceship.
- **Video**: Displays all game graphics and information (drawing .xpm files).
- **Timer**: Used to manage screen updates, bullets and asteroids spawn.

## What are the differentiating features of our project?
One interesting aspect of our implementation is how we handle screen rendering to improve performance and visual smoothness. We use double buffering to avoid screen tearing and flickering, and we take it a step further by preloading all static elements into a local buffer. Each frame, we copy this static buffer into the drawing buffer and then render only the dynamic elements like the spaceship, bullets, and asteroids. This approach reduces redundant operations and ensures a smoother gameplay experience.

Additionally, the game features an interactive menu system controlled by the mouse, which provides a more intuitive user interface right from the start, enhancing the overall experience.
