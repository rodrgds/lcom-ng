#include <lcom/lcf.h>
#pragma once

/**
 * @brief Enumeration to help handle the tux's moves.
 * 
 */
typedef enum {WALKING_R, WALKING_L, STANDING_R, STANDING_L, JUMPING_R, JUMPING_L, AFTERJUMPING_R, AFTERJUMPING_L, DUCKING_R, DUCKING_L} Moves;

/**
 * @brief Enumeration to help handle all possible game states.
 * 
 */
typedef enum {INITIAL_MENU, INSTRUCTIONS_MENU, HIGHSCORE_MENU, GAME_RUNNING, GAME_PAUSED, GAME_OVER, EXIT} State;

/**
 * @brief Enumeration to help handle the states of the left button.
 * 
 */
typedef enum {LEFTBUTTONPRESSED, LEFTBUTTONRELEASED} LeftButtonState;

/**
 * @brief Enumeration to help handle the enemies.
 * 
 */
typedef enum {SPIKY, ZEEKLING, SPIKE,KAMIKAZE}Enemy_type;


/**
 * @brief Struct for Fireball
 * 
 */
typedef struct FireBall{
    xpm_image_t sprites[3]; ///<The sprites used to make the fireball
    int xpos; ///<X coordinate of the fireball's position
    int ypos; ///<Y coordinate of the fireball's position
    int currentsprite; ///<The sprite currently in use
    int front; ///<If it's turned to the front or to the back (1 for front, 0 to back)
}FireBall;

/**
 * @brief Struct for Coin
 * 
 */
typedef struct Coin{
    xpm_image_t sprites[8]; ///<The sprites used to make the coin
    int currentsprite; ///<The sprite currently in use
    bool catched; ///Whether or not the coin was catched (true if catched, false otherwise)
    int xpos; ///<X coordinate of the coin's position
    int ypos; ///<Y coordinate of the coin's position
}Coin;

/**
 * @brief Struct for Enemy
 * 
 */
typedef struct Enemy{
    int frame_counter; ///<
    int xpos; ///<X coordinate of the enemy's position
    int ypos; ///<Y coordinate of the enemy's position
    Enemy_type type; ///<Type of the enemy
    int currentsprite; ///<The sprite currently in use
    xpm_image_t sprites[9]; ///<The sprites used to make the enemy
    bool dead; ///<Whether or not the enemy is dead (true if dead, false otherwise)
    bool atack; ///<If it's in attack mode
}Enemy;

/**
 * @brief Struct for Date
 * 
 */
typedef struct Date{
  uint32_t year; 
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} Date;

/**
 * @brief Struct for Highscore
 * 
 */
typedef struct Highscore{
    Date d; ///<Date the highscore was obtained
    int score; ///<The score obtained
}Highscore;

/**
 * @brief Struct for Background
 * 
 */
typedef struct Background{
    xpm_image_t sprite; ///<Sprite used to make the background
    int x; ///<X coordinate of the background's position
    int y; ///<Y coordinate of the background's position
    int speed; ///<Speed at which the background is moving
}Background;

/**
 * @brief Struct for Cursor
 * 
 */
typedef struct Cursor{
  int xpos; ///<X coordinate of the cursor's position
  int ypos; ///<Y coordinate of the cursor's position

  bool leftButtonPressed; 
  bool leftButtonReleased;
  LeftButtonState currentState; ///<Current state of the left button

  int currentSprite; ///<The sprite currently in use
  xpm_image_t m[2]; ///<The sprites used to make the cursor
}Cursor;

/**
 * @brief Struct for Tux
 * 
 */
typedef struct Tux {
    bool dead; ///<Whether or not the tux is dead
    int speed; ///<The speed of the tux
    int currentsprite; ///<The sprite currently in use
    xpm_image_t sprites[12]; ///<The sprites used to make the tux
    Moves currentmove; ///<The current movement type of the tux
    bool moving_left;                 //to know if tux is moving left while jumping
    bool moving_right;                       //to known if tux is moving right while jumping
    int xpos; ///<X coordinate of the tux's position
    int ypos; ///<Y coordinate of the tux's position
    bool active_fireball; ///<Whether or not ALT is pressed
    FireBall *fireball[30]; ///<Array of active fireballs
    int fireballs; ///<Size of the array that stores the active fireballs
} Tux;

/**
 * @brief Struct for Numeros
 * 
 */
typedef struct Numeros{
    xpm_image_t n[10]; ///<The sprites equivelent to all the existant digits
}Numeros;

/**
 * @brief Struct for Letras
 * 
 */
typedef struct Letras{
    xpm_image_t n[28]; ///<The sprites equivelent to all the existant letters
}Letras;

/**
 * @brief Struct for Game
 * 
 */
typedef struct Game{
    Background background[13]; ///<The sprites used to make the background of the game 
    State currentState; ///<The current state of the game
    Highscore highscores[5]; ///<Array of the highest scores
    int stage; ///<Stages from 0-5 that increment over time increasing the difficulty
    int score; ///<Stores the current score of the player
    int count_secs; ///<Counts the seconds the player has survided up till now
    Coin *coins[40]; ///<Array of ative coins
    Enemy *enemies[40]; ///<Array of active enemies
    int enemies_size; ///<Size of the array of enemies
    int coins_size; ///<Size of the array of coins
}Game;

/**
 * @brief Struct for Area_Collision.
 * Collision area is the area occupied by the object on the screen.
 * 
 */
typedef struct Area_Collision{
    int x0; ///<Leftmost X coordinate of the collision area
    int x1; ///<Rightmost X coordinate of the collision area
    int y0; ///<Uppermost Y coordinate of the collision area
    int y1; ///<Lowermost Y coordinate of the collision area
}Area_Collision;


