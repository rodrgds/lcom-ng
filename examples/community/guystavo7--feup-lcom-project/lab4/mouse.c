#include "mouse.h"
#include "utilsMouse.h"
#include <stdint.h>

int mouseSubscribe(int* hook_id) {
    return sys_irqsetpolicy(MOUSE_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_id);
}

int mouseUnsubscribe(int* hook_id) {
    return sys_irqrmpolicy(hook_id);
}

static int readStatus(uint8_t* status) {
    if (sys_inb_8b(STATUS_CMD_ADDR, status)) return -1;
    return 0;
}

int getStatusAndErrors(uint8_t* status, uint8_t* errors) {
    if (readStatus(status)) return -1;
    *errors = *status & (PARITY_MSK | TIMEOUT_MSK);
    return 0; }

bool canRead() {
    uint8_t status, errors;
    if (getStatusAndErrors(&status, &errors)) return false;
    return !errors && (status & OBF_MSK);
}

bool canWrite() {
    uint8_t status, errors;
    if (getStatusAndErrors(&status, &errors)) return false;
    return !errors && !(status & IBF_MSK);
}

static int writeCmd(uint8_t cmd) {
    if (sys_outb(STATUS_CMD_ADDR, REQUEST_BYTE_CMD)) return -1;
    for (int i = 0; i < TRIES; i++) {
        if (canWrite() && !sys_outb(INPUT_OUTPUT_ADDR, cmd)) return 0;
        tickdelay(micros_to_ticks(MOUSE_DELAY));
    }

    return -1;
}

static int readAckByte(uint8_t* byte) {
    for (int i = 0; i < TRIES; i++) {
        if (canRead() && !sys_inb_8b(INPUT_OUTPUT_ADDR, byte)) return 0;
        tickdelay(micros_to_ticks(MOUSE_DELAY));
    }

    return -1;
}

static int readOutputBuffer(uint8_t* byte) {
    if (canRead()) return sys_inb_8b(INPUT_OUTPUT_ADDR, byte);
    return -1;
}

int enableDataReport() {
    uint8_t ackByte = 0;
    for (int i = 0; i < TRIES; i++) {
        if (writeCmd(ENABLE_DATA_REP_CMD) || readAckByte(&ackByte)) continue;
        if (ackByte == SUCCESS_IN_ACK) return 0;
        tickdelay(micros_to_ticks(MOUSE_DELAY));
    }

    return -1;
}

int disableDataReport() {
    uint8_t ackByte = 0;
    for (int i = 0; i < TRIES; i++) {
        if (writeCmd(DISABLE_DATA_REP_CMD) || readAckByte(&ackByte)) continue;
        if (ackByte == SUCCESS_IN_ACK) return 0;
        tickdelay(micros_to_ticks(MOUSE_DELAY));
    }

    return -1;
}

void(mouse_ih)() {
    uint8_t byte;
    if (readOutputBuffer(&byte)) return;
    switch (currentByte) {
        case BYTE_1:
            if (byte & SYNC_MASK_BIT) {
                byte1 = byte;
                currentByte = BYTE_2;
            }

            break;

        case BYTE_2:
            byte2 = byte;
            currentByte = BYTE_3;
            break;

        case BYTE_3:
            byte3 = byte;
            currentByte = READY;
            break;

        default:
            break;
    }
}

uint8_t getByte1() {
    return byte1;
}

uint8_t getByte2() {
    return byte2;
}

uint8_t getByte3() {
    return byte3;
}

bool pressedRightButton() {
    return byte1 & RIGHT_BUTTON_MSK;
}

bool pressedLeftButton() {
    return byte1 & LEFT_BUTTON_MSK;
}

bool pressedMiddleButton() {
    return byte1 & MIDDLE_BUTTON_MSK;
}

int16_t getDeltaX() {
    int16_t byte = byte2;
    if (byte1 & X_SIGN_MSK) byte = byte - 256;
    return byte;
}

int16_t getDeltaY() {
    int16_t byte = byte3;
    if (byte1 & Y_SIGN_MSK) byte = byte - 256;
    return byte;
}

bool isOverflowingX() {
    return byte1 & OVERFLOW_X_MSK;
}

bool isOverflowingY() {
    return byte1 & OVERFLOW_Y_MSK;
}

void cleanBytes() {
    byte1 = 0;
    byte2 = 0;
    byte3 = 0;
    currentByte = BYTE_1;
}

bool isReady() {
    return currentByte == READY;
}
