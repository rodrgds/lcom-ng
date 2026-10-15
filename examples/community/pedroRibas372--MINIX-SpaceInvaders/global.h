#ifndef GLOBALS_H
#define GLOBALS_H
#include "sprites/sprites.h"
#include "game/game.h"
#include "menulevel/menulevel.h"
#include "menu/menu.h"

// how many enemies loaded
extern int indexEnemies;
// enemies array, max 25;
extern Sprite* enemies[25];
// hero Sprite
extern Sprite* hero;
extern Sprite* lvl;// header icon
extern Sprite* hp; // header icon
extern Sprite* scr;// header icon
extern Sprite* finalWin;//Game won drawing
extern Sprite* finalLost;//Game lost drawing
extern Sprite* levelButton;// Menu Buttons
extern Sprite* exitButton;// Menu Buttons
extern Sprite* startButton;// Menu Buttons
extern Sprite* SpaceInvaders;// Menu Buttons

extern uint8_t latest_scancode [2]; // keyboard read value
extern uint8_t size;// kbc readByte 
extern uint8_t done;// kbc readByte
extern int score; //header value
extern int level; //header value
extern int health;//header value
extern LevelSelection selected_level; // menulevel select variable
extern MenuSelection current_button; // menu select variable
extern xpm_map_t numbers[10]; // array that stores the xpm images of the numbers
extern uint32_t counter;// timer interrupt counter
extern GameState state;
extern int totalBullets;// max bullets in bullet array
extern int indexBullets;// updates +1 on bullet create

/**
 * @brief Selects if enemies should move left or right, starts at 0, it is only incremented, and follows this line of logic:
 * 
 * 0(right) -> 1 (right) -> 2 (right) -> 3 -> -3 (left) -> -2(left) -> -1(left) -> 0(right)
 * 
 */
extern int moveSelecter;



extern Sprite* bullets[50]; // Bullet array
extern int totalEnemies; // enemies left alive
extern uint8_t* double_buffer; // buffer that will update the main buffer 60 times per second

#endif

