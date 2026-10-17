#include <lcom/lcf.h>
#include <stdlib.h>
#include "utils.h"
#include "graphics.h"
#include "sprites.h"
#include "i8042.h"

/** @defgroup Game Game
 * @{
 *
 * Contais tha code for the game screen
 */

void game_init();

void game_frame();

void game_mouse(struct packet pckt);

void game_exit();

void game_keyboard(uint8_t data);

void addScore();

