#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0; //this will identify our program's subscription to an IRQ line (so that the kernel can track/manage interrupt subscriptions)
int timer_counter = 0; //used to count the number of interrupts --- see lab2.c



int (timer_subscribe_int)(uint8_t *bit_no) {
  
  if(bit_no == NULL) return 1;    // we validate the input
  *bit_no = BIT(timer_hook_id);   // the bit position of the IRQ line we're subscribing to (derived from the hook_id, using the macro)
  
  /*
  sys_irqsetpolicy(interrupt, policy, *hook_id) is a sys call provided by Minix that subscribes an interrupt to an IRQ line
  it tells the kernel that out program wants to handle interrupts from the specified IRQ line
  
  interrupt -> we choose which IRQ line we want to subscribe (TIMER0_IRQ meaning timer 0)
  policy -> we choose the interrupt handling policy (IRQ_REENABLE means that the kernel will re-enable the interrupts)
  hook_id -> used by kernel to track the subscription (it is passed to the kernel but it's also modified, it'll be set to the value of the IRQ line we are subscribing, plus other internal info)
  */
  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &timer_hook_id) != 0) return 1;  
  
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook_id) != 0) return 1;
  return 0;
}

void (timer_int_handler)() {
  timer_counter++;
}






int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  // confirmamos que os inputs são válidos
  if (st == NULL || timer < 0 || timer > 2) return 1;

  // construimos o nosso read-back command para ler o status do timer (temos de ativar o bit 5)
  uint8_t rb_cmd = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1); // = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)
  
  // avisamos previamente o registo de controlo que vamos ler a config
  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) return 1;

  // finalmente, lemos a config do timer, que é colocada em st
  //-> o registo do timer pretendido é obtido através de TIMER_0 + timer 
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;

  return 0;
}





// já temos o valor st, que contem a config do timer (fazer timer_get_config)
// agora vamos exibir um field específico da configuração

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
  // vamos ter de evocar uma funçao - já dada - para imprimir a config:
  // int timer_print_config(uint8_t timer, enum timer_status_field field, union timer_status_field_val conf)
  
  // para isso, precisamos de fazer a tradução/extração do nosso 'uint8_t st' para o formato 'union timer_status_field_val conf'
  union timer_status_field_val conf;


  switch (field) {

    // queremos o status byte (st) inteiro
    case tsf_all:  
      conf.byte = st;
      break;
    

    // queremos só os bits de inicialização -> bits 4 e 5
    case tsf_initial:  
      // fazemos uma right shift de 4 bits para colocar os bits 4-5 nas posições 0-1
      // e damos clear aos restantes bits 
      st = (st >> 4) & 0x03;
      
      // existem 4 resultados possíveis
      if (st == 1) conf.in_mode = LSB_only;  // st -> 01
      else if (st == 2) conf.in_mode = MSB_only;  // st -> 10
      else if (st == 3) conf.in_mode = MSB_after_LSB;  // st -> 11
      else conf.in_mode = INVAL_val;  // st -> 00
      
      break;


    // queremos só os bits de modo -> bits 1,2 e 3
    case tsf_mode:
      // mesmo procedimento de antes, só que agora para os bits 1, 2 e 3
      st = (st >> 1) & 0x07;

      // o valor que nos der em st é o valor do modo (verificar na tabela), 
      // EXCETO 2 casos "especiais" que devem ser mapeados:
      // 110 -> modo 2   (0b0110 <-> 6)
      // 111 -> modo 3   (0b0111 <-> 7)
      if (st == 6) conf.count_mode = 2;
      else if (st == 7) conf.count_mode = 3;
      else conf.count_mode = st;

      break;
    

    // queremos só o bit da base -> bit 0
    case tsf_base:
      conf.bcd = st & 0x01;;
      break;


    default:
      return 1;
  }
  


  // finalmente, fazemos a impressão da configuração
  if (timer_print_config(timer, field, conf) != 0) return 1;
  return 0;

}




int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  /* validamos inputs
  valor do contador pode ser, no máximo, 2^16 = 65536
  TIMER_FREQ no minix é 1193182
  sendo counter = TIMER_FREQ / freq, a freq tem de ser, no mínimo, 1193182/65538 = 18.2

  ->portanto, se freq < 19 o contador dá overflow!!!  */
  if (freq < 19 || freq > TIMER_FREQ || timer < 0 || timer > 2) return 1;


  /*parte 1*/
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer + 1);
  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) return 1;

  uint8_t old_conf, new_conf = 0;
  timer_get_conf(timer, &old_conf);

  //-> new configuration:

  //timer selection
  uint8_t selectedTimer;

  switch(timer) {
      case 0: 
        new_conf |= TIMER_SEL0; 
        selectedTimer = TIMER_0;
        break;
      
      case 1: 
        new_conf |= TIMER_SEL1; 
        selectedTimer = TIMER_1;
        break;
      
      case 2: 
        new_conf |= TIMER_SEL2;
        selectedTimer = TIMER_2;
        break;
      
      default: 
        return 1;
  }

  //register selection
  new_conf |= TIMER_LSB_MSB;

  //-> slide 18 : we do not change 4 LSB (mode and BCD/binary)
  new_conf |= old_conf & 0x0F;


  /*parte 2*/

  //calculo do novo valor para o contador c/ nova freq
  uint16_t counter = TIMER_FREQ / freq; //se TIMER_FREQ=100Hz e freq=4Hz, então o timer conta 25 clock ticks antes de gerar uma interrupt
  uint8_t LSB, MSB;

  util_get_LSB(counter, &LSB);
  util_get_MSB(counter, &MSB);


  //avisamos que vamos configurar o timer  c/ nova conf
  if (sys_outb(TIMER_CTRL, new_conf) != 0) return 1;

  //injetamos o valor do counter diretamente no timer em questão
  if (sys_outb(selectedTimer, LSB) != 0) return 1;
  if (sys_outb(selectedTimer, MSB) != 0) return 1;


  return 0;
}
