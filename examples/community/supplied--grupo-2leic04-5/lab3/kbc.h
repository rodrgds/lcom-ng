#ifndef GRUPO_2LEIC04_5_KBC_H
#define GRUPO_2LEIC04_5_KBC_H
#include <lcom/lcf.h>
int kbd_subscribe_int(uint8_t *bit_no);
int kbd_unsubscribe_int(void);
bool kbc_has_error(void);
uint8_t kbc_get_scancode(void);
#endif
