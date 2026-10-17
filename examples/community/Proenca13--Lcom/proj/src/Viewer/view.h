//
// Created by joao on 24-04-2024.
//

#ifndef SHARED_VIEW_H
#define SHARED_VIEW_H
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "Controller/Timer/timer.h"
#include "Controller/Timer/i8254.h"
#include "Controller/Video/graphic.h"
#include "Controller/Keyboard/keyboard.h"
#include "Controller/Mouse/mouse.h"
#include "Controller/RTC//rtc.h"
#include "Model/model.h"
#include "Model/game.h"
#include "configs.h"
#include "State/state.h"
#include "Model/xpm/Start.xpm"
#include "Model/xpm/Start2.xpm"
#include "Model/xpm/mouse.xpm"
#include "Model/xpm/Controls.xpm"
#include "Model/xpm/Controls2.xpm"
#include "Model/xpm/Exit.xpm"
#include "Model/xpm/Exit2.xpm"
#include "Model/xpm/Title.xpm"
#include "Model/xpm/dirt.xpm"
#include "Model/xpm/arrows.xpm"
#include "Model/xpm/DaySky.xpm"
#include "Model/xpm/NightSky.xpm"
#include "Model/xpm/AfternoonSky.xpm"
#include "Model/xpm/dyog.xpm"
#include "Model/xpm/dyog2.xpm"
#include "Model/xpm/Continue.xpm"
#include "Model/xpm/Continue2.xpm"
#include "Model/xpm/paused.xpm"
#include "Model/xpm/controls_title.xpm"
#include "Model/xpm/F.xpm"
#include "Model/xpm/ESC.xpm"
#include "Model/xpm/RightClick.xpm"
#include "Model/xpm/LeftClick.xpm"
#include "Model/xpm/space.xpm"
#include "Model/xpm/grass.xpm"
#include "Model/xpm/selectedgrass.xpm"
#include "Model/xpm/cobblestone.xpm"
#include "Model/xpm/cobblestone2.xpm"
#include "Model/xpm/tnt.xpm"
#include "Model/xpm/tile1.xpm"
#include "Model/xpm/tile1selected.xpm"
#include "Model/xpm/tile2.xpm"
#include "Model/xpm/tile2selected.xpm"
#include "Model/xpm/tile3.xpm"
#include "Model/xpm/tile3selected.xpm"
#include "Model/xpm/tile4.xpm"
#include "Model/xpm/tile4selected.xpm"
#include "Model/xpm/tile5.xpm"
#include "Model/xpm/tile5selected.xpm"
#include "Model/xpm/tile6.xpm"
#include "Model/xpm/tile6selected.xpm"
#include "Model/xpm/tile7.xpm"
#include "Model/xpm/tile7selected.xpm"
#include "Model/xpm/tile8.xpm"
#include "Model/xpm/tile8selected.xpm"
#include "Model/xpm/flag.xpm"
#include "Model/xpm/flagselected.xpm"
#include "Model/xpm/GameOver.xpm"
#include "Model/xpm/PlayAgain.xpm"
#include "Model/xpm/PlayAgain2.xpm"
#include "Model/xpm/Restart.xpm"
#include "Model/xpm/Restart2.xpm"
#include "Model/xpm/YouWon.xpm"
#include "Model/xpm/kingcharles.xpm"
#include "Model/xpm/Counter0.xpm"
#include "Model/xpm/Counter1.xpm"
#include "Model/xpm/Counter2.xpm"
#include "Model/xpm/Counter3.xpm"
#include "Model/xpm/Counter4.xpm"
#include "Model/xpm/Counter5.xpm"
#include "Model/xpm/Counter6.xpm"
#include "Model/xpm/Counter7.xpm"
#include "Model/xpm/Counter8.xpm"
#include "Model/xpm/Counter9.xpm"
#include "Model/xpm/FlagCounter.xpm"

/**
 * @brief Draws the current game state.
 * 
 * Calls specific drawing functions based on the current game and menu state.
 */

void draw_state();

/**
 * @brief Draws the main menu.
 * 
 * Draws the main menu interface with buttons for starting the game, viewing controls, and exiting.
 */

void draw_main_menu();

/**
 * @brief Draws the game pause menu.
 * 
 * Draws the game menu interface which appears during gameplay pause.
 * Provides options to continue playing, restart the game, or exit to the main menu.
 */

void draw_game_menu();

/**
 * @brief Draws the mouse cursor.
 * 
 * Draws the mouse cursor sprite at the current mouse coordinates.
 */

void draw_mouse();

/**
 * @brief Draws the game interface.
 * 
 * Draws the game interface including the game timer, game grid, and flag counter.
 */

void draw_game();

/**
 * @brief Draws the controls menu.
 * 
 * Draws the controls menu interface, providing information about game controls and an option to exit.
 */

void draw_controls_menu();

/**
 * @brief Draws the game over menu.
 * 
 * Draws the game over menu interface, displaying the game over message and options to play again or exit.
 */

void draw_game_over_menu();

/**
 * @brief Draws the win menu.
 * 
 * Draws the win menu interface, displaying the victory message, game time, and options to play again or exit.
 */

void draw_win_menu();

/**
 * @brief Determines the appropriate sky sprite based on the current time.
 * 
 * @return Returns a pointer to the appropriate sky sprite based on the time of day.
 */

Sprite *check_time_sprite();

/**
 * @brief Determines the appropriate counter sprite based on the provided digit.
 * 
 * @param timer2 The digit for which to determine the appropriate counter sprite.
 * @return Returns a pointer to the appropriate counter sprite.
 */

Sprite *check_time_sprite2(uint8_t timer2);

/**
 * @brief Draws the flag counter sprite based on the current count of flagged cells.
 * 
 * Draws the appropriate counter sprite to represent the number of flagged cells.
 */

void draw_counter_sprite();

/**
 * @brief Draws a sprite at the specified coordinates.
 * 
 * @param sprite Pointer to the sprite to be drawn.
 * @param x The x-coordinate of the top-left corner of the sprite.
 * @param y The y-coordinate of the top-left corner of the sprite.
 * @return Returns 0 on success, non-zero otherwise.
 */

int draw_sprite(Sprite *sprite, int x, int y);
#endif //SHARED_VIEW_H
