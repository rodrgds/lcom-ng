#include <lcom/lcf.h>

#ifdef mouse_enable_data_reporting
  #undef mouse_enable_data_reporting
#endif
#ifdef mouse_disable_data_reporting
  #undef mouse_disable_data_reporting
#endif

#define mouse_enable_data_reporting()  mouse_send_cmd_enable()
#define mouse_disable_data_reporting() mouse_send_cmd_disable()

#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    INIT,
    DRAW_UP,
    VERTEX,     
    DRAW_DOWN,  
    COMPLETE
  } gesture_state_t;
  

int mouse_send_cmd_enable(void);
int mouse_send_cmd_disable(void);
int  mouse_subscribe_int(uint8_t *bit_no);
int  mouse_unsubscribe_int(void);
void (mouse_ih)(void);
int  mouse_parse_packet(struct packet *pp, uint8_t bytes[3]);
gesture_state_t update_gesture_state(gesture_state_t state, struct packet *pkt, int16_t *dx_total, int16_t *dy_total, uint8_t x_len, uint8_t tolerance);


extern uint8_t       pkt_bytes[3];
extern uint8_t       pkt_index;
extern struct packet pkt;

#endif 
