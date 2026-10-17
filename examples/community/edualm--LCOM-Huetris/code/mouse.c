#include <stdio.h>
#include <minix/syslib.h>

#include "mouse.h"
#include "kbc.h"

int mouse_read(unsigned long *value) {
    return sys_inb(KBC_REG_DATA, value);
}

int mouse_enable() {
    int reply;
    
    if ((reply = keyboard_write(KBC_REG_STATUS, MOUSE_WRITE_BYTE)))
        return reply;
    
    if ((reply = keyboard_write(KBC_REG_DATA, MOUSE_ENABLE)))
        return reply;

    if (keyboard_read() != 0xFA)
        return 1;
    return 0;
}
