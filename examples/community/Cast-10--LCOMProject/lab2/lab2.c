#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include "i8254.h"
#include <stdbool.h>
#include <stdint.h>

extern uint32_t counter;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t st;

  if (timer > 2) return 1;

  if (timer_get_conf(timer, &st) != 0)
    return 1;

  if (timer_display_conf(timer, st, field) != 0)
    return 1;

  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  return timer_set_frequency(timer, freq);
}

int(timer_test_int)(uint8_t time) {
  int ipc_status, r;
  message msg;
  uint8_t bit_no;

  if (timer_subscribe_int(&bit_no) != 0) return 1;

  uint32_t irq_set = BIT(bit_no);
  counter = 0;

  while (counter < time * sys_hz()) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            timer_int_handler();

            if (counter % sys_hz() == 0) {
              timer_print_elapsed_time();
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;

  return 0;
}
