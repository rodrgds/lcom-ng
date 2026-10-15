# LCOM18G4
Brick Blitz

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)


- **Project name:** LCOM18G4 - Brick Blitz
- **Short description:** Breakout Clone
- **Environment:** Minix console
- **Tools:** C
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [LC](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=520323) (Computer Laboratory)
- **Project grade:** 15
- **Group members:**
    - Afonso Castro (up202208026@fe.up.pt)
    - José Martins (up202204857@fe.up.pt)
    - Pedro Santos (up202205900@fe.up.pt)
    - Rodrigo de Sousa (up202205751@fe.up.pt)

---
## Demo

[YouTube](https://youtu.be/ETXBJ4NJsDg)


## Installation
    cd labs/proj
    git clone git@github.com:rodeso/lcom.brick-blitz.git
    cd src
    make
    lcom_run proj
    
## Project Description

Our game is a remake of the well-known Breakout from 1976.

The context consists of several layers of blocks positioned in rows at the top of the screen that need to be destroyed by the player before they descend and reach the lower section of the screen. 

The player is given control of a board located in the lower section with the ability to move horizontally, which they must use to initially launch (ENTER key) and then reflect the movements of a ball that will be used to eliminate the blocks when in contact with them.

The initial launch angle is chosen by the player using the mouse, and the movement of the board is controlled using the A and D keys.
If the player fails to receive and reflect the main ball and lets it pass the lower limits of the screen, they will lose one of the 3 lives they are given.

To make the game more dynamic, by destroying each block there is a chance of winning a powerup which can be an extra ball which comes into play immediately (and if lost, does not affect the player's lives), or a missile which, when fired (SPACE key), flies vertically and can have a destructive trail of up to 6 blocks (missiles can be accumulated).

The game ends with victory, when all the blocks have been destroyed, or with defeat, when all 3 lives have been lost or the blocks have successfully reached the bottom section of the screen. 
