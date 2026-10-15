# Report 
## Disclaimers 
All images used in this project were obtained from publicly available sources on the Internet. We do not own or claim any rights to these sprites. The assets used include the <a href = "https://pixelfrog-assets.itch.io/tiny-swords">Tiny Swords</a> sprite sheet by Pixel Frog, the <a href = "https://foozlecc.itch.io/void-environment-pack">Void Environment Pack</a> by Foozle, the <a href = "https://nyknck.itch.io/pixelarteffectfx017">Fireball effect</a> by NYKNCk all of which are distributed under the CC0 license. We also used sprites from the game Bad Ice Cream by Studio Nitrome and one AI generated animation.

This project was created solely for educational purposes.

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

Signed: Gustavo Bastos, Isaac Santos, Dalton Yuiti Kitadani Liaw Djoesman, Tiago Su
Date: 26 de Fev / 2026

## What was our goal 
For this project, we aimed to create a tower defense game, a genre in which players manage resources and build defenses across the map in order to protect themselves from waves of enemies. The core gameplay loop involves gathering fruits and spending them to build towers that automatically attack enemies as they approach in successive waves.

![Gameplay1](./misc/gif1.gif)
![Gameplay2](./misc/gif2.gif)


## Note before compiling the project 
Due to an unknown reason, the current working directory is always the root of the operating system. That's why, it is important to check in <a href="./project/include/data.h"> data.h </a> file if the file paths section match your current directory tree.

## How to build and run
The final program is built from `project/src` using the course LCOM toolchain (`minix.lcom.mk`). It links against our previous labs (`lab2` timer, `lab3` keyboard, `lab4` mouse, `lab5` VESA BIOS).

Build the lab libraries:

```bash
cd /lab2 && make
```

```bash
cd /lab3 && make
```

```bash
cd /lab4 && make
```

```bash
cd /lab5 && make
```

Build the game:

```bash
cd /project/src && make
```

Run the game:

```bash
cd /project/src && lcom_run proj
```

## Level and map data
The game has **three separate levels**, each with its own map assets under <a href="./project/resources/maps">`project/resources/maps`</a> (`map1`, `map2`, `map3`) and a different role in the overall experience:

- **Level 1 (`map1`):** a smaller, easier layout that acts as our **tutorial**. The match is tuned so the player can win in **at most about seven minutes**, without the pressure of a boss fight—ideal for learning fruits, building, and basic waves.  
- **Level 2 (`map2`):** a **larger** map and a longer run (**about eight minutes**). When that time window is over, the player must **defeat the boss** to finish victorious; the level is built around that escalation.  
- **Level 3 (`map3`):** a **different** layout again—another version of the game with its own map geometry, paths, and pacing, distinct from the first two.

Each level still uses the same technical pipeline: three comma-separated grid files per folder—**`walls.csv`**, **`path.csv`**, and **`water.csv`** (lake tiles). The grid size and tile size match the constants in <a href="./project/include/data.h">`data.h`</a> (`MAP_TILES_X`, `MAP_TILES_Y`, `TILE_SIZE`).

## How did we structure the project
For our project, we adopted an OOP approach focusing on the SOLID principles, particularly the Single Responsibility Principle. Although, due to time constraints, we were unable to fully accomplish it in all files. Regardless, in a general view, we can define the flow of our project in this abstract UML.

![Diagrama UML geral](./misc/general.png)

### Application
The <a href="./project/src/application.c">application.c</a> is the entry point of our project; it is the file that starts our program and mainly handles all the initialization of the drivers and their interrupts. It also owns a `Game` object whose update method is called on each timer 0 interrupt.

![Diagrama UML application](./misc/application.png)

### Game
The <a href="./project/src/game.c">game</a> class has the responsibility to handle the state of the game, i.e. it should be able to perform all the necessary actions of each state and transition between them. For that reason, we designed a state machine with the corresponding states:
- Main Menu: state that should show the startup menu
- Level Selector: state that should show a menu in which you can choose which level to play
- Running Game: state that should display the actual game
- Game Over: state that should display when you lose
- Win Game: state that should display when you win

![Diagrama UML gameState1](./misc/gameState1.png)
![Diagrama UML gameState2](./misc/gameState2.png)

