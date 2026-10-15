#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <stdbool.h>
#include <stdint.h> 
#include <stddef.h> 


void pause_menu_assets_init(void);
void pause_menu_assets_destroy(void);
void pause_menu_draw(void);
void pause_menu_handle_input(uint8_t *scancode, size_t size);

void pause_menu_set_active(bool active);
bool pause_menu_is_active(void);

#endif
