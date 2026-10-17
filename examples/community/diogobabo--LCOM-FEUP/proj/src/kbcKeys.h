#ifndef _KBC_KEYS_H
#define _KBC_KEYS_H

/*
 * enum KEY
 * @brief used for the different keys of the keyboard
 */
enum KEY{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  ENTER,
  ESC,
};

/*
 * enum STATE
 * @brief events of the game, allows to easily switch between states
 */
enum STATE{
  PAUSE,
  MENU,
  PLAY_SOLO,
  PLAY_MULTIPLAYER,
  DEAD,
  EXIT,
  LEADERBOARD,
};

/*
 * struct: mouseInfo
 * @brief used to save the mouse information
 * @param rb
 * @param mb
 * @param lb
 * @param delta_x
 * @param delta_y
 */
typedef struct mouseInfo_t
{
  bool rb, mb, lb; 
  int16_t delta_x;  
  int16_t delta_y;
} mouseInfo;

/*
 * struct: Character
 * @brief used to save the information of a font char
 * @param x
 * @param y
 * @param letter
 */
typedef struct character_t {
  int x;
  int y;
  int letter;
}Character;

#endif
