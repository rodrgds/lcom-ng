#include "rtc.h"
#include "util.h"
#include "objects.h"
#include "snake.h"

uint8_t rtc_time[3];
uint8_t rtc_date[3];

int hook_id_rtc = 5;

int rtc_subscribe_int(uint8_t *bit_no) {
  *bit_no = BIT(hook_id_rtc);
  return sys_irqsetpolicy(IRQ_RTC, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_rtc);
}

int rtc_unsubscribe_int() {
  return sys_irqrmpolicy(&hook_id_rtc);
}

int enable() {
  return sys_irqenable(&hook_id_rtc);
}

int disable() {
  return sys_irqdisable(&hook_id_rtc) != 0;
}

int wait_valid_rtc() {
  uint32_t regA = 0;

  do {
    if (disable())return 1;

    if (sys_outb(RTC_ADDR_REG, RTC_A) != 0) {
      return 1;
    }
    if (sys_inb(RTC_DATA_REG, &regA) != 0) {
      return 1;
    }

    if (enable()) return 1;

  } while (regA & UIP);

  return 0;
}

int set_periodic() {
  uint8_t regA;

  if (sys_outb(RTC_ADDR_REG, RTC_A))
    return 1;
  if (util_sys_inb(RTC_DATA_REG, &regA))
    return 1;

  regA = regA | RS3 | RS2 | RS1 | RS0;

  if (sys_outb(RTC_ADDR_REG, RTC_A))
    return 1;
  if (sys_outb(RTC_DATA_REG, regA))
    return 1;

  return 0;
}

int set_update_int(bool on) {
  uint8_t regB;

  if (sys_outb(RTC_ADDR_REG, RTC_B))
    return 1;
  if (util_sys_inb(RTC_DATA_REG, &regB))
    return 1;

  if (on)
    regB = regB | PIE;
  else
    regB = regB & ~PIE;

  if (sys_outb(RTC_ADDR_REG, RTC_B))
    return 1;
  if (sys_outb(RTC_DATA_REG, regB))
    return 1;

  return 0;
}

void rtc_ih() {
  int ret;
  uint8_t regC;
  sys_outb(RTC_ADDR_REG, RTC_C);
  ret = util_sys_inb(RTC_DATA_REG, &regC);
  if (regC & PF)
    dealWithInterrupt=true;
}
