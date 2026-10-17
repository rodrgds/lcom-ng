#include "keyboard.h"
int counterK = 0;
int flag = 0;
int hook_id_kb = 4;
uint8_t scode = 0x00;

void (kbc_ih)	() {
  uint8_t status;
  flag = 0;
  if(util_sys_inb(STAT_REG,&status) != 0) {
    flag = 1;
  }
  if((status & OBF_AUX) != 0x01) {
    flag = 1;
    printf("OBF Not Full\n");
    return;
  }
  if(util_sys_inb(OUT_BUF,&scode) != 0) {
    flag = 1;
  }
  if((status & ERROR_PARITY_TIMEOUT) != 0) {
    flag = 1;
    printf("Parity / Timeout Error\n");
  }

}

int (kb_subscribe)(uint8_t *bit_no) {
  *bit_no = hook_id_kb;
  if(sys_irqsetpolicy(IRQ_KB,IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kb) != 0) {
    printf("Error Subscribe Keyboard\n");
    return 1;
  }
  return 0;
}


int (kb_unsubscribe)() {
  if(sys_irqrmpolicy(&hook_id_kb) != 0) {
    printf("Error Unsubscribe Keyboard\n");
    return 1;
  }
  return 0;
}


void (enable_interrupts)() {
  uint8_t kbc;
  sys_outb(STAT_REG,0x20);
  util_sys_inb(OUT_BUF,&kbc);
  kbc |= BIT(0);
  sys_outb(STAT_REG, 0x60);
  sys_outb(OUT_BUF,kbc);
}
