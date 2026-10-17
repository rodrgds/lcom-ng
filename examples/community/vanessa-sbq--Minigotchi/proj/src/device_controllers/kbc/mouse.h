#include "../utils.h"
#include "i8042.h"

#include <lcom/lcf.h>
#include "kbc.h"

// "Getter" functions
uint8_t mouse_get_readByte();
bool mouse_get_error();
void mouse_set_error(bool er);

// Functions that help with the mouse setup.
int (mouse_subscribe_int)(uint8_t* bit_no);
int (mouse_unsubscribe_int)();
int send_byte_to_mouse(uint8_t commandToSend);

// Interrupt handler for the mouse.
void (mouse_ih)();
