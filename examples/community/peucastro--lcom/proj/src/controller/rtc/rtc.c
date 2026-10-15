#include <lcom/lcf.h>

#include "controller/rtc/rtc.h"

static int rtc_hook_id = 13;
static bool rtc_binary_mode = false;
static bool rtc_24h_mode = false;

static int(rtc_read_register)(uint8_t reg, uint8_t *value);
static int(rtc_write_register)(uint8_t reg, uint8_t value);
static int(rtc_update_register_bit)(uint8_t reg, uint8_t bit_mask, bool set);
static int(rtc_read_config)(void);
static bool(rtc_is_updating)(void);

int(rtc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    fprintf(stderr, "rtc_subscribe_int: bit_no is NULL\n");
    return 1;
  }

  uint8_t reg_c;
  if (rtc_read_register(RTC_ADDR_C, &reg_c) != 0) {
    fprintf(stderr, "rtc_subscribe_int: failed to clear pending interrupts\n");
    return 1;
  }

  *bit_no = rtc_hook_id;
  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &rtc_hook_id) != 0) {
    fprintf(stderr, "rtc_subscribe_int: failed to subscribe to RTC interrupts\n");
    return 1;
  }

  if (rtc_read_config() != 0) {
    fprintf(stderr, "rtc_subscribe_int: failed to read RTC configuration\n");
    rtc_unsubscribe_int();
    return 1;
  }

  return 0;
}

int(rtc_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&rtc_hook_id) != 0) {
    fprintf(stderr, "rtc_unsubscribe_int: failed to unsubscribe from RTC interrupts\n");
    return 1;
  }
  return 0;
}

void(rtc_ih)(void) {
  uint8_t reg_c;
  rtc_read_register(RTC_ADDR_C, &reg_c);
}

int(rtc_enable_interrupt)(rtc_interrupt_type_t interrupt_type) {
  uint8_t bit_mask;

  switch (interrupt_type) {
    case RTC_UPDATE_INT:
      bit_mask = RTC_REG_B_UPDATE_IE;
      break;
    case RTC_ALARM_INT:
      bit_mask = RTC_REG_B_ALARM_IE;
      break;
    case RTC_PERIODIC_INT:
      bit_mask = RTC_REG_B_PERIODIC_IE;
      break;
    default:
      fprintf(stderr, "rtc_enable_interrupt: invalid interrupt type\n");
      return 1;
  }

  return rtc_update_register_bit(RTC_ADDR_B, bit_mask, true);
}

int(rtc_disable_interrupt)(rtc_interrupt_type_t interrupt_type) {
  uint8_t bit_mask;

  switch (interrupt_type) {
    case RTC_UPDATE_INT:
      bit_mask = RTC_REG_B_UPDATE_IE;
      break;
    case RTC_ALARM_INT:
      bit_mask = RTC_REG_B_ALARM_IE;
      break;
    case RTC_PERIODIC_INT:
      bit_mask = RTC_REG_B_PERIODIC_IE;
      break;
    default:
      fprintf(stderr, "rtc_disable_interrupt: invalid interrupt type\n");
      return 1;
  }

  return rtc_update_register_bit(RTC_ADDR_B, bit_mask, false);
}

int(rtc_disable_all_interrupts)(void) {
  uint8_t reg_b;

  if (rtc_read_register(RTC_ADDR_B, &reg_b) != 0) {
    fprintf(stderr, "rtc_disable_all_interrupts: failed to read register B\n");
    return 1;
  }

  reg_b &= ~(RTC_REG_B_UPDATE_IE | RTC_REG_B_ALARM_IE | RTC_REG_B_PERIODIC_IE);

  if (rtc_write_register(RTC_ADDR_B, reg_b) != 0) {
    fprintf(stderr, "rtc_disable_all_interrupts: failed to write register B\n");
    return 1;
  }

  return 0;
}

int(rtc_set_periodic_rate)(uint8_t rate) {
  if (rate > 0x0F) {
    fprintf(stderr, "rtc_set_periodic_rate: invalid rate\n");
    return 1;
  }

  uint8_t reg_a;

  if (rtc_read_register(RTC_ADDR_A, &reg_a) != 0) {
    fprintf(stderr, "rtc_set_periodic_rate: failed to read register A\n");
    return 1;
  }

  reg_a &= 0xF0;
  reg_a |= rate;

  if (rtc_write_register(RTC_ADDR_A, reg_a) != 0) {
    fprintf(stderr, "rtc_set_periodic_rate: failed to write register A\n");
    return 1;
  }

  return 0;
}

