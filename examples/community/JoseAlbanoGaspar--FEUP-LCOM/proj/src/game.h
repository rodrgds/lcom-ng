#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "macros/constants.h"
#include "assist.h"
#include "snake.h"
#include "pause.h"

/**
 * @brief creates all the game
 * @param irq_set_keyboard irq line of keyboard
 * @param irq_set_mouse irq line of mouse
 * @param irq_set_timer irq line of timer
 * @param vg_mode current video mode
 * @return return 0 if success, non 0 otherwise
 */
int (game_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer, uint16_t vg_mode);
/**
 * @brief initializes game content
 * @return return 0 if success, non 0 otherwise
 */
int (start_game)();
/**
 * @brief draw the elements of the game
 */
void (drawGame)();


