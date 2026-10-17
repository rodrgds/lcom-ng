#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

int mouse_hook_id = 12;
extern uint32_t no_sysinb;

int(MC_subscribe_int)(uint8_t *bit_no)
{
    mouse_hook_id = *bit_no;
    if (bit_no == NULL)
        return 1;
    if (sys_irqsetpolicy(MC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id))
        return 1;
    return 0;
}

int(MC_unsubscribe_int)()
{
    return sys_irqrmpolicy(&mouse_hook_id);
}

void(MC_int_handler)(uint8_t *data)
{
    readOutBuffRegister(data);
}

int(readOutBuffRegister)(uint8_t *read)
{
    uint8_t status;
    uint8_t notimportant;
    if (util_sys_inb(statReg, &status))
    {
        return 1;
    }
    if ((status & BIT(6)) == 0 && (status & BIT(0)) && (status & BIT(5)))
    {
        util_sys_inb(in_outBuff, read);
        return 0;
    }
    util_sys_inb(in_outBuff, &notimportant);
    return 1;
}

int(renableMouse_int)()
{
    uint8_t status;
    if (sys_outb(statReg, ReadCommandByte))
        return 1;
    if (util_sys_inb(in_outBuff, &status))
        return 1;
    status |= BIT(1);
    if (sys_outb(statReg, WriteCommandByte))
        return 1;
    if (sys_outb(in_outBuff, status))
        return 1;
    return 0;
}


void(Write_to_struct_packet)(uint8_t bytes[3], struct packet *pp)
{
    for (int i = 0; i < 3; i++)
    {
        pp->bytes[i] = bytes[i];
    }
    pp->x_ov = bytes[0] & BIT(6);
    pp->y_ov = bytes[0] & BIT(7);
    pp->rb = bytes[0] & BIT(1);
    pp->lb = bytes[0] & BIT(0);
    pp->mb = bytes[0] & BIT(2);
    pp->delta_x = (bytes[0] & BIT(4)) ? (int16_t)(bytes[1] | 0xFF00) : (int16_t)bytes[1];
    pp->delta_y = (bytes[0] & BIT(5)) ? (int16_t)(bytes[2] | 0xFF00) : (int16_t)bytes[2];
}

int(mouse_enable_data)(void)
{
    int retries = 10;
    uint8_t read, status;
    while (retries > 0)
    {
        if (sys_outb(statReg, WriteByteToMouse))
            return 1;
        if (util_sys_inb(statReg, &status))
            return 1;
        if ((status & BIT(1)) == 0)
        {
            if (sys_outb(in_outBuff, Enable))
                return 1;
        }

        if (util_sys_inb(in_outBuff, &read))
            return 1;
        switch (read)
        {
        case Nack_invalid:
            retries--;
            break;
        case Error:
            return 1;
        case Ack_ok:
            return 0;
            break;
        default:
            return 1;
            break;
        }
    }
    return 1;
}

int(mouse_disable_data_reporting)(void)
{
    int retries = 10;
    uint8_t read, status;
    while (retries > 0)
    {
        if (sys_outb(statReg, WriteByteToMouse))
            return 1;
        if (util_sys_inb(statReg, &status))
            return 1;
        if ((status & BIT(1)) == 0)
        {
            if (sys_outb(in_outBuff, Disable))
                return 1;
        }

        if (util_sys_inb(in_outBuff, &read))
            return 1;
        switch (read)
        {
        case Nack_invalid:
            retries--;
            break;
        case Error:
            return 1;
        case Ack_ok:
            return 0;
            break;
        default:
            return 1;
            break;
        }
    }
    return 1;
}
void checkifMouseClickedaButton(struct Game *game){
    if(game->signal.mouse_pressed &&game->mouseC.mouse_x<664 && game->mouseC.mouse_x>349 && game->mouseC.mouse_y>393 &&game->mouseC.mouse_y<492){
        game->state=1;
        clean_screen();
    }
  }
void updateMouse(struct Game *game, int16_t dy, int16_t dx){
    game->oldMouseC.mouse_y = game->mouseC.mouse_y;
    game->oldMouseC.mouse_x = game->mouseC.mouse_x;

    int newx=game->mouseC.mouse_x+dx;
    int newy=game->mouseC.mouse_y-dy;
    if(newx<0){
        newx=0;
    }
    if(newx>x_axis-24){
        newx=x_axis-24;
    }
    if(newy<0){
        newy=0;
    }
    if(newy>y_axis-50){
        newy=y_axis-50;
    }
    game->mouseC.mouse_x=newx;
    game->mouseC.mouse_y=newy;
    checkifMouseClickedaButton(game);
  }


