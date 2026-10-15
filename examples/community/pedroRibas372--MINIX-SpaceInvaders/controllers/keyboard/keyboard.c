#include "keyboard.h"

static int hook_id;
uint8_t latest_scancode [2];
uint8_t size = 0;
uint8_t done = 0;


int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  
  hook_id = KBD_HOOK_ID;
  *bit_no = hook_id;
  
  if (sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)!=OK) return 1;
  return 0;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}

int (readByte)(uint8_t * data){
  uint8_t status;
  uint8_t tries = 0;

  while(tries < 3){
    tries++;
    
    //Read status register
    if (util_sys_inb(KBC_ST_REG, &status) != OK) {
      printf("Error reading status register\n");
      break;
    }

    if (status & 0x01) { // There is data on output buffer
      if (util_sys_inb(KBC_OUT_BUF, data) != OK) {
        printf("Error reading output buffer\n");
        break;
      }

      if (status & (TIMEOUT_ERROR | PARITY_ERROR)) { // Check errors
        printf("Parity or Timeout error\n");
        break;
      }

      return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

void (kbc_ih)() {
  uint8_t data;
  if(readByte(&data) == 0){
    if (size < 2){
      latest_scancode[size] = data;
      size++;
    } 
    
    if(data != TWO_BYTES){
      done = 1;
      size = 0;
    }

    else {
      done = 0;
    }
  }
}

int (keyboard_scan)() {

  int ipc_status;
  message msg;
  uint8_t bit_no;
  uint32_t irq_set;
  int r;
  bool make;

  if (kbd_subscribe_int(&bit_no) != 0) return 1;

  irq_set = BIT(bit_no);

  while (latest_scancode[0] != ESCAPE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { //Interrupt
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) { //Hardware interrupt
        if (msg.m_notify.interrupts & irq_set) { //Interrupt for keyboard
          kbc_ih(); 
            
          if (done) {
            //Makecode --> BIT(7) == 0 --> key pressed
            //Breakcode --> BIT(7) == 1 --> key released
            make = !(latest_scancode[latest_scancode[0] == TWO_BYTES ? 1: 0] & MAKE_CODE);
          }
        }
      }
    }
  }

if (kbd_unsubscribe_int() != OK) {
    printf("Failed to unsubscribe keyboard interrupts\n");
    return 1;
}


  return 0;
}
