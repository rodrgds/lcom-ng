#include "kbc.h"
#include "i8042.h"
#include <minix/drivers.h>

int kbc_write(unsigned char cmd, unsigned long port) {
        unsigned long stat;
        while (1) {
                if (sys_inb(STAT_REG, &stat) != OK)
                        return -1;
                if ((stat & IBF) == 0) {
                        if (sys_outb(port, cmd) != OK)
                                return -1;
                        break;
                }
                tickdelay(micros_to_ticks(DELAY_US));
        }
        return 0;
}

int kbc_read() {
        unsigned long stat, data;
        int kbc_tries = 0;
        while (kbc_tries < KBC_IO_MAX_TRIES) {
                sys_inb(STAT_REG, &stat); /* assuming it returns OK */
                /* loop while 8042 output buffer is empty */
                if (stat & OBF) {
                        sys_inb(OUT_BUF, &data); /* assuming it returns OK */
                        if ((stat & (PAR_ERR | TO_ERR)) == 0)
                                return data;
                        else if (data == ERROR)
                                return data;
                        else if (data == RESEND)
                                return data;
                        else
                                return -1;
                }
                tickdelay(micros_to_ticks(DELAY_US));
                kbc_tries++;
        }
        return -1;
}
