#include <lcom/lcf.h>
#include <stdint.h>

int kb_inb_counter = 0;

int (util_get_LSB)(uint16_t val, uint8_t *lsb){
    if (lsb==NULL) return 1; //check if the pointer given is valid

    *lsb = 0xFF & val; //gets the last byte of val
    return 0; //everything went correctly
}

int (util_get_MSB)(uint16_t val, uint8_t *msb){
    if(msb==NULL) return 1; //check if the pointer given is valid

    *msb = (val>>8) & 0xFF; //shifts val one byte to the right
    return 0; //everything went correctly
}

int (util_sys_inb)(int port, uint8_t *val){
    if(val==NULL) return 1; //check if the pointer given is valid

    kb_inb_counter++;
    uint32_t value; //sys_inb returns a 32 bit value

    if(sys_inb(port, &value)){ //gets the information of the port into value
        return 1; //sys_inb system call returned an error
    }

    if (util_get_LSB(value,val)){ //we only want the least significant byte
        return 1; //util_get_LSB returned an error
    }

    return 0; //everything went correctly
}
