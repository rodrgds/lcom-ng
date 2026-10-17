#include "control.h"
#include <lcom/lcf.h>
#include "../devices/serialPort/serial_port.h"

int timer_interrupt() {
  (timer_ih)();
  ti_menu();
  ti_game();
  swap_buffers();
  return 0;
}

int keyboard_interrupt() {
  (kbc_ih)();
  ki_game();
  ki_menu();
  return 0;
}

int mouse_interrupt() {
  mouse_ih();
  if (byte_pos == 3) {
    packet_load();
    mouse_action();
  }
  return 0;
}

int mouse_action() {
  mouseaction_menu();
  mouseaction_game();
  return 0;
}

int serp_identify(uint8_t *iir){
  printf(" \n SERIAL INTERRUPT \n");
  
  get_iir(iir);
  if(*iir & BIT(0)){
    //not serial port interrupt, erro
    printf("no int pending\n");
    return 0;
  }
  printf("IIR\n");
  //print IIR
  uint8_t iircpy=*iir;
  printBinary(iircpy);
  printf("\n");
  //END PRINT IIR
  printf("IIR to 3\n");
  //print IIR transformation to 3
  uint8_t iir3=(*iir & 0xE)>>1;
  printBinary(iir3);
  printf("\n");
  //END PRINT IIR

  printf("the vaue going in to the switch%d \n",(*iir & 0xE)>>1 );//0b0000XXX1 //&// 0b00001110 (E)//>>1
  return 0;
}

int ser_p_interrupt(){
  uint8_t char_rec, iir,lsr;
  serp_identify(&iir);
  switch((iir & 0xE)>>1){
    case 3:
      //get lsr to reset inetrrupts, we ignore this interrupt because it happened cause an error occured
      if(get_lsr(&lsr)){
        printf("get lsr returned cause of error\n");  
        return 1;
      }
      printf("lsr detected error, move on\n");
      break;
    case 2:
      if(util_sys_inb(COM1_ADDR, &char_rec)){
      printf("sysinb error\n");
      return 1;
      }
      if(char_rec == 0xFD){
        allow_stand();
        break;
      }
      //wait handling
      if(char_rec == 0xFF){
        received_quit();
        break;
      }
      if(char_rec == 0xFE){
        received_duel();
        break;
      }
      //wait handling over

      //guardar value numa variavel para calcular winner no final e desenhar carta
      serp_card_attribution(&char_rec);
      printf("received char successfully and the char is %d\n",char_rec);
      break;
    case 6://não deve acontecer mas basicamente é quando fifo nao enche num intervalo de tempo chera interrupt sem encher, reset interrupt é igual ao case 2 mesmo que o fifo tenha por exemplo 2 chars(impossivel no nosso caso) basta ler um char para dar reset aos interrupts
      if(util_sys_inb(COM1_ADDR, &char_rec)){
      printf("sysinb error\n");
      return 1;
      }
      printf("received char successfully and the char is %d\n",char_rec);
      break;
    case 1:
      //printf("probably send here from list that does not exist yet, for now sending one more char\n");
      // find out a way to reset interrupt without sending char
      //find out isr(isr is iir ... ) , data sheet different
      //send_char(0x05);
      printf("interrupt empty\n");
      get_iir(&iir);//resets interrupts
      break;
    default:
      break;
  }
  
  return 0;
}

