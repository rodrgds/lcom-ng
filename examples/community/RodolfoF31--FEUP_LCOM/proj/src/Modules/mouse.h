#ifndef _MOUSE_H
#define _MOUSE_H
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include "mouse_macro.h"

typedef enum { 
	INIT,
	DRAW_UP,
	VERTEX,
	DRAW_DOWN,
	DETECTED
} state_t;

typedef enum {
	RBDOWN,
	RBUP,
	LBDOWN,
	LBUP,
	MBDOWN,
	MBUP,
	MOVE,
	INVALID
} event_type_t;


typedef struct {
	event_type_t type;
	uint8_t moveX;
	uint8_t moveY;
	bool lbdown, rbdown, mbdown;
} mouse_event_t;


int (util_sys_inb)(int port, uint8_t *value);

int mouse_enable_info();

void first_byte(struct packet *pp, bool *x_negative, bool *y_negative);

void second_byte(struct packet *pp, bool *x_negative);

void third_byte(struct packet *pp, bool *x_negative);

int mouse_disable_info();

int write_to_mouse(uint8_t command);

int (mouse_unsubscribe_int)(void);

int (mouse_subscribe_int)(uint8_t *bit_no);


int mouse_check_pattern(mouse_event_t evt,uint8_t x_len, uint8_t tolerance);

void mouse_check_event(struct packet *pp, event_type_t *event);

#endif 
