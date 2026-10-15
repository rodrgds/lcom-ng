#include "../drivers.h"

datetime_t curr_time;
int rtc_hook_id = 5;
uint8_t bin_mode = 0;

int (rtc_subscribe_int)(uint8_t *bit_no) {
  uint8_t val = 0;

  if (rtc_read(RTC_REG_C, &val))
    return 1;

  if (bit_no == NULL)
    return 1;

  *bit_no = BIT(rtc_hook_id);

  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id))
    return 1;

  return 0;
}

int(rtc_unsubscribe_int)() {
  if (rtc_set_alarm_int(false))
    return 1;
  if (rtc_set_update_int(false))
    return 1;

  return sys_irqrmpolicy(&rtc_hook_id);
}

uint8_t rtc_to_bin(uint8_t inbcd) {
  uint8_t units = inbcd & 0x0F;
  uint8_t tens = (inbcd >> 4) * 10;

  return units + tens;
}

int rtc_get_time() {
  uint8_t out;

  rtc_wait();

  if (rtc_read(RTC_Y, &out))
    return 1;

  curr_time.year = bin_mode ? out : rtc_to_bin(out);

  if (rtc_read(RTC_M, &out))
    return 1;

  curr_time.month = bin_mode ? out : rtc_to_bin(out);

  if (rtc_read(RTC_D, &out))
    return 1;

  curr_time.day = bin_mode ? out : rtc_to_bin(out);

  if (rtc_read(RTC_H, &out))
    return 1;

  curr_time.hours = bin_mode ? out : rtc_to_bin(out);

  if (rtc_read(RTC_MIN, &out))
    return 1;

  curr_time.minutes = bin_mode ? out : rtc_to_bin(out);

  if (rtc_read(RTC_S, &out))
    return 1;

  curr_time.seconds = bin_mode ? out : rtc_to_bin(out);

  return 0;
}

int rtc_set_alarm() {
  rtc_wait();

  if (rtc_set_update_int(false))
    return 1;

  if (rtc_write(RTC_H_ALARM, RTC_DONT_CARE))
    return 1;
  if (rtc_write(RTC_MIN_ALARM, RTC_DONT_CARE))
    return 1;
  if (rtc_write(RTC_S_ALARM, RTC_DONT_CARE))
    return 1;

  return rtc_set_update_int(true);
}

int rtc_setup() {
  uint8_t ts;

  if (rtc_read(RTC_REG_B, &ts))
    return 1;

  if (RTC_REG_B_IS_BIN & ts)
    bin_mode = 1;

  if (rtc_set_alarm())
    return 1;

  if (rtc_set_periodic_int(false))
    return 1;
  if (rtc_set_alarm_int(false))
    return 1;

  if (rtc_set_alarm_int(true))
    return 1;

  return rtc_get_time();
}

void rtc_wait() {
  uint8_t status;

  if (rtc_read(RTC_REG_A, &status))
    return;

  while (status & RTC_IS_UPDATING) {
    tickdelay(micros_to_ticks(RTC_DELAY));

    if (rtc_read(RTC_REG_A, &status))
      return;
  }
}

int rtc_read(uint8_t cmd, uint8_t *output) {
  if (output == NULL)
    return 1;

  if (sys_outb(RTC_INPUT_REG, cmd))
    return 1;

  return util_sys_inb(RTC_OUTPUT_REG, output);
}

int rtc_write(uint8_t cmd, uint8_t content) {
  if (sys_outb(RTC_INPUT_REG, cmd))
    return 1;

  return sys_outb(RTC_OUTPUT_REG, content);
}

int (rtc_ih)() {
  uint8_t value = 0;

  if (rtc_read(RTC_REG_C, &value))
    return 1;

  if (value & RTC_REG_C_ALARM_FLAG) {
    if (rtc_get_time())
      return 1;
    vg_set_redraw();
  }

  if (value & RTC_REG_C_UPDATE_FLAG) {
    if (rtc_get_time())
      return 1;
    vg_set_redraw();
  }

  return 0;
}

int rtc_set_periodic_int(bool enable) {
  uint8_t val;

  if (rtc_read(RTC_REG_B, &val))
    return 1;

  if (enable)
    val |= RTC_REG_B_PERIODIC;
  else
    val &= ~RTC_REG_B_PERIODIC;

  return rtc_write(RTC_REG_B, val);
}

int rtc_set_update_int(bool enable) {
  uint8_t val;

  if (rtc_read(RTC_REG_B, &val))
    return 1;

  if (enable)
    val &= ~RTC_REG_B_DONT_UPDATE;
  else
    val |= RTC_REG_B_DONT_UPDATE;

  return rtc_write(RTC_REG_B, val);
}

int rtc_set_alarm_int(bool enable) {
  uint8_t val = 0;

  if (rtc_read(RTC_REG_B, &val))
    return 1;

  if (enable)
    val |= RTC_REG_B_ALARM;
  else
    val &= ~RTC_REG_B_ALARM;

  return rtc_write(RTC_REG_B, val);
}
