#include "src/include/controllers/kbc/mouse.h"
#include "src/include/controllers/kbc/KBC.h"
#include "src/include/Macros.h"
#include "../i8042.h"

static int hookId = 3;
static uint8_t numberBytesRead = 0;
static uint8_t readBytes[3];
MouseInput *mouseFormattedInput = NULL;

int (mouseSubscribeInt)(uint8_t *bit_no){
    *bit_no = BIT(hookId);
    CHECK_RETURN(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookId));
    return 0;
}

int (mouseUnsubscribeInt)(){
    CHECK_RETURN(sys_irqrmpolicy(&hookId));
    return 0;
}

static void (formPacket)(){
    struct packet mousePacket;
    mousePacket.bytes[0] = readBytes[0];
    mousePacket.bytes[1] = readBytes[1];
    mousePacket.bytes[2] = readBytes[2];
    mousePacket.lb = readBytes[0] & MOUSE_LEFT;
    mousePacket.rb = readBytes[0] & MOUSE_RIGHT;
    mousePacket.mb = readBytes[0] & MOUSE_MIDDLE;
    mousePacket.delta_x = (readBytes[0] & MOUSE_X_DELTA) ? (0xFF00 | readBytes[1]) : readBytes[1];
    mousePacket.delta_y = (readBytes[0] & MOUSE_Y_DELTA) ? (0xFF00 | readBytes[2]) : readBytes[2];
    mousePacket.x_ov = readBytes[0] & MOUSE_X_OVERFLOW;
    mousePacket.y_ov = readBytes[0] & MOUSE_Y_OVERFLOW;
    mouseFormattedInput = mouseConstructor(&mousePacket);
    numberBytesRead = 0;
}

void (mouse_ih)(){
    if(mouseFormattedInput){
        mouseFormattedInput->destroy(mouseFormattedInput);
        mouseFormattedInput = NULL;
    }
    uint8_t readByte;
    CHECK_ERROR(kBCReadMouse(&readByte),"Unable to read mouse byte.\n","");
    if((readByte&MOUSE_CTRL_BYTE) || numberBytesRead){
        readBytes[numberBytesRead++] = readByte;
        if(numberBytesRead == 3) formPacket();
    }
}

int (mouseWriteCommand)(uint8_t cmd){
    uint8_t tries = MAX_TRIES;
    while(tries-- > 0){
        uint8_t mouseOutput;
        CHECK_RETURN(kBCCommand(WRITE_MOUSE));
        CHECK_RETURN(kBCArgument(cmd));
        tickdelay(micros_to_ticks(KBC_WAIT_TIME));
        CHECK_RETURN(util_sys_inb(KBC_OUT_BUF,&mouseOutput));
        if(mouseOutput == MOUSE_ACK) return 0;
    }
    return 1;
}
