#include <minix/devio.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8254.h"
#include "kbd.h"

#ifdef LAB3
unsigned int sys_inb_counter;

int sys_inb_cnt(port_t port, uint32_t *byte) {
    sys_inb_counter++;
    return sys_inb(port, byte);
}
#else
#define sys_inb_cnt(p, q) sys_inb(p, q)
#endif


static int kbd_hook_id = 2;
extern unsigned int sys_inb_counter;
uint8_t scancode[] = {0, 0};


int (kbd_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(kbd_hook_id);
  sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
  return 0;
}

int (kbd_unsubscribe_int)(){
	sys_irqrmpolicy(&kbd_hook_id);
	return 0;
}

void kbd_ih(){

	uint32_t tmp_scan;
	sys_inb_cnt(STATUS_REG, &tmp_scan);

	if((tmp_scan & OBF)	 == 1){
		sys_inb_cnt(OUT_BUF, &tmp_scan);
		if(scancode[0] != TWO_BYTE) scancode[0] = tmp_scan;
		else scancode[1] = tmp_scan;
	}
}

bool is_make(){
	if(scancode[0] != TWO_BYTE) return (scancode[0] & BIT(7)) == 0;
	else return (scancode[1] & BIT(7)) == 0;
}

bool is_twoByte(){
	return (scancode[0] == TWO_BYTE);
}

int poller(uint32_t data){
  if (scancode[0] != TWO_BYTE) scancode[0] = data;
  else scancode[1] = data;
  return 0;
}

int kbd_poll(){
  uint32_t tmp_scan;
  uint32_t tmp_data;
    if(sys_inb_cnt(STATUS_REG, &tmp_scan) != OK) return 1;
    if(tmp_scan & OBF){
      if(sys_inb_cnt(OUT_BUF, &tmp_data) != OK) return 2;
      int pollr = poller(tmp_data);
      if(!pollr) return 0;
    }
  return 3;
}

int kbd_cmd(uint8_t cmd){
  int tries = 0;
  uint32_t tmp_scan;
  while(tries < 5){
    sys_inb_cnt(STATUS_REG, &tmp_scan);
    if(!(tmp_scan & IBF)) {
      sys_outb(STATUS_REG, cmd);
      return 0;
    }
    tries++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int read_OB(uint32_t * data){
    sys_inb_cnt(OUT_BUF, data);
    return 0;
}
