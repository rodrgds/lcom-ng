#include "rtc.h"

int getRTCOutput(uint8_t cmd, uint8_t *output) {
    if (sys_outb(RTC_REG_INPUT, cmd)) return 1;
    return util_sys_inb(RTC_REG_OUTPUT, output);
}

bool isBinary() {
    uint8_t result;
    if (getRTCOutput(RTC_REG_COUNTING, &result)) return 1;
    return result & RTC_BIN_MASK;
}

uint8_t toBinary(uint8_t num) {
    return ((num >> 4) * 10) + (num & 0xF);
}

int readRTC(uint8_t what, uint8_t* result) {
    if (getRTCOutput(what, result)) return 1;
    if (!isBinary()) *result = toBinary(*result);
    return 0;
}
