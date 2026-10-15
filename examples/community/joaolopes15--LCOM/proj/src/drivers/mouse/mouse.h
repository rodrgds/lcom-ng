#include <lcom/lcf.h>
#include "i8042.h"

extern uint8_t mouse_bytes[3];

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)(); 

void (packet_assembly)(void);

void (m_assemble_packet)(void);

int (m_write)(uint8_t command);
