#include <lcom/lcf.h>

#include <stdint.h>

#include "mouse.h"

int mouse_hook_id = 2;

uint8_t byte_no = 0;
uint8_t byte;

struct packet mouse_packet;

int (mouse_subscribe_int)(uint32_t *bit_no)
{
    *bit_no = BIT(mouse_hook_id);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)()
{
    return sys_irqrmpolicy(&mouse_hook_id);
}

void(mouse_ih)()
{
    read_output(KBC_OUT_BUF, &byte, false);
}

void (sync_packet)(){
    if (byte_no  || (byte & BIT(3))) {
        parse_packet();
        byte_no++;
    }
}

void(parse_packet)()
{
    mouse_packet.bytes[byte_no] = byte;

    switch (byte_no)
    {
    case 0:
        mouse_packet.y_ov = byte & BIT(7);
        mouse_packet.x_ov = byte & BIT(6);
        mouse_packet.delta_y = (byte & BIT(5)) ? 0xFF00 : 0x0000;
        mouse_packet.delta_x = (byte & BIT(4)) ? 0xFF00 : 0x0000;
        mouse_packet.mb = byte & BIT(2);
        mouse_packet.rb = byte & BIT(1);
        mouse_packet.lb = byte & BIT(0);
        break;
    case 1:
        mouse_packet.delta_x |= byte;
        break;
    case 2:
        mouse_packet.delta_y |= byte;
        break;
    }
}

int (disable_data_reporting)()
{

    uint8_t cmd;
    int attempts = 10;

    while (attempts--)
    {

        if (write_command(KBC_IN_BUF, WRITE_BYTE_TO_MOUSE) )
            return 1;

        if (write_command(KBC_CMD_REG, DISABLE_DATA_REPORTING) )
            return 1;

        WAIT_KBC;

        if (util_sys_inb(KBC_OUT_BUF, &cmd) )
            return 1;

        if (cmd == MOUSE_ACK)
            return 0;
    }
    return 1;
}

int (enable_data_reporting)()
{

    uint8_t cmd;
    int attempts = 10;

    while (attempts--)
    {

        if (write_command(KBC_IN_BUF, WRITE_BYTE_TO_MOUSE) )
            return 1;

        if (write_command(KBC_CMD_REG, ENABLE_DATA_REPORTING) )
            return 1;

        WAIT_KBC;

        if (util_sys_inb(KBC_OUT_BUF, &cmd) )
            return 1;

        if (cmd == MOUSE_ACK)
            return 0;
    }
    return 1;
}
