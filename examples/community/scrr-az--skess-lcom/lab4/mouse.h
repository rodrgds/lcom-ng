#include <lcom/lcf.h>
#include "KBC.h"

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
int (mouse_write_command)(uint8_t command);
void (mouse_ih)();
