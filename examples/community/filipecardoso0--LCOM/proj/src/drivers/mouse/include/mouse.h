#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>

#include "mouse_macros.h"
#include "kbc_macros.h"
#include "../../../utils/include/utils.h"
#include "../../../lib/model/include/sprite.h"
#include "../../../images/mouse_cursor.xpm"

typedef struct mouse_cursor{
    int x, y; 
    sprite_t* cursorsprite; 
} mouse_cursor_t; 

/**
 * @brief Subscribes mouse's interrupts.
 * 
 * @param bit_no Hook ID's value.
 * @return int 0 if succede, 1 otherwise.
 */
int mouse_subscribe(uint8_t* bit_no);

/**
 * @brief Unsubscribes mouse's interrupts.
 * 
 * @return int 
 */
int mouse_unsubscribe();

/**
 * @brief Handles mouse's interrupts.
 * 
 */
void (mouse_ih)(void);

/**
 * @brief Get the byte count object
 * 
 * @return int Number of bytes.
 */
int get_byte_count();

/**
 * @brief Returns mouse's packets.
 * 
 * @return struct packet 
 */
struct packet mouse_get_packet();
/**
 * @brief Checks the Mouse Input's buffer is active.\
 * 
 */
bool (mouse_check_IBF)(void);

/**
 * @brief 
 * 
 */
int (mouse_write_command)(uint32_t, uint8_t*);

/**
 * @brief Activates the data report by asserting the acknowledge byte.
 * 
 */
int (mouse_enables_data_reporting)(void);

/**
 * @brief Activates the data report by updating the acknowledge byte.
 * 
 */
int (mouse_disable_data_reporting)(void);

/**
 * @brief Resets the mouse's settings to the default ones.
 * 
 */
int (mouse_reset_to_default)(void);

/**
 * @brief Checks if the output buffer is active.
 * 
 */
int (mouse_check_OBF)(void);

/**
 * @brief Generates a new cursor.
 * 
 * @return mouse_cursor_t* Points to the cursor created.
 */
mouse_cursor_t* cursor_new(void); 

#endif
