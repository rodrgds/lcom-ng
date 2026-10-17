#ifndef _PS2_H
#define _PS2_H

typedef struct packetData {
	uint8_t bytes[3];
	bool rb;
	bool mb;
	bool lb;
	int16_t delta_x;
	int16_t delta_y;
	bool x_ov;
	bool y_ov;
	bool x_sign;
	bool y_sign;
} packetData;

int (init_mouse)(uint8_t *bit_no);

int (ps2_subscribe_int)(uint8_t *bit_no);

int(ps2_unsubscribe_int)(void);

int (ps2_ignore)(void);

int (mouse_set_stream_mode)(void);

int (mouse_set_remote_mode)(void);

int (mouse_write)(void);

int (mouse_send_command)(unsigned long cmd);

int (mouse_send_argument)(unsigned long cmd);

int (mouse_cmd_receive)(void);

packetData *paux;

int counter;

#endif
