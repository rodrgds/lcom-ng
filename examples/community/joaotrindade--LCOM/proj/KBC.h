#ifndef TEST4_H
#define TEST4_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#define KB_IRQ 0x01
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUT 0x60
#define BIT(n) (0x01<<n)
#define OBF BIT(0)
#define OUT_BUF 0x60
#define WAIT_KBC 20000
#define TO_ERR BIT(6)
#define PAR_ERR BIT(7)

int test_scan(void);
int test_leds(unsigned short n, unsigned short *leds);


#endif
