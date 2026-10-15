#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>

int (KBC_read_status)(uint8_t* status);
int (KBC_read_output)(uint8_t* output);
int (KBC_get_byte)(uint8_t* output);
int (KBC_write_command)(uint8_t port, uint8_t command);

#endif
