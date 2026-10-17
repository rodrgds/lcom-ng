#include <lcom/lcf.h>
#include "grafics.h"
#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "files.h"
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#pragma once
#include "img_files/fundo.xpm"
#include "img_files/tux-stand.xpm"
#include "img_files/tux-walk-0.xpm"
#include "img_files/tux-walk-3.xpm"
#include "img_files/tux-walk-4.xpm"
#include "img_files/tux_jump.xpm"
#include "img_files/tux_duck.xpm"
#include "img_files/tux-standb.xpm"
#include "img_files/tux-walk-0b.xpm"
#include "img_files/tux-walk-3b.xpm"
#include "img_files/tux-walk-4b.xpm"
#include "img_files/tux_jumpb.xpm"
#include "img_files/tux_duckb.xpm"

#include "img_files/mousecursor.xpm"
#include "img_files/mousecursor-click.xpm"
#include "img_files/menu_inicial.xpm"
#include "img_files/menu_inicial_start_game.xpm"
#include "img_files/menu_inicial_instructions.xpm"
#include "img_files/menu_inicial_highscores.xpm"
#include "img_files/highscores.xpm"
#include "img_files/highscores_back.xpm"
#include "img_files/instructions.xpm"
#include "img_files/instructions_back.xpm"
#include "img_files/pausa.xpm"
#include "img_files/game_over.xpm"
#include "img_files/game_over_back.xpm"
#include "img_files/game_over_exit.xpm"
#include "game_macros.h"
#include "numbers_letters.h"
#include "jogo_funcs.h"
#include "game_structs.h"


/**
 * @brief Calls the game in itself.
 * 
 * @return 0 if successful
 */
int jogo();

/**
 * @brief Create a Game object
 * 
 * @return Game* - Game pointer to the game reated
 */
Game* createGame();

/**
 * @brief Create a Tux object
 * 
 * @return Tux* - Tux pointer to the tux created
 */
Tux* createTux();

/**
 * @brief Create a Cursor object
 * 
 * @return Cursor* - Cursor pointer to the cursor created
 */
Cursor* createCursor();


