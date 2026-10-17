#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

/**
 * Subscribes the mouse interruptions.
 * 
*/
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * Unsubscribes the mouse interruptions.
 * 
*/
int (mouse_unsubscribe_int)();

/**
 * Enable mouse data reporting.
 * 
*/
int (enable_data_reporting)();

/**
 * Disable mouse data reporting.
 * 
*/
int (disable_data_reporting)();

/**
 * Adds the received packet to the array of packets.
 * 
*/
void (mouse_int_handler)();

void (parse_mouse_packet)(struct packet* pp);

#endif
