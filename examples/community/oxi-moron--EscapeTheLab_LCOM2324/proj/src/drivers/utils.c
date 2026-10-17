#include <lcom/lcf.h>
#include <assert.h>

int (util_get_MSB)(uint16_t data, uint8_t* msb) {
    *msb = (uint8_t)(data & 0xFF00) >> 8;
    return 0;
}

int (util_get_LSB)(uint16_t data, uint8_t* lsb) {
    *lsb = (uint8_t)(data & 0x00FF);
    return 0;
}

int (util_sys_inb)(int port, uint8_t* data) {
    uint32_t val = *data;

    assert(sys_inb(port, &val) == 0);

    *data = val & 0x000000FF;
    return 0;
}
