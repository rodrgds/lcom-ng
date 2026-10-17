#include <lcom/lcf.h>
#include "mouse.h"

int mouse_hook_id = 2;

uint8_t mouse_packet[3];
int mouse_next_index = 0;
uint8_t mouse_read_byte;
bool mouse_complete;
struct packet pp;


//Reads a byte and tries to build a mouse packet
void (mouse_ih)(){
    bool from_mouse = true;
    if (mouse_complete) {
        mouse_next_index = 0;
        mouse_complete = false;
    }
    kbc_get_out(&mouse_read_byte, from_mouse);

    if ((mouse_next_index == 0) && ((mouse_read_byte & BIT(3)) != 0)) {  //if waiting for first byte and getting it
        mouse_packet[mouse_next_index] = mouse_read_byte;

    }
    if ((mouse_next_index == 1) || (mouse_next_index == 2)) {   //if waiting for second and third byte
        mouse_packet[mouse_next_index] = mouse_read_byte;
    }
    mouse_next_index++;

    if (mouse_next_index == 3) {
        mouse_complete = true;
    }
}

//Organizes the mouse packet information in a packet struct
void (mouse_parse_packet)() {

    uint8_t byte_one = mouse_packet[0];


    //mouse packet raw bytes
    pp.bytes[0] = byte_one;
    pp.bytes[1] = mouse_packet[1];
    pp.bytes[2] = mouse_packet[2];

    //right mouse button pressed
    pp.rb = byte_one & RIGHT_BUTTON;

    //middle mouse button pressed
    pp.mb = byte_one & MIDDLE_BUTTON;

    //left mouse button pressed
    pp.lb = byte_one & LEFT_BUTTON;

    //mouse x-displacement overflow
    pp.x_ov = byte_one & X_OV;

    //mouse y-displacement overflow
    pp.y_ov = byte_one & Y_OV;

    //deltas and their signals as a uint16_t
    pp.delta_x = (byte_one & DELTA_X_SIGNAL) ? (0xFF00 | mouse_packet[1]) : mouse_packet[1];
    pp.delta_y = (byte_one & DELTA_Y_SIGNAL) ? (0xFF00 | mouse_packet[2]) : mouse_packet[2];
}



int (mouse_subscribe_int)(uint8_t *irq_set_mask) {
    if (irq_set_mask == NULL) return 1;

    *irq_set_mask = BIT(mouse_hook_id);

    if (sys_irqsetpolicy(MOUSE_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0) return 1;  //IRQ_EXCLUSIVE to disable default IH

    return 0;
}



//Subscribes and enables Mouse Interrupts
int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&mouse_hook_id) != 0) return 1;
    return 0;
}




//Sends Mouse Command Byte to the Mouse
int (mouse_write_byte)(uint8_t byte) {

    int max_tries = LIMIT_TRIES;
    uint8_t acknowledge_byte;


    while (max_tries != 0) {

        if (kbc_write_input(KBC_CTRL_REG, WRITE_BYTE_TO_MOUSE) != 0) return 1;  //Request forwarding of byte (command) to the mouse

        if (kbc_write_input(KBC_IN_BUF, byte ) != 0) return 1; //Write the byte (command) to port 0x60

        if (read_OBF(&acknowledge_byte) != 0) return 1; //Read the acknowledgement byte received from the mouse from port 0x60

        if(acknowledge_byte == NACK || acknowledge_byte == ERROR) return 1;


        if (acknowledge_byte == ACK) return 0;  //If the byte was sent successfully, return 0

         max_tries--;
    }

    
    return 1;  //If the byte couldn't be sent successfully in the limit of tries
}





//Disable data reporting (in Stream Mode)
int (mouse_disable_data_report)() {
    if (mouse_write_byte(DISABLE_DATA_REPORT) != 0) return 1;  //Write Mouse Command Byte 0xF5
    return 0;
}



//Enable data reporting (in Stream Mode)
int (mouse_enable_data_report)() {
    if (mouse_write_byte(ENABLE_DATA_REPORT) != 0) return 1;  //Write Mouse Command Byte 0xF4
    return 0;

}

//Set Mouse to operate in Stream Mode
int (mouse_set_stream_mode)() {
    if (mouse_write_byte(STREAM_MODE) != 0) return 1;  //Write Mouse Command Byte 0xEA
    return 0;
}


//Set Mouse to operate in Remote Modeyy
int (mouse_set_remote_mode)() {
    if (mouse_write_byte(REMOTE_MODE) != 0) return 1;  //Write Mouse Command Byte 0xF0
    return 0;
}
