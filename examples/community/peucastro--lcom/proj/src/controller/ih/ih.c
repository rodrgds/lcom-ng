#include <lcom/lcf.h>

#include "controller/ih/ih.h"

static uint32_t irq_set_timer = 0, irq_set_kbd = 0, irq_set_mouse = 0, irq_set_rtc = 0;
static uint8_t i = 0, bytes[2] = {0, 0};
static struct packet pp;

int(subscribe_interrupts)(void) {
  uint8_t bit_no;

  if (timer_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe timer interrupts.");
    return 1;
  }
  irq_set_timer = BIT(bit_no);

  if (kbd_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe keyboard interrupts.");
    return 1;
  }
  irq_set_kbd = BIT(bit_no);

  if (mouse_write_cmd(MOUSE_EN_DATA_REPORTS) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to enable mouse data reporting.");
    return 1;
  }
  if (mouse_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe mouse interrupts.");
    return 1;
  }
  irq_set_mouse = BIT(bit_no);

  if (rtc_enable_interrupt(RTC_ALARM_INT) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to enable rtc interrupts.");
    return 1;
  }
  if (rtc_set_alarm_every_second() != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to set rtc alarm.");
    return 1;
  }
  if (rtc_subscribe_int(&bit_no) != 0) {
    fprintf(stderr, "subscribe_interrupts: failed to subscribe rtc interrupts.");
    return 1;
  }
  irq_set_rtc = BIT(bit_no);

  return 0;
}

int(unsubscribe_interrupts)(void) {
  if (rtc_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe rtc interrupts.");
    return 1;
  }
  if (rtc_disable_interrupt(RTC_ALARM_INT) != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to disable rtc alarm interrupt.");
    return 1;
  }

  if (mouse_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe mouse interrupts.");
    return 1;
  }
  if (mouse_write_cmd(MOUSE_DIS_DATA_REPORTS) != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to disable mouse data reporting.");
    return 1;
  }

  if (kbd_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe keyboard interrupts.");
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    fprintf(stderr, "unsubscribe_interrupts: failed to unsubscribe timer interrupts.");
    return 1;
  }

  return 0;
}

void(timer_handler)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "timer_handler: game pointer cannot be null.");
    return;
  }

  timer_int_handler();
  draw_next_frame(game);
  vbe_flip_page();

  if (handle_timer_event(game, timer_get_counter()) != 0) {
    fprintf(stderr, "timer_handler: failed to call hanble_timer_event.");
    return;
  }
}

void(kbd_handler)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "kbd_handler: game pointer cannot be null.");
    return;
  }

  kbd_ih();
  bytes[i] = get_scancode();

  if (get_scancode() == CODE_HEADER) {
    i++;
    return;
  }

  i = 0;

  handle_kbd_event(game, translate_scancode(get_scancode()));
}

void(mouse_handler)(Game *game) {
  if (game == NULL) {
    fprintf(stderr, "mouse_handler: game pointer cannot be null.");
    return;
  }

  mouse_ih();
  mouse_sync();

  if (mouse_get_index() == 0) {
    pp = mouse_parse_packet();
    mouse_update_info(pp);

    if (handle_mouse_event(game, mouse_get_info()) != 0) {
      fprintf(stderr, "mouse_handler: failed to call handle_mouse_event.");
      return;
    }
  }
}

void(rtc_handler)(Game *game) {
  rtc_ih();

  if (handle_rtc_event(game) != 0) {
    fprintf(stderr, "rtc_handler: failed to call handle_rtc_event.");
    return;
  }
}

void(process_interrupts)(uint32_t irq_mask, Game *game) {
  if (game == NULL) {
    fprintf(stderr, "process_interrupts: game pointer cannot be null.");
    return;
  }

  if (irq_mask & irq_set_timer) {
    timer_handler(game);
  }
  if (irq_mask & irq_set_kbd) {
    kbd_handler(game);
  }
  if (irq_mask & irq_set_mouse) {
    mouse_handler(game);
  }
  if (irq_mask & irq_set_rtc) {
    rtc_handler(game);
  }
}
