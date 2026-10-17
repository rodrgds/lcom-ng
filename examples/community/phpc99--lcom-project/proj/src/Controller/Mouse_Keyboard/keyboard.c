#include <lcom/lcf.h>

#include <stdint.h>
#include "i8042.h"

/**
 * @file keyboard.c
 * @brief Implements keyboard interrupt handling and scan code parsing.
 */

int hook_id_keyboard = 1;
uint8_t scan_code[2];
uint8_t size = 1;
bool waiting_keyboard = false;

int (keyboard_subscribe_int)(uint8_t *bit_no) {

    *bit_no = hook_id_keyboard;
  
    if(sys_irqsetpolicy(KEYBOARD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_keyboard) != 0) return 1;
  
    return 0;
  }
  
  int (keyboard_unsubscribe_int)() {
    
    if(sys_irqrmpolicy(&hook_id_keyboard) != 0) return 1;
  
    return 0;
  }


  void (kbc_ih)() {
    uint8_t status;
    uint8_t data;

    util_sys_inb(KBC_ST_REG, &status); /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if( status & KBC_OBF ) {
        util_sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
        if ( (status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){
            if(data == 0xE0){
              scan_code[0] = data;
              waiting_keyboard = true;
              size = 2;
              return ;
            }else if(waiting_keyboard == true && scan_code[0]== 0xE0){
              scan_code[1] = data;
              waiting_keyboard = false;
              return ;
            }
            else{
              scan_code[0] = data;
              waiting_keyboard = false;
              size = 1;
              return;
            }
        }
        else{
          
        }
            
    }
    tickdelay(micros_to_ticks(DELAY_US)); // e.g. tickdelay()
}

int(read_out)(){
  uint8_t status;
    uint8_t data;

    util_sys_inb(KBC_ST_REG, &status); /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if( status & KBC_OBF ) {
        util_sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
        if ( (status & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){
            if(data == 0xE0){
              scan_code[0] = data;
              waiting_keyboard = true;
              size = 2;
              return 0;
            }else if(waiting_keyboard == true && scan_code[0]== 0xE0){
              scan_code[1] = data;
              waiting_keyboard = false;
              return 0;
            }
            else{
              scan_code[0] = data;
              waiting_keyboard = false;
              size = 1;
              return 0;
            }
        }
        else{
          
        }
            
    }
    return 1;
}
