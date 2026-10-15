#include "mouse.h"


int mouse_hook_id = 2; 

struct packet mouse_packet;
uint8_t byte_index = 0;
uint8_t current_byte = 0;
uint8_t mouse_bytes[3];



int (mouse_subscribe_int)(uint32_t *irq_set) {
    if (irq_set == NULL) return 1;
    *irq_set = BIT(mouse_hook_id); 
    if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &mouse_hook_id) != 0) return 1;
    return 0;
}

int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
    return 0;
}





/*
-> summary of actions for sending cmd to mouse
-> see https://pages.up.pt/~up722898/aulas/lcom2425/lab4/lab4_3.html

1) request fowarding of byte (cmd) to the mouse -> write KBC_WRITE_MOUSE (0xD4) to port KBC_CMD_REG (0x64)
2) write command (one of these below) to KBC_ARGS (0x60)
3) read acknowledgment byte received from the mouse from OUT_BUF (0x60)   (if success, the ack byte is MOUSE_OK (0xFA), if error 0xFE)
4) if cmd has arguments, we repeat the 3 previous steps for each byte of the argument(s)  (if 0xFE earlier, we resend the entire cmd from the beggining, not only the last byte)
---- mouse executes cmd ----
5) if cmd has response, we read it from OUT_BUF (0x60) 
*/
int (mouse_write_cmd)(uint8_t command) {
    
    int timeout = 0;
    uint8_t mouse_response;

    while (timeout < 10) {
        if (write_KBC_cmd(KBC_CMD_REG, KBC_WRITE_MOUSE) != 0) {
            printf("Error writing to KBC_CMD_REG.\n");
            return 1;}
        if (write_KBC_cmd(KBC_ARGS, command) != 0) {
            printf("Error writing to KBC_ARGS.\n");
            return 1;}
        
        tickdelay(micros_to_ticks(10000));
        

        if (util_sys_inb(OUT_BUF, &mouse_response) != 0) {
            printf("Error reading from OUT_BUF.\n");
            return 1;}

        if (mouse_response == MOUSE_ACK) return 0; 
        timeout++;
    }

    return 1; 
}


int (mouse_set_stream_mode)() {
    if (mouse_write_cmd(MOUSE_SET_STREAM) != 0) {
        printf("Error enabling data reporting.\n");
        return 1;
    }
    return 0;
}



int (_mouse_enable_data_reporting)() {
    if (mouse_write_cmd(MOUSE_ENABLE) != 0) {
        printf("Error enabling data reporting.\n");
        return 1;
    }
    return 0;
}

int (_mouse_disable_data_reporting)() {
    if (mouse_write_cmd(MOUSE_DISABLE) != 0) {
        printf("Error disabling data reporting.\n");
        return 1;
    }
    return 0;
}




void (mouse_ih)() {
    uint8_t st;

    if (util_sys_inb(STATUS_REG, &st) != 0) {
        printf("Error reading status.\n");
        return;
    }
    //tickdelay(micros_to_ticks(DELAY_US));

    //if the data is from mouse (and there's no errors), we read a byte from the OUT_BUF 
    if ((st & OBF) && (st & AUX) && !(st & (PARITY | TIMEOUT))) {
        if (util_sys_inb(OUT_BUF, &current_byte) != 0) {
            printf("Error reading from OUT_BUF.\n");
            return;
        }
    }    
}



/*check for sync:
imagine we are processing the 1st byte of the packet (index=0)
the 1st byte of the packet is the control byte
a valid control byte must have its 3 bit set 
that's what we're checking here 

besides, we keep the data in a array that we will later use to create the packet
and we check if the packet is complete
if it is, we return true
if it is not, we return false
*/
bool (mouse_sync_bytes)() {

    if (byte_index == 0 && !(current_byte & IS_FIRST_BYTE)) {
        //printf("Invalid byte!\n");
        return false;
    }

    if ((byte_index == 0 && (current_byte & IS_FIRST_BYTE)) || (byte_index > 0 && byte_index < 3)) {
        mouse_bytes[byte_index] = current_byte;
        byte_index++;
    }

    if (byte_index == 3) {
        byte_index = 0; //we reset index for the next packet
        return true; //we've completed the packet
    }

    return false; //we did not complete the packet yet
}



void (create_packet)() {

    for (int i = 0; i < 3; i++) {
        mouse_packet.bytes[i] = mouse_bytes[i];
    }

    mouse_packet.lb = mouse_bytes[0] & MOUSE_LB;
    mouse_packet.rb = mouse_bytes[0] & MOUSE_RB;
    mouse_packet.mb = mouse_bytes[0] & MOUSE_MB;
    mouse_packet.delta_x = (mouse_bytes[0] & MOUSE_MSB_X_DELTA) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1];
    mouse_packet.delta_y = (mouse_bytes[0] & MOUSE_MSB_Y_DELTA) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2];
    mouse_packet.x_ov = mouse_bytes[0] & MOUSE_X_OVERFLOW;
    mouse_packet.y_ov = mouse_bytes[0] & MOUSE_Y_OVERFLOW;
}