### Arena 
The <a href="./project/src/arena.c">Arena</a> class is a large wrapper that holds all running objects from the running game and calls their update methods. Besides being a wrapper, it also holds some auxiliary functions to help components connect with other objects, for example a method to help colliders detect if they are colliding with other objects, or a method to check if the game is over. It reads the level CSVs, maintains wave timing, fruit spawning, towers, and UI such as the build and upgrade menus.

### Components
To explain the rest of our project, we need to explain the components directory. These are black-box classes that should have a single purpose and should be independent or only depend on other components in order to work. These files were the first structure created for our project and we used some ideas from game engines like Unity and Godot to help start our project; for that reason we defined that we would need the following components:
- <a href="./project/src/boxCollider.c">BoxCollider</a>: a simple object that represents a rectangular hitbox
- <a href="./project/src/rigidBody.c" >RigidBody</a>: a component that needs a collider and handles all necessary movement for objects that need to collide with other objects (used for the player)
- <a href="./project/src/timer.c">Timer</a>: a component to count down time
- <a href="./lab5/spriteXMP.c">SpriteXPM</a>: a component that should be able to handle the displaying of an XPM
- <a href="./project/src/animatedSprite.c">AnimatedSprite</a>: a component able to display animations 
- <a href="./project/src/button.c">Button</a>: a component that has `onClick` and `onHover` behaviour

### Player
The <a href="./project/src/player.c">Player class</a> follows something similar to an MVC architecture with only a <a href="./project/src/playerModel.c">model class</a> and a <a href="./project/src/playerController.c">controller class</a> to handle the visual and input logic. To handle all variations of player behaviour, we defined player states to process inputs and decide which animation to show. To make the player easier to manage, we use animated sprites for different actions, a rigid body for collisions with walls, and box colliders for the hitbox.

![Diagrama UML playerState1](./misc/playerState1.png)
![Diagrama UML playerState2](./misc/playerState2.png)

### Tile and Collectable
The <a href="./project/src/tile.c">Tile class</a> and <a href="./project/src/collectable.c">Collectable class</a> are classes that have a box collider and SpriteXPM and they represent a generic tile to display on the map. The collectable is special in that it disappears after being collected, to represent the fruits the player needs to gather. In order to detect collisions with fruits, we use the arena as an observer to detect which collectable collided with the player.

### Generic Tower and Projectiles
In order to have a modular way to generate different types of tower, we decided to use an inheritance-style approach. We defined the generic tower as an abstract base that is implemented by several tower types:
- <a href="./project/src/basicTower.c">Basic Tower</a>;
- <a href="./project/src/basicLongTower.c">Long Tower</a>;
- <a href="./project/src/fastTower.c">Fast Tower</a>;
- <a href="./project/src/piercingTower.c">Piercing Tower</a>;
- <a href="./project/src/eletrictTower.c">Electric Tower</a>;
- <a href="./project/src/mortarTower.c">Mortar Tower</a>;

Besides that, each tower also has a projectile stored in it. The <a href="./project/src/projectile.c">Projectile class</a> also uses inheritance to keep a modular structure; we created the following projectile kinds:
- normal: a normal projectile that stops after the first valid collision
- piercing: a projectile that pierces everything until it hits a wall
- sky fall: a projectile that falls from the sky and is affected by gravity
- near: an immobile projectile 

### Monsters and its Generators
To ensure modularity, the <a href="./project/src/monster/monster.c">monster</a> (pseudo-)"class" was designed to keep all general information in it. Through this factory-based approach, the creation of a new (specific) monster was simplified by the implementation of a new (pseudo-)"constructor" that would define its attributes, such as the speed, HP and the animation generator function. Furthermore, a clean implementation of the facade pattern for the MVC architecture components was accomplished in this class, which aggregates all the distinct data and functionalities from <a href="./project/src/monster/monsterModel.c">monsterModel</a>, <a href="./project/src/monster/monsterView.c">monsterView</a> and <a href="./project/src/monster/monsterController.c">monsterController</a>. 

