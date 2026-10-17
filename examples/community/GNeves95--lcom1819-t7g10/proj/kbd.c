//#include <minix/sysutils.h>
#include <lcom/lcf.h>
#include "kbd.h"

#include "i8042.h"

int hook_kbc_id = 1;
int countCycle = 0;
//uint32_t data;

#ifdef LAB3
int cont_sys_inb(port_t port, uint32_t *byte)
{
    countCycle++;
    return sys_inb(port, byte);
}
#else
#define cont_sys_inb(port, byte) sys_inb(port, byte)
#endif

int(kbd_subscribe_int)(uint8_t *bit_no)
{
    *bit_no = hook_kbc_id;
    if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_kbc_id) != 0)
        return 1;
    return 0;
}

int(kbd_unsubscribe_int)(void)
{
    if (sys_irqrmpolicy(&hook_kbc_id) != 0)
        return 1;
    return 0;
}

void(kbc_ih)(void)
{
    uint32_t status;

    if (sys_inb(KBD_STATUS, &status) != 0)
    {
        printf("kbc_ih(23): Error reading from status register!\n");
        return;
    }

    if (((status & PAR_ERR) == 0) && ((status & TO_ERR) == 0))
    {
        if (sys_inb(KBD_CTRL, &kbc_code) != 0)
        {
            printf("kbc_ih(41): Error reading scan code!\n");
            return;
        }
    }
}

void (kbd_scan_loop)(void)
{
    uint32_t stat;
    do
    {
        sys_inb(KBD_STATUS, &stat); /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        tickdelay(micros_to_ticks(DELAY_US));
    } while ((stat & OBF) == 0);
    sys_inb(OUT_BUF, &(pret.kbc_code));
    pret.receive = true;
}
