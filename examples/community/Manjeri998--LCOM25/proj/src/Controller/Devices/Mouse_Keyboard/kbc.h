#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../macros.h"
#include "../func_aux.h"

int (read_command_byte)(uint8_t* command);
int (write_command_byte)(uint8_t port, uint8_t command);
int (mouse_read_command_byte)(uint8_t* response);

#endif 
