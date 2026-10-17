#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer.h"

uint32_t time_counter;
static int32_t irq_hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    if (timer < 0 || timer > 2)
    {
        printf("Timer number ranges from 0 to 2\n");
        return 1;
    }

    uint8_t conf;
    if (timer_get_conf(timer, &conf) != 0)
    {
        printf("timer_get_conf::Error geting configuration\n");
        return 2;
    }

    // mask: 0x0F 0b00001111 to preserve last 4 LSB's;
    uint8_t controlWord = TIMER_LSB_MSB | (conf & 0x0F);

    switch (timer)
    {
        case 0:
            controlWord |= TIMER_SEL0;
            break;
        case 1:
            controlWord |= TIMER_SEL1;
            break;
        case 2:
            controlWord |= TIMER_SEL2;
            break;
    }

    // controlWord -> control register (sys_outb)
    if (sys_outb(TIMER_CTRL, controlWord) != 0)
    {
        printf("Error writing control word to timer control register\n");
        return 3;
    }

    // basicamente mudar a frequencia do timer
    // escrever primeiro LSB, depois MSB porque o timer não consegue receber os dois ao mesmo tempo

    // MSB - most significant BYTE***

    uint32_t initVal = (uint32_t) TIMER_FREQ / freq;
    uint8_t initVal_LSB = (uint8_t) initVal;
    uint8_t initVal_MSB = (uint8_t) (initVal >> 8);

    uint8_t timerPort = timer + TIMER_0;
    // load LSB
    if (sys_outb(timerPort, initVal_LSB) != 0)
    {
        printf("Error writing LSB\n");
        return 4;
    }
    // load MSB
    if (sys_outb(timerPort, initVal_MSB) != 0)
    {
        printf("Error writing MSB\n");
        return 5;
    }
    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no){
    (*bit_no) = BIT(irq_hook_id);
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &irq_hook_id) != OK)
    {
        printf("Error::sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &irq_hook_id)\n");
        return 1;
    }
    return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&irq_hook_id) == OK)
      return 0;
  return 1;
}

void (timer_int_handler)() {
    time_counter++; // cannot overflow or it will start again from 0
}

// working !!
int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint32_t readBCommand;    // palavra de 32bits para Read-Back Command
  uint32_t conf;            // conf onde será armazenado o Status Byte lido do timer em questão

  readBCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);  // 0b1101....
  if (sys_outb(TIMER_CTRL, readBCommand) != 0)  // escreve no control register o readbcommand
  {
      printf("sys_outb::error\n");
      return 1;
  }

  if (timer == 0)
  {
      if (sys_inb(TIMER_0, &conf) != 0)         // escreve em conf o status byte lido do TIMER_0
      {
          printf("sys_inb::error\n");
          return 2;
      }
  }
  else if (timer == 1)
  {
      if (sys_inb(TIMER_1, &conf) != 0)
      {
          printf("sys_inb::error\n");
          return 2;
      }
  }
  else if (timer == 2)
  {
      if (sys_inb(TIMER_2, &conf) != 0)
      {
          printf("sys_inb::error\n");
          return 2;
      }
  }
  else {
      printf("error in timer number\n");
      return 3;
  }
  *st = (uint8_t) conf;         // cast para 8bits do status byte
  return 0;
}


int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field){

      union timer_status_field_val conf;    // ver definição de UNION

      if (field == tsf_all)
      {
          conf.byte = st;                   // apresentar o status byte

          timer_print_config(timer, field, conf);
      }
      if (field == tsf_initial)
      {
          char counterInit = (0x30 & st) >> 4;  // mascara para o modo de inicialização
          enum timer_init initTimer;
          switch (counterInit)
          {
              case 1:
                  initTimer = LSB_only;
                  break;
              case 2:
                  initTimer = MSB_only;
                  break;
              case 3:
                  initTimer = MSB_after_LSB;
                  break;
              case 0:
                  initTimer = INVAL_val;
                  break;
          }
          conf.in_mode = initTimer;

          timer_print_config(timer, field, conf);
      }
      if (field == tsf_mode)
      {
          uint8_t progMode = (0x0E & st) >> 1;      // mascara para o modo de contagem
          conf.count_mode = progMode;

          timer_print_config(timer, field, conf);
      }
      if (field == tsf_base)
      {
          char bcd = st & 0x01;                     // mascara para a base de contagem
          if (bcd == 1)
              conf.bcd = 1;
          else
              conf.bcd = 0;

          timer_print_config(timer, field, conf);
      }
      return 0;
}
