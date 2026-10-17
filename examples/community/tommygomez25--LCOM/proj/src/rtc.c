#include "rtc.h"

int hook_id_rtc = RTC_IRQ;

uint8_t rtc_time[3]; /* index 0 -> seconds, index 1-> minutes, index 2-> hours */
uint8_t rtc_date[3]; /* index 0 -> day , index 1-> month, index 2-> year */

int(rtc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id_rtc;
  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_rtc)) {
    return 1;
  }

  uint32_t reg;

  if (sys_outb(RTC_ADDR_REG, RTC_B) != 0) {
    return 1;
  } /* indicate that I want to read from Register B */
  if (sys_inb(RTC_DATA_REG, &reg) != 0) {
    return 1;
  } /* read register B */

  reg = reg | RTC_UIE | RTC_DM; /* Update Interrupt Enable */

  if (sys_outb(RTC_ADDR_REG, RTC_B) != 0) {
    return 1;
  } /* indicate that I want to write in Register B */
  if (sys_outb(RTC_DATA_REG, reg) != 0) {
    return 1;
  } /* write updated register B */

  return 0;
}

int(rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_rtc)) {
    return 1;
  }

  uint32_t reg;

  if (sys_outb(RTC_ADDR_REG, RTC_B) != 0) {
    return 1;
  } /* indicate that I want to read from Register B */
  if (sys_inb(RTC_DATA_REG, &reg) != 0) {
    return 1;
  } /* read register B */

  reg = reg & ~RTC_UIE & ~RTC_DM; /* Update Interrupt Enable */

  if (sys_outb(RTC_ADDR_REG, RTC_B) != 0) {
    return 1;
  } /* indicate that I want to write in Register B */
  if (sys_outb(RTC_DATA_REG, reg) != 0) {
    return 1;
  } /* write updated register B */

  return 0;
}

int wait_regA_rtc() {
  uint32_t regA = 0;

  do {
    if (sys_irqdisable(&hook_id_rtc) != 0) {
      return 1;
    } /* need to disable interrupts before checking register A bit 7 */

    if (sys_outb(RTC_ADDR_REG, RTC_A) != 0) {
      return 1;
    }
    if (sys_inb(RTC_DATA_REG, &regA) != 0) {
      return 1;
    }

    if (sys_irqenable(&hook_id_rtc) != 0) {
      return 1;
    }

  } while (regA & RTC_UIP); /* in Status Register A , if the UIP is set to 1, then an update is in progress, which means we cannot access time/date registers */

  return 0;
}

void(rtc_ih)() {
  uint32_t regC;

  sys_outb(RTC_ADDR_REG, RTC_C);
  sys_inb(RTC_DATA_REG, &regC);

  if (regC & RTC_UF) { /* UF is set to 1 if an update occured */
    if (rtc_time[1] == 59 || rtc_time[1] == 0) {
      rtc_read(RTC_HOURS);
    }
    rtc_read(RTC_MINUTES);
    rtc_read(RTC_SECONDS);
    rtc_read(RTC_DAY);
    rtc_read(RTC_MONTH);
    rtc_read(RTC_YEAR);
  }
}

int rtc_read(uint8_t reg) {
  if (wait_regA_rtc() != 0) {
    return 1;
  }

  if (reg != RTC_SECONDS && reg != RTC_MINUTES && reg != RTC_HOURS && reg != RTC_DAY && reg != RTC_MONTH && reg != RTC_YEAR) {
    return 1;
  }

  uint32_t read;
  if (sys_outb(RTC_ADDR_REG, reg) != 0) {
    return 1;
  }
  if (sys_inb(RTC_DATA_REG, &read) != 0) {
    return 1;
  }

  switch (reg) {
    case RTC_SECONDS:
      //rtc_time[0] = convert_from_binary((uint8_t) read);
      rtc_time[0] = read;
            break;
    case RTC_MINUTES:
      rtc_time[1] = read;
      //rtc_time[1] = convert_from_binary((uint8_t) read);
      break;
    case RTC_HOURS:
      rtc_time[2] = read;
      //rtc_time[2] = convert_from_binary((uint8_t) read);
      break;
    case RTC_DAY:
       rtc_date[0] = read;
      //rtc_date[0] = convert_from_binary((uint8_t) read);
      break;
    case RTC_MONTH:
    rtc_date[1] = read;
      //rtc_date[1] = convert_from_binary((uint8_t) read);
      break;
    case RTC_YEAR:
    rtc_date[2] = read;
      //rtc_date[2] = convert_from_binary((uint8_t) read);
      break;
    default:
      return 1;
      break;
  }

  return read;
}


void print_clock() {
  printf("%d:%d:%d\n", rtc_time[2], rtc_time[1], rtc_time[0]);
  printf("%d/%d/20%d\n\n", rtc_date[0], rtc_date[1], rtc_date[2]);
}
