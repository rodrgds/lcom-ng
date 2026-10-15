#ifndef __kbdhelper_h
#define __kbdhelper_h


#include <lcom/lcf.h>
#include "i8042.h"

int (read_KBC_status)(uint8_t* status);

int (read_KBC_output)(uint8_t port, uint8_t *output, bool isMouse);

int (write_KBC_command)(uint8_t port, uint8_t commandByte);


#endif
