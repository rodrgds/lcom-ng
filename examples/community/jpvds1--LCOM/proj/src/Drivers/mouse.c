#include "mouse.h"


// Mouse packet macros
#define MOUSE_LB        BIT(0)
#define MOUSE_RB        BIT(1)
#define MOUSE_MB        BIT(2)
#define MOUSE_X_DELTA   BIT(4)
#define MOUSE_Y_DELTA   BIT(5)
#define MOUSE_X_SIGNAL  BIT(6)
#define MOUSE_Y_SIGNAL  BIT(7)


// Other mouse macros
#define KBC_WRITE_PORT    0x64
#define KBC_PORT          0x60
#define WRITE_TO_MOUSE    0xD4
#define ACK               0xFA
#define ENABLE_STREAM     0xEA
#define ENABLE_DATA_REP   0XF4
#define IS_MOUSE          BIT(5)
#define IS_FIRST_BYTE     BIT(3)
#define TIMEOUT_ERROR     BIT(6)
#define PARITY_ERROR      BIT(7)
#define OUT_BUFFER_FULL   BIT(0)

uint8_t mouseBytes[3];
int mouse_counter = 0;
uint32_t byte;
struct packet mousePacket;
int mouse_x = 5;
int mouse_y = 5;
int hook_mouse = 5;

extern vbe_mode_info_t mode_inf;




int (write_to_mouse)(uint8_t command) {

    uint32_t response;



    for (int i = 0; i < 10; i++) {
        if (mouse_write_to_KBC(KBC_WRITE_PORT, WRITE_TO_MOUSE)) continue; //Allow writing to mouse

        if (mouse_write_to_KBC(KBC_PORT, command)) continue;

        if (sys_inb(KBC_PORT, &response)) continue;

        if (response == ACK) return 0;
    }


    return 1;
}


int (proj_enable_data_reporting)() {
    if (write_to_mouse(ENABLE_STREAM)) return 1; //Enable stream mode for mouse

    if (write_to_mouse(ENABLE_DATA_REP)) return 1; //Enable data reporting

    return 0;
}


int (mouse_write_to_KBC)(uint8_t port, uint8_t command) {

    uint32_t status;

    for (int i = 0; i < 10; i++) {
        if (sys_inb(KBC_WRITE_PORT, &status)) continue;
        if (status & (TIMEOUT_ERROR | PARITY_ERROR)) continue;
        if (sys_outb(port, command)) continue;

        return 0;
    }

    return 1;
}


//F4 - ENABLES DATA REPORTING IN THE KBC


void (mouse_ih)() {

    uint32_t output;

    if (mouse_read_output(&output)) {printf ("Error reading output in ih");}    

    //Organizing the 3 bytes

    if (mouse_counter == 0 && (output & IS_FIRST_BYTE)) {
        mouseBytes[0] = output;
        mouse_counter++;
    }
    else {
        mouseBytes[mouse_counter] = output;
        mouse_counter++;
    }
    
    
}

int (mouse_read_output)(uint32_t *output) {

    uint32_t status;

    for (int i = 0; i < 10; i++) {

        if (sys_inb(KBC_WRITE_PORT, &status)) continue;

        if (status & OUT_BUFFER_FULL) {
            if (status & (TIMEOUT_ERROR | PARITY_ERROR)) continue;

            if (!(status & IS_MOUSE)) continue;

            if (sys_inb(KBC_PORT, output)) continue;
            
            return 0;
        }
    }
    printf("Out of attempts");
    return 1;
}


int (mouse_write_to_packet)() {
    for (int i = 0; i < 3; i++) {
        mousePacket.bytes[i] = mouseBytes[i];
    }

    mousePacket.lb = mouseBytes[0] & MOUSE_LB;
    mousePacket.mb = mouseBytes[0] & MOUSE_MB;
    mousePacket.rb = mouseBytes[0] & MOUSE_RB;

    mousePacket.x_ov = mouseBytes[0] & MOUSE_X_SIGNAL;
    mousePacket.y_ov = mouseBytes[0] & MOUSE_Y_SIGNAL;

    if (mouseBytes[0] & MOUSE_X_DELTA) { //need to switch signal
        mousePacket.delta_x = mouseBytes[1] | 0xFF00;
    }
    else {mousePacket.delta_x = mouseBytes[1];}

    if (mouseBytes[0] & MOUSE_Y_DELTA) { //need to switch signal
        mousePacket.delta_y = 0XFF00 | mouseBytes[2];
    }
    else {mousePacket.delta_y = mouseBytes[2];}

    return 0;
}


void (mouse_update_coords)() {
    if (!mousePacket.x_ov) {
        if (mouse_x + mousePacket.delta_x < 0) {mouse_x = 0;}
        else if (mouse_x + mousePacket.delta_x > mode_inf.XResolution - 10) {mouse_x = mode_inf.XResolution - 10;}
        else {mouse_x += mousePacket.delta_x;}
    }
    if (!mousePacket.y_ov) {
        if (mouse_y - mousePacket.delta_y < 0) {mouse_y = 0;}
        else if (mouse_y - mousePacket.delta_y > mode_inf.YResolution - 10) {mouse_y = mode_inf.YResolution - 10;}
        else {mouse_y -= mousePacket.delta_y;}
    }

    //mouse_print_packet(&mousePacket);
    //printf("deltaX = %d, deltaY = %d\n", mousePacket.delta_x, mousePacket.delta_y);
    //printf("mouseX = %d, mouseY = %d, deltaX = %d, deltaY = %d\n", mouse_x, mouse_y, mousePacket.delta_x, mousePacket.delta_y);
}

int (mouse_subscribe_int)(uint8_t *irq_set) {
    if (irq_set == NULL) {return 1;}

    *irq_set = BIT(hook_mouse);
    return sys_irqsetpolicy(12, IRQ_ENABLE | IRQ_EXCLUSIVE, &hook_mouse);
}

int (mouse_unsubscribe_int)() {
    if(sys_irqrmpolicy(&hook_mouse)) return 1;
    return 0;
}