int(rtc_read_time)(rtc_time_t *time) {
  if (time == NULL) {
    fprintf(stderr, "rtc_read_time: time is NULL\n");
    return 1;
  }

  if (rtc_wait_update_complete() != 0) {
    fprintf(stderr, "rtc_read_time: failed to wait for update completion\n");
    return 1;
  }

  if (rtc_read_register(RTC_ADDR_SECONDS, &time->seconds) != 0 ||
      rtc_read_register(RTC_ADDR_MINUTES, &time->minutes) != 0 ||
      rtc_read_register(RTC_ADDR_HOUR, &time->hours) != 0 ||
      rtc_read_register(RTC_ADDR_DAY_OF_MONTH, &time->day) != 0 ||
      rtc_read_register(RTC_ADDR_MONTH, &time->month) != 0 ||
      rtc_read_register(RTC_ADDR_YEAR, &time->year) != 0) {
    fprintf(stderr, "rtc_read_time: failed to read time registers\n");
    return 1;
  }

  if (!rtc_binary_mode) {
    time->seconds = rtc_bcd_to_binary(time->seconds);
    time->minutes = rtc_bcd_to_binary(time->minutes);
    time->hours = rtc_bcd_to_binary(time->hours);
    time->day = rtc_bcd_to_binary(time->day);
    time->month = rtc_bcd_to_binary(time->month);
    time->year = rtc_bcd_to_binary(time->year);
  }

  return 0;
}

int(rtc_set_alarm)(const rtc_time_t *alarm_time) {
  if (alarm_time == NULL) {
    fprintf(stderr, "rtc_set_alarm: alarm_time is NULL\n");
    return 1;
  }

  uint8_t seconds = alarm_time->seconds;
  uint8_t minutes = alarm_time->minutes;
  uint8_t hours = alarm_time->hours;

  if (!rtc_binary_mode) {
    seconds = rtc_binary_to_bcd(seconds);
    minutes = rtc_binary_to_bcd(minutes);
    hours = rtc_binary_to_bcd(hours);
  }

  if (rtc_update_register_bit(RTC_ADDR_B, RTC_REG_B_INHIBIT_UPDATES, true) != 0) {
    fprintf(stderr, "rtc_set_alarm: failed to disable updates\n");
    return 1;
  }

  int result = 0;
  if (rtc_write_register(RTC_ADDR_SEC_ALARM, seconds) != 0 ||
      rtc_write_register(RTC_ADDR_MIN_ALARM, minutes) != 0 ||
      rtc_write_register(RTC_ADDR_HOUR_ALARM, hours) != 0) {
    fprintf(stderr, "rtc_set_alarm: failed to set alarm registers\n");
    result = 1;
  }

  if (rtc_update_register_bit(RTC_ADDR_B, RTC_REG_B_INHIBIT_UPDATES, false) != 0) {
    fprintf(stderr, "rtc_set_alarm: failed to re-enable updates\n");
    result = 1;
  }

  return result;
}

int(rtc_set_alarm_every_second)(void) {
  if (rtc_update_register_bit(RTC_ADDR_B, RTC_REG_B_INHIBIT_UPDATES, true) != 0) {
    fprintf(stderr, "rtc_set_alarm_every_second: failed to disable updates\n");
    return 1;
  }

  int result = 0;
  if (rtc_write_register(RTC_ADDR_SEC_ALARM, RTC_DONT_CARE_VALUE) != 0 ||
      rtc_write_register(RTC_ADDR_MIN_ALARM, RTC_DONT_CARE_VALUE) != 0 ||
      rtc_write_register(RTC_ADDR_HOUR_ALARM, RTC_DONT_CARE_VALUE) != 0) {
    fprintf(stderr, "rtc_set_alarm_every_second: failed to set alarm registers\n");
    result = 1;
  }

  if (rtc_update_register_bit(RTC_ADDR_B, RTC_REG_B_INHIBIT_UPDATES, false) != 0) {
    fprintf(stderr, "rtc_set_alarm_every_second: failed to re-enable updates\n");
    result = 1;
  }

  return result;
}