The monsters we created were:
- <a href="./project/src/monster/pawnMonster.c">pawn</a>: a simple basic monster with a medium speed and HP.
- <a href="./project/src/monster/alienMonster.c">alien</a>: the fastest monster in the game, but also the weakest, even though it doesn't make him that easy to kill.
- <a href="./project/src/monster/spearManMonster.c">spearMan</a>: a little bit slow, tough man with a long spear that is hard to kill.
- <a href="./project/src/monster/bossMonster.c">boss</a>: as the name says, it is an ultimately powerful beast and probably the last thing you want to see. His massive aura farming while walking is going to make you tremble with fear and defeating him is our final challenge to you so you can claim ultimate victory.

The <a href="./project/src/monster/monsterGen.c">MonsterGen</a> (pseudo-)"class" is responsible for the generation of monsters along a single <a href="./project/src/monster/monsterGen.c">path</a> (a monsterGen is required for each path). It implements a dynamic object pool pattern in order to avoid the unnecessary memory (de)allocation. When the monsters die or finish their path, they are tracked by a stack of deactivated monsters (indices) to be reused to initialize new monsters. 
The monsterGen class is also very generic and customizable, since you can control very precisely how many and which monsters you want to create in each instant of time using only 2 functions that are passed in the monsterGen (pseudo-)"constructor". 
For the timeGenFunc callback, the class provides 2 parameters: a Timer* (that can be used for the spawn rate) and a float (with the total elapsed time).

## What devices did we use
### VESA BIOS
The most obvious usage of a driver is the VESA BIOS which was used to fill the VRAM and display images on the screen. This driver can be seen wrapped in the object <a href="./lab5/terminal.c"> Terminal </a> used in conjuction with <a href="./lab5/spriteXMP.c"> SpriteXPM </a>. Besides the basic functionalities from labaratories exercises, we also used the page flipping in order to avoid screen tearing (NOTE: in order to implement the page flipping, we resorted to AI assistance).

### Keyboard Controller
The keyboard driver was used to detect which state the keys from the keyboard are on. It was important to detect if a key is being held, if it was the first instance of being held or it is not being pressed, that's why we defined an enum to represent that states. All logic from the driver handling is wrapped in a singleton called <a href="./project/src/inputHandler.c"> InputHandler</a> with a simple API to be used. We decided to design this as a singleton because during all the project execution, we only have one object that will change the state of the singleton and the rest of the program only queries the state of the keyboard is in, making the usage of a singleton for this purpose optimal and easier to maintain and access to keyboard information.

### Mouse Controller
The mouse was used to display a cursor and to be able to interact with UI. All logic is abstracted inside the <a href="./project/src/cursorHandler.c"> CursorHandler class </a> which handles all controllers' data sent by the Application class and it also handles its own visual display. Similar to the keyboard controller, this class was designed as singleton because of the same reasons from the keyboard, there is only one class that changes the state of this object and the rest of the game only request information.

### Timer 0 - interrupt
The Timer 0 was primarily used to have a game running 60 FPS so that every time, the interrupt is fired we execute a game loop.

### RTC - not used
During our project development, we tried to think in useful ways to use the RTC and the only valid option that came to mind was to use to calculate the deltaTime or the elapsed time between frames. But unfortunately, the RTC is too slow and was hindering our game performance so we decided to discard the idea and use the timer 0 to calculate the deltaTime.

## What are the differentiating features of our project?
Using an OOP approuch, we believe we reached a very complete and complex project, with a game that starts and ends very concisely. The big differentiating feature is how we designed our project because from the beginning, we always preoccupied in making modularity as the main point of all classes. That led us to a project that is very easy to add new features, new levels, new towers, new enemies and easy to maintain at a certain degree of understanding of the codebase. Although, due to time constraints, the Arena became not very modular. 

Beside the modularity of our code, we could say that we created a very complete and enjoyable game.

## Authors and acknowledgment

LCOM Project for group GRUPO_2LEIC13_2.

Group members:<br>
Dalton Yuiti Kitadani Liaw Djoesman (up202407080@up.pt)<br>
Gustavo Bastos (up202403874@up.pt)<br>
Isaac Santos (up202407099@up.pt)<br>
Tiago Su (up202403468@up.pt)<br>
