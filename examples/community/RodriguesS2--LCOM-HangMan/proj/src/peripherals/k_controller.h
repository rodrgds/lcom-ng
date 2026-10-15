#ifndef K_CONTROLLER_H
#define K_CONTROLLER_H

#include <lcom/lcf.h>
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>

int (read_controller) (uint8_t port, uint8_t *data, uint8_t mouse);  

int (write_controller) (uint8_t port, uint8_t data);

#endif

