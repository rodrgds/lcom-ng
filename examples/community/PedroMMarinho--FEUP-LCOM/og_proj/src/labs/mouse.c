#include <lcom/lcf.h>

#include "i8042.h"
#include "mouse.h"
#include "kbc.h"
#include <stdint.h>

static int mouse_hookId = 2;
static bool error = false;

static int byteNumber = 0;
static struct packet packet;

void(mouse_ih)() {
    uint8_t mouseByte;
    if (read_outputBuffer(&mouseByte)) error = true;
    if (updatePacket(mouseByte)) error = true;
}

bool mouseError(){
    return error;
}

int(mouse_subscribe)(uint8_t *bit_no) {
    if (bit_no == NULL)
        return 1;
    *bit_no = BIT(mouse_hookId);
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hookId);
}

int(mouse_unsubscribe)() {
    return sys_irqrmpolicy(&mouse_hookId);
}

int updatePacket(uint8_t mouseByte){
    packet.bytes[byteNumber++] = mouseByte;
    return 0;
}

int complete_packet() {

    // considering data inside the packet byte array is well written
    uint8_t firstByte = packet.bytes[0];
    packet.lb  = firstByte & MOUSE_DATA_PACKET_LB;
    packet.mb  = firstByte & MOUSE_DATA_PACKET_MB;
    packet.rb  = firstByte & MOUSE_DATA_PACKET_RB;
    packet.x_ov = firstByte & MOUSE_DATA_PACKET_XOVFL;
    packet.y_ov = firstByte & MOUSE_DATA_PACKET_YOVFL;

    packet.delta_x = ((firstByte & MOUSE_DATA_PACKET_XMSB) ? 0xff00 : 0x0) | packet.bytes[1];
    packet.delta_y = ((firstByte & MOUSE_DATA_PACKET_YMSB) ? 0xff00 : 0x0) | packet.bytes[2];

    return 0;
}

int disableDataReporting(){
    uint8_t argument = MOUSE_DISABLE_DATA_REPORTING;
    while (argument != MOUSE_OK){
        argument = MOUSE_DISABLE_DATA_REPORTING;
        if (kbc_issue_command(KBC_WRITE_BYTE_TO_MOUSE, &argument)) return 1;
    }
    return 0;
}

bool isPacketComplete(){
    return (byteNumber == 3);
}

struct packet getMousePacket(){
    byteNumber = 0; 
    complete_packet();
    return packet;
}
