#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"
#include <lcom/utils.h>

int cnt;
int hook_id_kbd=IRQ1;
uint8_t data;
uint8_t arr[2];
bool twobyte = false;


int (kbd_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id_kbd;
    if(sys_irqsetpolicy(IRQ1, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id_kbd) != 0)
      return 1;

  return 0;
}

int (kbd_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id_kbd) != 0)
      return 1;

  return 0;
}


void (kbc_ih)() {
    read_kbc();
}

int read_kbc(){
  uint8_t stat;
  while( 1 ) {
    if(util_sys_inb(STATUS_REG, &stat)!=0) return 1; /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if( stat & OBF ) {
      if(util_sys_inb(OUT_BUF, &data)!=0) return 1; /* ass. it returns OK */
        if ( (stat &(PARITY_ERROR| TIMEOUT_ERROR) ) == 0 ){
          return data;
        }
        else
          return -1;
    }
    tickdelay(micros_to_ticks(DELAY_US)); 
  }
}

void get_data(){
  if(twobyte == true){
    arr[1] = data;
    twobyte = false;
  }

  else if(data == FIRST_BYTE){
    arr[0]=data;
    twobyte = true;
  }

  else{
    arr[0] = data;
  }
}

int size(uint8_t data){
  return sizeof(data);
}

bool isMake(uint8_t data){
  if(data & BIT(7))
    return false;
    
  return true;
}





//KBC_TEST_POLL

int read_kbc_poll(){
  uint8_t stat;
  while( 1 ) {
    if(util_sys_inb(STATUS_REG, &stat)!=0) return 1; /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if( stat & OBF ) {
        if(util_sys_inb(OUT_BUF, &data)!=0) return 1; /* ass. it returns OK */
          if ( (stat &(PARITY_ERROR| TIMEOUT_ERROR| AUX) ) == 0 ){
            return data;
          }
          else
            return -1;
      
    }
    tickdelay(micros_to_ticks(DELAY_US)); 
  }
}

int write_command(){
  uint8_t st;
  sys_outb(STATUS_REG, READ_COMMAND_BYTE);
  util_sys_inb(OUT_BUF, &st);

  st = st | BIT(0);

  sys_outb(STATUS_REG, WRITE_COMMAND_BYTE);
  sys_outb(OUT_BUF, st);

  return 0;
}
