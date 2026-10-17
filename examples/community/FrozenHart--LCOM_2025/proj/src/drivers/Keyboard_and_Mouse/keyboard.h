#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "src/drivers/i8042.h"
#include "KBC.h"

#define MAX_SCANCODE 0xFF

int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
void (keyboard_ih)();
int (keyboard_get_key)(key_identifier_t *key);
void (keyboard_init_scancode_map)();
// int (kbc_check_config)(uint8_t *st);
// int (kbc_read_data)(uint8_t port, uint8_t *data);
// int(kbc_write_command)(kbc_commands_t command, uint8_t *command_byte);
// int(kbc_command_byte_IO)(kbc_command_byte_components_t components[], size_t size);

#endif // KEYBOARD_H
