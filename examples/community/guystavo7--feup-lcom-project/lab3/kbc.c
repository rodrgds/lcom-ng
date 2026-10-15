#include "kbc.h"
#include <lcom/lcf.h>
#include <stdint.h>

#define DELAY_US 20000

static int hook_id = 1;
static uint8_t topbyte = 0;
static uint8_t botbyte = 0;

static uint16_t lastByte = 0;

int(kbcSubscribe)(uint8_t *bit_no) {
    *bit_no = hook_id;
    int ret = sys_irqsetpolicy(KBC1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
	
	
    return ret;
}

int(kbcUnsubscribe)() { return sys_irqrmpolicy(&hook_id); }

static int kbcSendCommand(uint8_t cmd) {
    uint8_t stat = 0;
    int counter = 10;
    while (counter--) {
        util_sys_inb(KBC_ST_REG, &stat);
        if ((stat & KBC_ST_IBF) == 0) {
            sys_outb(KBC_CMD_REG, cmd);
            return 0;
        }

        tickdelay(micros_to_ticks(DELAY_US));
    }

    return -1;
}

static uint8_t kbcReadCommandResponse() {
    uint8_t stat, data;

    int counter = 10;
    while (counter--) {
        util_sys_inb(KBC_ST_REG, &stat);
        if (stat & KBC_OBF) {
            util_sys_inb(KBC_OUT_BUF, &data);
            if ((stat & (KBC_PARITY_ERR | KBC_TIMEOUT_ERR)) == 0)
                return data;
            else
                return -1;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    return -1;
}

uint8_t kbcEnableInterface() { return kbcSendCommand(0xAE); }
uint8_t kbcDisableInterface() { return kbcSendCommand(0xAD); }

bool kbc_self_check() {
    int ret;
    ret = kbcSendCommand(0xAA);
    uint8_t data = kbcReadCommandResponse();
    return data == 0x55;
}

bool kbc_check_interface() {
    int ret;
    ret = kbcSendCommand(0xAB);
    uint8_t data = kbcReadCommandResponse();
    return data == 0;
}

uint8_t kbc_read_command_byte() {
    kbcSendCommand(0x20);
    return kbcReadCommandResponse();
}

uint8_t kbcWriteCommandByte(uint8_t cmd_byte) {
    int ret = kbcSendCommand(0x60);
    sys_outb(KBC_IO_BUF_REG, cmd_byte);
    return ret;
}

void kbcEnableInterrupts() {
    uint8_t cmd_byte = kbc_read_command_byte();
    cmd_byte = cmd_byte & (~0x10);
    cmd_byte = cmd_byte | (0x01);
    kbcWriteCommandByte(cmd_byte);
}

void kbcDisableInterrupts() {
    uint8_t cmd_byte = kbc_read_command_byte();
    cmd_byte = cmd_byte | (0x10);
    cmd_byte = cmd_byte & (~0x01);
    kbcWriteCommandByte(cmd_byte);
}

void kbcResetKeys() {
    lastByte = kbcGetScancode();
    topbyte = 0;
    botbyte = 0;
}

uint8_t kbcGetMSB() { return botbyte; }
uint8_t kbcGetLSB() { return topbyte; }
bool kbcIsTwoByteCode() { return topbyte == 0xE0; }
uint8_t kbcGetSize() { return topbyte == 0xE0 ? 2 : 1; }

bool kbcIsMake() {
    uint8_t byte = botbyte;
    if (!botbyte) byte = topbyte;
    return !(byte & 0x80);
}

bool kbcIsKeyReady() { return (!kbcIsTwoByteCode() && topbyte) || botbyte; }

void(kbcIh)() {
    uint8_t status = 0;
    uint8_t key = 0;
    util_sys_inb(KBC_ST_REG, &status);
    bool erro = ((status & 0x80) >> 7) | ((status & 0x40) >> 6);

    if ((erro == 0) && (status & 0x1)) {
        util_sys_inb(KBC_IO_BUF_REG, &key);

        if (topbyte == 0) {
            topbyte = key;
        } else {
            botbyte = key;
        }
    }
}

uint16_t kbcGetScancode() {
    if (kbcIsTwoByteCode()) {
        return topbyte << 8 | botbyte;
    }

    return topbyte;
}

uint16_t _getLastByte() {
    if (lastByte == 0) return kbcGetScancode();
    return lastByte;
}
