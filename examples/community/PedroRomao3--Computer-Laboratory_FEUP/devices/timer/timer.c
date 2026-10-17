#include "timer.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // verificar se timer 0 1 2
  if(timer < 0 || timer > 2) return 1;
  // freq tem que ser > 18 e < que TIMER_FREQ
  if(freq < 19 || freq > TIMER_FREQ) return 1;
  // counter o Freq de minix / freq que nos queremos
  uint16_t counter = TIMER_FREQ / freq;
  // partimos o valor de freq para 2 de 8 bits, porque podemos escrever por 8 bits, mas o valor de freq 16 bits
  uint8_t lsb,msb;
  util_get_LSB(counter, &lsb);
  util_get_MSB(counter, &msb);
  // lemos configuracao
  uint8_t config;
  timer_get_conf(timer, &config);
  // apagamos primeros 4 bits (counter celection e de counter init)
  config = config & 0xF;
  // os 4 bits ultimos + init LSB e depois MSB + e espicificamos o timer
  config = config | TIMER_LSB_MSB | BIT(timer + 5); 
  // escrevemos control word para 0x43, com configuracao
  sys_outb(TIMER_CTRL, config);
  // depende de switch escrevemos para timer escolhido 1 2 3 o valor de initializacao de contador 
  // lsb e msb que junto = TIMER_FREQ / freq que queremos
  switch (timer) {
  case 0:
    sys_outb(TIMER_0, lsb);
    sys_outb(TIMER_0, msb);
    break;
  case 1:
    sys_outb(TIMER_1, lsb);
    sys_outb(TIMER_1, msb);
    break;
  case 2:
    sys_outb(TIMER_2, lsb);
    sys_outb(TIMER_2, msb);
    break;
  default:
    break;
  }
  return 0;
}


int timer_hook_id = 0;
int counter = 0;

int(timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(timer_hook_id);

  if (sys_irqsetpolicy(0, IRQ_REENABLE, &timer_hook_id)) return 1;
  return 0;
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void(timer_ih)() {
  counter++;
  if (counter % GAME_FREQUENCY == 0) {
    add_second();
  }
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer < 0 || timer > 2 || st == NULL) return 1;
  // madamos para control word 0x43 com comand word + status + timer celecionado
  if (sys_outb(TIMER_CTRL, TIMER_RB_CMD | BIT(5) | TIMER_RB_SEL(timer))) return 1;
  // lemos status depende de timer escolhido
  int r = 0;
  switch (timer) {
    case 0:
      r = util_sys_inb(TIMER_0, st);
      break;

    case 1:
      r = util_sys_inb(TIMER_1, st);
      break;

    case 2:
      r = util_sys_inb(TIMER_2, st);
      break;

    default:
      break;
  }

  if (r) return 1;
  return 0;
}
