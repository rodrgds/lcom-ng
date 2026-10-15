#include "mouse.h"
#include <lcom/lab4.h>
#include <lcom/lcf.h>

static int timer_hook_id = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = (uint8_t) timer_hook_id;
  return sys_irqsetpolicy(0, IRQ_REENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)() {
}

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of zprintf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status, r;
  message msg;
  uint8_t bit_no;

  if (mouse_subscribe_int(&bit_no) != 0)
    return 1;

  mouse_disable_int();
  my_mouse_enable_data_reporting();
  mouse_enable_int();

  uint32_t iqr_set = BIT(bit_no);

  while (cnt > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & iqr_set) {
          mouse_ih();

          if (byte_tracking == 3) {
            struct packet p;
            mouse_parse_packet(&p);
            mouse_print_packet(&p);

            byte_tracking = 0;
            cnt--;
          }
        }
        break;
      default:
        break;
      }
    }
  }
  mouse_disable_int();
  my_mouse_disable_data_reporting();
  mouse_enable_int();
  mouse_unsubscribe_int();
  return 0;
}


int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status, r;
  message msg;
  uint8_t mouse_bit_no;
  uint8_t timer_bit_no;

  if (mouse_subscribe_int(&mouse_bit_no) != 0) return 1;
  if (timer_subscribe_int(&timer_bit_no) != 0) return 1;


  mouse_disable_int();
  if (my_mouse_enable_data_reporting() != 0) return 1;
  mouse_enable_int();

  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);


  uint32_t freq = sys_hz();
  uint32_t ticks = 0;


  while (ticks < idle_time * freq) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
      
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler(); 
          ticks++;            
        }

        if (msg.m_notify.interrupts & mouse_irq_set) {
          mouse_ih();

          if (byte_tracking == 3) {
            struct packet p;
            mouse_parse_packet(&p);
            mouse_print_packet(&p);

            byte_tracking = 0;
            ticks = 0; 
          }
        }
        break;
      default:
        break;
      }
    }
  }
  
  mouse_disable_int();
  my_mouse_disable_data_reporting();
  mouse_enable_int();

  timer_unsubscribe_int();
  mouse_unsubscribe_int();

  return 0;
}