#include <lcom/lcf.h>
#include "serial_port.h"


int (get_lcr)(uint8_t *lcr)
{
  return util_sys_inb(COM1_ADDR + SERP_LCR, lcr);
}

int (set_bit_rate)(uint16_t bit_rate){
    uint8_t lcr;
    get_lcr(&lcr);
    if (sys_outb(COM1_ADDR + SERP_LCR, lcr | LCR_DLAB)){printf("bit rate\n");return 1;}
    uint8_t msb;
    uint8_t lsb;
    util_get_MSB(FIXED_FREQUENCY/bit_rate, &msb);
    util_get_LSB(FIXED_FREQUENCY/bit_rate, &lsb);
    sys_outb(COM1_ADDR + SERP_DLM, msb);/* escreve MSB do divisor */
    sys_outb(COM1_ADDR + SERP_DLL, lsb);/* escreve LSB do divisor */
    return sys_outb(COM1_ADDR + SERP_LCR, lcr & 0x7f);
    
}

int (setup_lcr) (int length, int stop ){
    //return on invalid
    uint8_t lcr = 0x00;
    get_lcr(&lcr);
    switch (length) {
    case 5:
      break;
    case 6:
      lcr = lcr | LCR_WORD_LENGTH_6;
      break;
    case 7:
      lcr = lcr | LCR_WORD_LENGTH_7;
      break;
    case 8:
      lcr = lcr | LCR_WORD_LENGTH_8;
      break;
    default:
      break;
  }

    switch (stop) {
        case 1:
        break;
        case 2:
        lcr = lcr | LCR_STOP_2;
        break;

        default:
        break;
    }

    //parity even for now
    lcr = lcr | LCR_PARITY_EVEN;

    return (sys_outb(COM1_ADDR + SERP_LCR, lcr));
}

int get_ier( uint8_t *p){

    return util_sys_inb(COM1_ADDR + SERP_IER, p);

}

int (ier_enable_receive)(){
    uint8_t ier = 0x00;
    ier = ier | IER_ENREC_INT;
    return sys_outb(COM1_ADDR + SERP_IER, ier);

}

int (ier_enable_transmit_lsr)(){
    uint8_t ier;
    get_ier(&ier);
    ier = ier | IER_ENT_INT | IER_EN_LS_INT;
    return sys_outb(COM1_ADDR + SERP_IER, ier);
}

int serial_hook_id = 6;

int (serial_subscribe_int) (uint8_t *bit_no){
    *bit_no = BIT(serial_hook_id);
    return (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &serial_hook_id));
}


int (set_fcr)(uint8_t fcr){
  if(sys_outb(COM1_ADDR + SERP_FCR,fcr )) return 1;
  return 0;
}

int (fifo_en)(){
  uint8_t fcr;
  fcr = 0x00;
  fcr = fcr | FCR_EN_1 ;
  return set_fcr(fcr);

}

int (send_char)(uint8_t char_send){
  if (sys_outb(COM1_ADDR, char_send)){printf("\nerror SEND\n");return 1;}
  printf("\nSEND CHAR FUNC SUCCESS\n");
  return 0;
}

int (serp_undo)(){
  sys_irqrmpolicy(&serial_hook_id);
  if(set_fcr(0x00))return 1;
  uint8_t ier;
  get_ier(&ier);
  if(sys_outb(COM1_ADDR + SERP_IER, ier & 0xf8))return 1;
  return 0;

}

int(get_iir)(uint8_t *iir){
  return util_sys_inb(COM1_ADDR + SERP_IIR,iir);
}

int(get_lsr)(uint8_t *lsr){
  return util_sys_inb(COM1_ADDR + SERP_LSR,lsr);
}



