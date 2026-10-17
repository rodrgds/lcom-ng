#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "i8042.h"


unsigned int playtime;

int subscribe_int(void );
int unsubscribe_int();
long read_KBC_data();
int interrupt();
int receiveKey();


#endif
