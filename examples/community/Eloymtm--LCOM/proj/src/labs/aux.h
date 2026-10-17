#ifndef __AUX_H
#define __AUX_H

#include <lcom/lcf.h>

int (read_status)(uint8_t *status);
int (read_output_buffer)(uint8_t *scancode,bool IsMouse);
int (write_command)(uint8_t port, uint8_t commandByte);

#endif
