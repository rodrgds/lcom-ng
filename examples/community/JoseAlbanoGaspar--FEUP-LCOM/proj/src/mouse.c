#include "mouse.h"

uint8_t mouse_status;
int bit_no_global_mouse;
int hook_id_mouse = MOUSE_IRQ;
uint8_t packet_byte;
struct packet mouse_packet;
int mouseX = 2000;
int mouseY = 2000;
int lastMouseX = 2000;
int lastMouseY = 2000;
int mouseCount = 0;

int (mouse_subscribe_int)(uint8_t *bit_no){
    int aux = (int)*bit_no;
    
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&aux))
        return 1;

    *bit_no = (uint8_t)aux;
    return 0;
}

int (mouse_unsubscribe_int)(){
    //Unsubscribing the interruptions
    
    if(sys_irqrmpolicy(&hook_id_mouse))
        return 1;

    return 0;
}

int (mouse_get_status)()
{
    if (util_sys_inb(MOUSE_STATUS_REG, &mouse_status))
    {
        return 0;
    }
    return 1;
}

int (mouse_check_status)()
{
    if ((mouse_status & MOUSE_OUTB) != 0)  //check if buffer has data to read
    {
        if ((mouse_status & (MOUSE_PARE | MOUSE_TIMEO)) == 0) //check if there are errors
        {
            if ((mouse_status & MOUSE_AUXB) != 0)  //check if the data is for the mouse 
            {
                //printf("valid status\n");
                return 0;
            }
        }
    }
    //printf("Invalid Status\n");
    return 1;
}

void parse()
{
    /* Preencher Struct mouse_packet definida em lab4.c */
    switch (mouseCount)
    {
    case 0:
        memset(mouse_packet.bytes, 0, sizeof(mouse_packet.bytes));
        mouse_packet.bytes[0] = packet_byte;
        //printf("packet_byte 1: 0x%x", packet_byte);
        mouse_packet.lb = (packet_byte & MOUSE_LB_PRESSED);
        mouse_packet.rb = (packet_byte & MOUSE_RB_PRESSED);
        mouse_packet.mb = (packet_byte & MOUSE_MB_PRESSED);
        mouse_packet.y_ov = (packet_byte & MOUSE_Y_OVF);
        mouse_packet.x_ov = (packet_byte & MOUSE_X_OVF);
        break;
    case 1:
        mouse_packet.bytes[1] = packet_byte;
        //printf("packet_byte 2: 0x%x", packet_byte);
        if ((mouse_packet.bytes[0] & BIT(4)))
        {
            //printf("deltaX\n");
            //uint16_t temp = 0xFF00 | packet_byte;
            uint16_t temp = 0xFF00 | packet_byte;
            mouse_packet.delta_x = temp;
        }
        else
            mouse_packet.delta_x = (uint16_t) packet_byte;
        break;
    case 2:
        mouse_packet.bytes[2] = packet_byte;
        //printf("packet_byte 3: 0x%x", packet_byte);
        if ((mouse_packet.bytes[0] & BIT(5)))
        {
            //printf("deltaY\n");
            uint16_t temp = 0xFF00 | packet_byte;
            mouse_packet.delta_y = temp;
        }
        else
            mouse_packet.delta_y = (uint16_t) packet_byte;
        break;
    }
}

void(mouse_ih)()
{
    //printf("Mouse_ih()\n");
    if (mouse_get_status() == OK)
    {
        //printf("Got Status\n");
        if (mouse_check_status() == OK)
        {
            if (util_sys_inb(MOUSE_OUT_BUF, &packet_byte))
            {
                //printf("parsing...\n");
                parse();
            }
        }
        else {
            mouseCount--; //ensures the count mantains if the status is invalid
        }
    }
}


int (mouse_reset)(){
    uint8_t ack;
    if (sys_outb(MOUSE_STATUS_REG, MOUSE_RESET) != OK) return 1;
    if (util_sys_inb(MOUSE_OUT_BUF, &ack) == 1) return 1;
    else {
        if (ack == MOUSE_OK) return 0;
        else if (ack == MOUSE_NACK) //If a NACK 0xFE is received, the command should be retried from the start
          return mouse_reset();
        else if (ack == MOUSE_ACK_ERROR) return 1;
    }
    printf("Error value: %x\n", ack);
    return 1;
}

int (mouse_en_data_report)(){
  uint8_t ack;
  do {
    util_sys_inb(MOUSE_STATUS_REG, &mouse_status);
    if (sys_outb(MOUSE_STATUS_REG, MOUSE_CMD) != OK)
      return 1;

    util_sys_inb(MOUSE_STATUS_REG, &mouse_status);
    if (sys_outb(MOUSE_OUT_BUF, MOUSE_ENABLE_CMD) != OK)
      return 1;

    util_sys_inb(MOUSE_STATUS_REG, &mouse_status);
    util_sys_inb(MOUSE_OUT_BUF, &ack);

    if (ack == MOUSE_ACK_ERROR) return 1;

  } while (ack == MOUSE_NACK);

  return 0;
}

int (mouse_dis_data_report)(){
  uint8_t ack;
  do {
    util_sys_inb(MOUSE_STATUS_REG, &mouse_status);
    if (sys_outb(MOUSE_STATUS_REG, MOUSE_CMD) != OK)
      return 1;

    util_sys_inb(MOUSE_STATUS_REG, &mouse_status);
    if (sys_outb(MOUSE_OUT_BUF, MOUSE_DISABLE_CMD) != OK)
      return 1;

    util_sys_inb(MOUSE_STATUS_REG, &mouse_status);
    util_sys_inb(MOUSE_OUT_BUF, &ack);

    if (ack == MOUSE_ACK_ERROR) return 1;

  } while (ack == MOUSE_NACK);

  return 0;
}

int (mouse_is_valid_first_line_mov)(int tolerance){
    if(((mouse_packet.delta_x > 1 && mouse_packet.delta_y > 1) ||   //the slope must be larger than 1
        ( mouse_packet.delta_x <= 0 && abs(mouse_packet.delta_x) < tolerance)  ||  //check tolerance in x
        ( mouse_packet.delta_y <= 1 && abs(mouse_packet.delta_y) < tolerance)) &&  //check tolerance in y
       mouse_packet.lb && !mouse_packet.mb && !mouse_packet.rb)

       return 1;
    return 0;

}

int (mouse_is_valid_second_line_mov)(int tolerance){
    if(((mouse_packet.delta_x > 1 && mouse_packet.delta_y < -1) ||
      (mouse_packet.delta_x <= 0 && abs(mouse_packet.delta_x) < tolerance) ||
      (mouse_packet.delta_y >= -1 && abs(mouse_packet.delta_y) < tolerance)) &&
       !mouse_packet.lb && mouse_packet.mb && !mouse_packet.rb)

       return 1;
    return 0;
}