bool(rtc_is_valid)(void) {
  uint8_t reg_d;

  if (rtc_read_register(RTC_ADDR_D, &reg_d) != 0) {
    return false;
  }

  return (reg_d & RTC_REG_D_VALID_RT) != 0;
}

int(rtc_wait_update_complete)(void) {
  int timeout = 1000;

  while (rtc_is_updating() && timeout > 0) {
    tickdelay(micros_to_ticks(1000));
    timeout--;
  }

  if (timeout == 0) {
    fprintf(stderr, "rtc_wait_update_complete: timeout waiting for update completion\n");
    return 1;
  }

  return 0;
}

int(rtc_get_interrupt_type)(void) {
  uint8_t reg_c;

  if (rtc_read_register(RTC_ADDR_C, &reg_c) != 0) {
    return 1;
  }

  if (reg_c & RTC_REG_C_UF) {
    return RTC_UPDATE_INT;
  }
  if (reg_c & RTC_REG_C_AF) {
    return RTC_ALARM_INT;
  }
  if (reg_c & RTC_REG_C_PF) {
    return RTC_PERIODIC_INT;
  }

  return 1;
}

int(rtc_read_register_value)(uint8_t reg, uint8_t *value) {
  return rtc_read_register(reg, value);
}

uint8_t(rtc_bcd_to_binary)(uint8_t bcd_value) {
  return ((bcd_value >> 4) * 10) + (bcd_value & 0x0F);
}

uint8_t(rtc_binary_to_bcd)(uint8_t binary_value) {
  return ((binary_value / 10) << 4) | (binary_value % 10);
}

static int(rtc_read_register)(uint8_t reg, uint8_t *value) {
  if (value == NULL) {
    fprintf(stderr, "rtc_read_register: value is NULL\n");
    return 1;
  }

  if (sys_outb(RTC_ADDR_REG, reg) != 0) {
    fprintf(stderr, "rtc_read_register: failed to select register %d\n", reg);
    return 1;
  }

  if (util_sys_inb(RTC_DATA_REG, value) != 0) {
    fprintf(stderr, "rtc_read_register: failed to read from register %d\n", reg);
    return 1;
  }

  return 0;
}

static int(rtc_write_register)(uint8_t reg, uint8_t value) {
  if (sys_outb(RTC_ADDR_REG, reg) != 0) {
    fprintf(stderr, "rtc_write_register: failed to select register %d\n", reg);
    return 1;
  }

  if (sys_outb(RTC_DATA_REG, value) != 0) {
    fprintf(stderr, "rtc_write_register: failed to write to register %d\n", reg);
    return 1;
  }

  return 0;
}

static int(rtc_update_register_bit)(uint8_t reg, uint8_t bit_mask, bool set) {
  uint8_t reg_value;

  if (rtc_read_register(reg, &reg_value) != 0) {
    fprintf(stderr, "rtc_update_register_bit: failed to read register %d\n", reg);
    return 1;
  }

  if (set) {
    reg_value |= bit_mask;
  }
  else {
    reg_value &= ~bit_mask;
  }

  if (rtc_write_register(reg, reg_value) != 0) {
    fprintf(stderr, "rtc_update_register_bit: failed to write register %d\n", reg);
    return 1;
  }

  return 0;
}

static int(rtc_read_config)(void) {
  uint8_t reg_b;

  if (rtc_read_register(RTC_ADDR_B, &reg_b) != 0) {
    fprintf(stderr, "rtc_read_config: failed to read register B\n");
    return 1;
  }

  rtc_binary_mode = (reg_b & RTC_REG_B_DM_BINARY) != 0;
  rtc_24h_mode = (reg_b & RTC_REG_B_24_12) != 0;

  return 0;
}

static bool(rtc_is_updating)(void) {
  uint8_t reg_a;

  if (rtc_read_register(RTC_ADDR_A, &reg_a) != 0) {
    return false;
  }

  return (reg_a & RTC_REG_A_UPDATE_IP) != 0;
}
