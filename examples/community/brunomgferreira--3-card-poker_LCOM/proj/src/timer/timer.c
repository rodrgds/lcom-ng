#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t timer_counter = 0;
int hook_id = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq)
{
  if (timer < 0 || timer > 2)
    return 1;

  if (freq < 19 || freq > 1193181)
    return 1;

  uint16_t c = 1193181 / freq;
  uint8_t st;

  if (timer_get_conf(timer, &st))
    return 1;

  if (sys_outb(0x43, (st & 0xF) | BIT(4) | BIT(5) | (timer << 6)))
    return 1;

  uint8_t LSB;
  uint8_t MSB;

  if (util_get_LSB(c, &LSB))
    return 1;

  if (util_get_MSB(c, &MSB))
    return 1;

  if (sys_outb(0x40 + timer, LSB))
    return 1;

  if (sys_outb(0x40 + timer, MSB))
    return 1;

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no)
{
  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(hook_id);

  if (sys_irqsetpolicy(0, IRQ_REENABLE, &hook_id))
    return 1;

  return 0;
}

int(timer_unsubscribe_int)()
{
  if (sys_irqrmpolicy(&hook_id))
    return 1;
  return 0;
}

void(timer_int_handler)()
{
  timer_counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st)
{
  if (timer < 0 || timer > 2)
    return 1;

  uint8_t rb = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1);

  if (sys_outb(0x43, rb))
    return 1;

  if (util_sys_inb(0x40 + timer, st))
    return 1;

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field)
{
  union timer_status_field_val val;

  switch (field)
  {
  case tsf_all:
    val.byte = st;
    break;
  case tsf_initial:
    val.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
    break;
  case tsf_mode:
    val.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
    break;
  case tsf_base:
    if ((st & 0x1) == 1)
      val.bcd = true;
    else
      val.bcd = false;
    break;
  default:
    return 1;
  }

  if (timer_print_config(timer, field, val))
    return 1;

  return 0;
}
