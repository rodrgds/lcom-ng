#include <lcom/lcf.h>

#include "kbc.h"
#include "i8042.h"

int cnt = 0;

int (read_status)(uint8_t *st) {
    if (util_sys_inb(KBC_ST_REG, st) != 0) return 1;
    cnt++;
    return 0;
}

int (write_command)(uint8_t port, uint8_t cmd) {
    uint8_t st;
    int attempts = 10;
    while(attempts > 0) {
        if (read_status(&st) != 0) return 1; 
        if(!(st & KBC_IBF)) {
            if (sys_outb(port, cmd) != 0) return 1;
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 0;
}

int (read_output)(uint8_t port, uint8_t *content) {
    uint8_t stat;
    int attempts = 10;
    while(attempts) {
        if (read_status(&stat) != 0) return 1; 
        if((stat & KBC_OBF) != 0) {
            if (util_sys_inb(port, content) != 0) return 1; 
            cnt++;
            if (stat & (KBC_PARITY | KBC_TIMEOUT)) return 1;
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(DELAY_US)); 
    }
    return 1;
}
