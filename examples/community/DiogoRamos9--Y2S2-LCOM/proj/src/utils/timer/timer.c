#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer.h"

#define FREQ 1193182

int timer_hook_id = 0;
int timer_cnt = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > FREQ) return 1;               // Verificar se é inválido

  uint8_t control_word;                                        // Temos de consultar a configuração atual do timer
  if (timer_get_config(timer, &control_word) != 0) return 1;     // Extrair control word para controlWord
  
  control_word |= TIMER_LSB_MSB & 0xC0;                        // Ativar os bits LSB followed by MSB (padrão) e resetar o timer
  control_word |= (timer << 6);                                // Meter o timer

  uint32_t time = FREQ / freq;                          // Calcular valor com a freq fornecida
  uint8_t selected_timer;
  if (!timer) selected_timer = TIMER_0;                        // Extrair o timer conforme o input dado
  else if (timer) selected_timer = TIMER_1;                    // para ser mais fácil depois meter o time
  else if (timer == 2) selected_timer = TIMER_2;               // no selectedTimer
  else return 1;

  uint8_t lsb;                                                // Como dissemos ao controlador que vamos fazer
  uint8_t msb;                                                // com LSB followed by MSB, temos de separá-los
  if (util_get_LSB(time, &lsb) != 0) return 1;                // para depois meter um de cada vez com sys_inb
  if (util_get_MSB(time, &msb) != 0) return 1;                               

  if (sys_outb(TIMER_CTRL, control_word) != 0) return 1;       // Avisar que vamos fazer o que tá na controlWord ao controlador

  if (sys_outb(selected_timer, msb) != 0) return 1;            // Meter no timer os msb do time
  if (sys_outb(selected_timer, lsb) != 0) return 1;            // Meter no timer os lsb do time

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;                               // Verficar se input é válido
  *bit_no = BIT(timer_hook_id);                               // Ativa o bit que indica que o timer gerou uma interrupção                                           // Meter no bit_no para informar quem gerou a interrupção
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) return 1; // Meter as notificações automáticas nas IRQ do timer
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id) != 0) return 1;                     // Unsubscribe a interrupção
  return 0;
}

void (timer_int_handler)() {
  timer_cnt++;
}

int (timer_get_config)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || st == NULL || timer > 2) return 1;         // Verificar se é inválido

  uint8_t rb_cmd = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1); // BIT(7) e BIT(6) -> ativar o read back
                                                              // BIT(5) -> não queremos ler o counter
                                                              // BIT(timer + 1) -> ativar o bit do timer
  if (sys_outb(TIMER_CTRL, rb_cmd) != 0) return 1;            // Avisa o registo de controlo que vamos fazer rb
  if (util_sys_inb(TIMER_0 + timer, st) != 0) return 1;       // Mete a configuração do timer em st
  return 0;                                                   
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val info;                            // Estrutura de dado para guardarmos

  switch (field) {
    case tsf_all:                                               // Quer a configuração -> metemos no data.byte
      info.byte = st;                                           // o status do comando read-back: st
      break;
    
    case tsf_mode:
      st >>= 1;
      st &= 0x07;
      if (st == 6) info.count_mode = 2;
      if (st == 7) info.count_mode = 3;
      else info.count_mode = st;
      break;

    case tsf_base:
      info.bcd = st & 0x01;
      break;

    case tsf_initial:
      st >>= 4;
      st &= (BIT(1) | BIT(0));
      if (!st) info.in_mode = INVAL_val;
      else if (st) info.in_mode = LSB_only;
      else if (st == 2) info.in_mode = MSB_only;
      else if (st == 3) info.in_mode = MSB_after_LSB;
      break;

      default:
        return 1;
  }

  if (timer_print_config(timer, field, info) != 0) return 1;
  return 0;
}

int(timer_test_int)(uint8_t time) {
  int ipc_status;
  uint8_t irq_set;
  message msg;

  if (timer_subscribe_int(&irq_set) != 0) return 1;

  while (time) { 
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed\n");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */                
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            timer_int_handler();
            if (!(timer_cnt%60)) {
              time--;
              timer_print_elapsed_time();
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */    
      }
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;

  return 0;
}
