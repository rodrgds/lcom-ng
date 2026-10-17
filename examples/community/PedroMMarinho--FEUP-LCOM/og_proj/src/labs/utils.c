#include <lcom/lcf.h>

#include <stdint.h>


int sysinb_count=0;

bool is_make_scancode(uint8_t scancode){
    return !(scancode & 0x80);
}

int create_scancode_array(uint8_t size, uint8_t scancode, uint8_t bytes[]){

    if (bytes == NULL) return 1;
    if (size > 2) return 1;

    bytes[0] = scancode;
    if (size == 2){
        bytes[0]= 0xE0;
        bytes[1] = scancode;
    }
    return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
    #ifdef LAB3
    sysinb_count++;
    #endif
    if (value == NULL)
        return 1;
    uint32_t intValue;
    if (sys_inb(port, &intValue) != 0)
        return 1;
    *value = intValue;
    return 0;
}

int get_no_sysinb(){
    return sysinb_count;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL)
        return 1;
    *lsb = val;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (msb == NULL)
        return 1;
    *msb = (val >> 8);
    return 0;
}

void (delay_milli_seconds)(unsigned milli_seconds){
  tickdelay(micros_to_ticks(milli_seconds*1000));
}

uint8_t (bcd_binary)(uint8_t value) {
  return (value >> 4) * 10 + (value & 0x0F);
}



