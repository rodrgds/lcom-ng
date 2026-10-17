#ifndef _LCOM_SETUP_H_
#define _LCOM_SETUP_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>

typedef struct {
    int x, y;

} GamePos;
typedef enum {
  MENU,
  GAME,
  GAMEOVER,
  INSTRUCTIONS,
  EXIT,
  WINNER,
} state_game;
typedef enum {
  NO_BOMB,
  ONE_BOMB,
  EXPLODE,
  CLEAN,
} state_bomb;

/**
     * @brief  Launch the game map and use RTC to set the map depending on the time of day.
     *
     * @return Return 0 upon success and non-zero otherwise 
**/
int start_game();
/**
     * @brief  Launch the menu settings.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int start_menu();




#endif /* __TIMER_H */
