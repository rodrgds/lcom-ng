#include "keyboard.h"

uint8_t status;
uint16_t scancode = 0x0000;
int hook_id_keyboard = 1;

int (kbd_subscribe_int)(uint8_t *bit_no){
    int aux = (int)*bit_no;
    
    if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&aux))
        return 1;

    *bit_no = (uint8_t)aux;
    return 0;
}

int (kbd_unsubscribe_int)() {
  //Unsubscribing the interruptions
  if(sys_irqrmpolicy(&hook_id_keyboard))
    return 1;

  return 0;
}

int (kbd_read_buffer)(uint16_t * scancode){
  uint8_t temp = 0x00;
  if(!util_sys_inb(OUT_BUF,&temp)) return 1;

  if (temp == 0xE0){
    *scancode |= 0xE000;
  }
  else{
    *scancode |= temp;
  }
  return 0;
}

int (kbd_read_status)(uint8_t * status){
  uint8_t temp;

  if(!util_sys_inb(STATUS_REG,&temp))
    return 1;
  *status = (uint8_t) temp;
  return 0;
}

int (isValidStatus)(){
    //first reads the status register
  if (kbd_read_status(&status) == 0) {
    return 1;
  }
  //checks if the ouput buffer has data to read
  if (status & KBC_OUTB) {
    //checks if the data is valid 
    if ((status & (KBC_PARE | KBC_TIMEO | KBC_AUXB)) == 0) {
      return 0;
    }
    else {
      return 1; //data has errors
    }
  }
  else
    return 1; //no data to read
}

void (kbc_ih)(void) {
  if(isValidStatus()){
    kbd_read_buffer(&scancode);
    //printf("Scancode: %X\n", scancode);
  }
}

void(kbc_reset_scancode)(void){
   if (scancode != 0x81) scancode = 0x0000;
}

int (kbc_poll)(){
  if (!util_sys_inb(STATUS_REG, &status))
    return 1;

  if ((status & (KBC_TIMEO | KBC_PARE | KBC_AUXB)) == 0){
    if (status & KBC_OUTB){
      kbd_read_buffer(&scancode);
      if (scancode != FIRST_OF_TWO_BYTES) kbc_print();
      else {
        kbd_read_buffer(&scancode);
        kbc_print();
      }
      return 0;
    }
    else return 0;
  }
  else return 1;

}

void (kbc_print)(){
  uint8_t size = 0x01;
  bool make = true;
  uint8_t lsb;
  uint8_t msb;
  util_get_LSB(scancode, &lsb);  
  util_get_MSB(scancode, &msb);

  if (msb != 0x00){
    uint8_t bytes[2];
    bytes[0] = msb;
    bytes[1] = lsb;
    size = 0x02;
    if ((bytes[1] & 0x80) == 0x80) make = false;
    kbd_print_scancode(make, size, bytes);
  }
  else{
    if ((lsb & 0x80) == 0x80) make = false;
    kbd_print_scancode(make, size, &lsb);
  }
  kbc_reset_scancode();
}


