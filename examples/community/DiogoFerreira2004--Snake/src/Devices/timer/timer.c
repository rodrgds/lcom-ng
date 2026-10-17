#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "./../../snake.h"

int timer_counter = 0;
int timer_hook = 0;


/**
 *@brief Função que subscreve e permite as interrupções do timer 0
 *
 *
 * @param bit_no endereço de memória que vai ser inicializado com o bit do número ativado na máscara 
 * 
 * @return retorna 0 em caso de sucesso
 */
int(timer_subscribe_int)(uint8_t *bit_no) {
  if ( bit_no == NULL ) return 1;

  *bit_no = BIT(timer_hook);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook)!=0) return 1; 
  
  return 0;
}



/**
 *@brief Função que calcela a subescrição do timer 0
 *
 * 
 * @return retorna 0 em caso de sucesso
 */
int(timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook)!=0) return 1;
  return 0;
}


/**
 *@brief IH do timer 0
 *
 * 
 */
void(timer_int_handler)() {
  timer_counter++;
}

