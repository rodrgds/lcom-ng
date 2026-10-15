#ifndef _MOUSE_H_
#define _MOUSE_H_
#include <lcom/lcf.h>
#include "../i8042.h"
#include "../k_controller.h"
#include "../../game/menu.h"



int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
void (buffer_mouse_bytes)();
void (process_mouse_bytes)();
int(mouse_write_cmd)(uint8_t cmd);
int init_mouse_pointer();
void save_background(int x, int y);
void restore_background(int x, int y);
void update_mouse_position();
void draw_mouse_pointer_to_back_buffer();
void draw_mouse_pointer();
void process_mouse_clicks();

#endif



