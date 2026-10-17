#include <lcom/lcf.h>
#include "serial_port.h"
#include "enum.h"
#include "program.h"

static int hook_uart = 5;
uint8_t data_uart;

extern enum online_state o_state;
extern bool game_already_played;
extern int points_adv;

int (uart_subscribe_int)(uint8_t *bit_no){

  int idhook_back = hook_uart;

  if(sys_irqsetpolicy(IRQ_COM1, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_uart) == 0){
    *bit_no |= BIT(idhook_back);
    return 0;
  } else {
    return 1;
  }
}


int (uart_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_uart) == OK)
    return 0;
  else
    return 1;
}

int uart_read_reg(uint8_t reg, uint8_t* arg){
  uint32_t tmp;
  if (sys_inb(BASE_COM1 + reg, &tmp))
    return 1;
  *arg = tmp;
  return 0;
}

int uart_write_reg(uint8_t reg, uint8_t arg){
  return sys_outb(BASE_COM1 + reg, arg);
}

int uart_enable_DL(){
  uint8_t d;
  if (uart_read_reg(LCR, &d))
    return 1;
  d |= UART_DLAB;
  return uart_write_reg(LCR, d);
}

int uart_disable_DL(){
  uint8_t d;
  if (uart_read_reg(LCR, &d))
    return 1;
  d &= ~UART_DLAB;
  return uart_write_reg(LCR, d);
}

int uart_set_bitrate(uint32_t bitrate){
  uint16_t div = 115200/bitrate;
  uint8_t LS_rate = div & 0xFF;
  uint8_t MS_rate = (div>>8) & 0xFF;
  if (uart_enable_DL())
    return 1;
  if (uart_write_reg(UART_DLL, LS_rate))
    return 1;
  if (uart_write_reg(UART_DLM, MS_rate))
    return 1;
  if (uart_disable_DL())
    return 1;
  return 0;
}

int uart_send_char(uint8_t c){
  int tries = 0;
  uint8_t lsr_stat;
  while (tries++ < 3){
    if (uart_read_reg(LSR, &lsr_stat))
      return 1;
    if (lsr_stat & UART_TX_RDY)
      return uart_write_reg(THR, c);
    tickdelay(micros_to_ticks(UART_WAIT_TIME));
  }
  return 1;
}

int uart_send_message(char* cs, int8_t s){
  for (int i = 0; i < s; i++){
    if (uart_send_char(cs[i]))
      return 1;
  }
  return 0;
}

int init_uart_proj(){

  uint8_t LCR_conf = 0, IER_conf = 0;

  LCR_conf |= UART_8_BITS | UART_2_STOP_BIT | UART_EVEN_PARITY;
  IER_conf |= UART_REC_DATA_AVL_INT | UART_REC_DATA_ERROR_INT;

  if (uart_write_reg(LCR, LCR_conf))
    return 1;

  if (uart_write_reg(IER, IER_conf))
    return 1;

  if (uart_set_bitrate(9600))
    return 1;

  return 0;
}

bool on_mess = false;
char begin_mess;
char mess[100];
int mess_index = 0;

void handle_mess(char c){
  if (on_mess){
    if ( (c - begin_mess) == 'A' - 'a'){
      on_mess = false; // HANDLE inf
      if (c == 'S'){
        o_state = (int) mess[0]-'0';
        if (o_state == ONLINE || o_state == OFFLINE)
          game_already_played = false;
      } else if ( c == 'P'){
        mess[mess_index] = '\0';
        points_adv = atoi(mess);
      }
    } else if ( c == 's' || c == 'p'){
      begin_mess = c;
      mess_index = 0;
    } else {
      mess[mess_index++] = c;
    }
  } else {
    if ( c == 's' || c == 'p'){
      on_mess = true;
      begin_mess = c;
      mess_index = 0;
    }
  }
}

void uart_ih(){
  uint8_t iir_stat;
  uint8_t c;

  if (uart_read_reg(IIR, &iir_stat))
    return;

  if (iir_stat & UART_IIR_NO_INT)
    return;

  iir_stat &= 0xF;

  if (iir_stat == UART_IIR_RLS){
    // printf("ERROR\n");
    uint8_t lsr_stat;
    if (uart_read_reg(LSR, &lsr_stat))
      return;
    if (lsr_stat & UART_DATA_READ)
      uart_read_reg(RBR, &lsr_stat); // Not useful
  } else if (iir_stat == UART_IIR_RDA){

    if (uart_read_reg(RBR, &c))
      return;

    // printf("%c", c);
    handle_mess(c);
  }
}
