 #pragma once
 
 typedef enum {
 	INIT,
 	DRAWUP,
 	VERTEX,
 	DRAWDOWN,
 	CMP
 } state_t;

 void (mouse_ih)(void);

 int (mouse_subscribe_int)(int *irq_set);

 int (mouse_unsubscribe_int)();

 int (setMouseIRQ)(bool enable);

 int (mouse_write_command)(uint8_t cmd);

 void (mouse_read_out_buff)(void);

 int (createMousePacket)(struct packet *pckt, uint8_t data3byte[]);

 int (set_kbc_cmd_byte_dflt)();

 void (state_machine)(struct packet *pckt, uint8_t x_len, uint8_t tolerance);
