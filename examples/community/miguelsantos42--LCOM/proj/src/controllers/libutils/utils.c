#include <lcom/lcf.h>
#include <stdint.h>
#include "src/include/Macros.h"
#include "src/include/utils.h"

#ifdef LAB3
uint64_t KBCCounter = 0;
#endif

uint8_t state;
uint8_t scancode;
uint8_t scanbytes[2];

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = val&0xFF;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = val>>8;
  return 0;
}

int read_status_register(){
  return util_sys_inb(KBC_STATUS_PORT, &state);
}

int read_buffer(){
  while (true) {
    if(read_status_register() != 0) return 1;
    
    // xxxxxxx1 -> buffer cheio, podemos ler
    // xxxxxxx0 -> buffer vazia, não temos nada para ler

    if((state & KBC_OBF) != 0){
        if((state & KBC_PAR_ERR) != 0) return 1;
        if((state & KBC_TO_ERR) != 0) return 1;
        
        if(util_sys_inb(KBC_OUT_BUF, &scancode) != 0) return 1; 

        if(scanbytes[0] == SCAN_CODE_HEADER) {
          scanbytes[1] = scancode;
          return 0;
        }

        scanbytes[0] = scancode;

        return 0;
    }  
  } 
}

// int kbd_print_scancode(bool make, uint8_t size, uint8_t bytes[])
int print_code(){
  if(kbd_print_scancode(!(scancode & MAKE_BIT), scanbytes[0] == SCAN_CODE_HEADER ? 2 : 1, scanbytes) != 0) return 1;

  scanbytes[0] = 0;
  scanbytes[1] = 0;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t buf;
  CHECK_RETURN(sys_inb(port, &buf));
  *value = (uint8_t)(buf & 0xFF);
  #ifdef LAB3
  ++KBCCounter;
  #endif
  return 0;
}

