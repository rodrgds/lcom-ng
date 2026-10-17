#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "keyboard.h"

#include <stdint.h>

#include "i8254.h"
#include "i8042.h"

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

int hookid = 1;

int (kbd_subscribe_int)(uint8_t *bit_no){
	*bit_no = hookid;
	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookid) != OK)
		return 1;
		

	//*bit_no = hookid;
	//printf("subscribe successful\n");
	return 0;
}

uint32_t kbdData = 0;

void (kbd_ih)(){
	uint32_t stat = 0;
	uint32_t data = 0;

	sys_inb_cnt(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
	if( stat & OBF ) {
		sys_inb_cnt(OUT_BUF, &data); /* assuming it returns OK */
		if ((stat & (PAR_ERR | TO_ERR)) == 0 ){
			kbdData = data;
        }
		else
			kbdData = -1;
	}
}

int(kbd_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hookid) != OK) {
    return -1;
  }
  //printf("unsubscibe successfull\n");
  return 0;
}

int cnt_sys_inb = 0;
int (sys_inb_cnt)(port_t port, uint32_t *byte){
	if (sys_inb(port, byte) != OK)
		return 1;
	cnt_sys_inb++;
	return 0; 
}

int(kbd_write)(uint8_t cmd) {
	uint32_t stat = 0;
    while( 1 ) {
        sys_inb(STAT_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
	    //uint32_t cmd = stat | INT;
        if( (stat & IBF) == 0 ) {
            sys_outb(KBC_CMD_REG, cmd); /* no args command */
            return 0;
        }	
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
}

int(kbd_read)() {
	uint32_t stat = 0;
	uint32_t data = 0;

    while( 1 ) {
		sys_inb_cnt(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb_cnt(OUT_BUF, &data); /* assuming it returns OK */
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 ){
				return data;
            }
			else
				return -1;
		}
        tickdelay(micros_to_ticks(WAIT_KBC));
	}
}

int(kbd_cmd_reset)(uint32_t cmd, uint32_t arg) {
	uint32_t stat = 0;
    while( 1 ) {
        sys_inb(STAT_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
	    //uint32_t cmd = stat | INT;
        if( (stat & IBF) == 0 ) {
            sys_outb(KBC_CMD_REG, cmd); /* no args command */
			//sys_outb(ARG_REG, 0xFE); /* passing arg */
            return 0;
        }	
        tickdelay(micros_to_ticks(WAIT_KBC));
		stat = arg;
    }
}

