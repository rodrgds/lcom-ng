#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe.h"
#include "assist.h"
#include "sprite.h"
#include "macros/constants.h"

/**
 * @brief creates all the menu
 * @param irq_set_keyboard irq line of keyboard
 * @param irq_set_mouse irq line of mouse
 * @param irq_set_timer irq line of timer
 * @return Returns 0 upon success and non-zero otherwise
 */
int (menu_loop)(uint32_t irq_set_keyboard, uint32_t irq_set_mouse, uint32_t irq_set_timer);
/**
 * @brief initializes menu screen
 * @return Returns 0 upon success and non-zero otherwise
 */
int (init_menu)();
/**
 * @brief updates menu scrren
 * @param sel selected option
 */
int (update_menu)(int sel);
/**
 * @brief detects the option that is selected and changes it if its necessary
 * @param scancode scancode of the key that was pressed
 * @param sel option that is selected
 * @return the sel to be highlighted
 */
int (selectedOpt)(uint16_t scancode,int sel);
/**
 * @brief detects if we click in an option
 * @return returns 0 if we do not click, non 0 otherwise
 */
int (onPress)();

