#include <lcom/lcf.h>
#include <stdint.h>

uint32_t sys_inb_counter = 0;

int (util_sys_inb)(int port, uint8_t *value) {
    #ifdef LAB3
    sys_inb_counter++;
    #endif

    uint32_t temp_val;
    if (sys_inb(port, &temp_val) != 0) {
        return 1;
    }
    
    *value = (uint8_t) temp_val;
    return 0;
}
