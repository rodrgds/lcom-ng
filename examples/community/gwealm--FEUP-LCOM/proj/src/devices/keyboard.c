#include "devices/keyboard.h"
#include "devices/kbc.h"

static int hook_id;
static int irq_set;

static bool is_long_code;

static uint8_t translation_ranges[][2] = {
    { ESC, ZERO },
    { BACKSPACE, BACKSPACE },
    { Q, P },
    { ENTER, ENTER },
    { A, L },
    { Z, DOT },
    { NUM_SEVEN, NUM_NINE },
    { NUM_FOUR, NUM_SIX },
    { NUM_ONE, NUM_DOT }
};

static char translation_values[] = {
    '\x1B', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '\b',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '\n',
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',

    '7', '8', '9',
    '4', '5', '6',
    '1', '2', '3',
    '0', '.'
};

int (kbd_init)() {
  if (kbd_subscribe_int() != 0) {
    printf("kbd_setup: could not subscribe to interrupts\n");
    return KBD_EINT;
  }

  event_on(INTERRUPT, kbd_handle_interrupt);
  
  return OKAY;
}

int (kbd_cleanup)() {
  if (kbd_unsubscribe_int() != 0) {
    printf("kbd_cleanup: could not unsubscribe interrupts\n");
    return KBD_EINT;
  }

  event_unregister(INTERRUPT, kbd_handle_interrupt);

  return OKAY;
}

int (kbd_subscribe_int)() {
  hook_id = KBD_IRQ; irq_set = BIT(hook_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (kbd_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

char (kbd_map_code_to_char)(uint8_t code) {
  size_t idx = 0;

  size_t num_ranges = sizeof(translation_ranges) / sizeof(*translation_ranges);
  for (size_t i = 0; i < num_ranges; i++) {
    uint8_t first = translation_ranges[i][0];
    uint8_t second = translation_ranges[i][1];

    if (code >= first && code <= second) {
      idx += code - first;
      break;
    }

    idx += second - first + 1;
  }

  size_t num_values = sizeof(translation_values) / sizeof(*translation_values);
  return idx < num_values ? translation_values[idx] : '\0';
}

void (kbd_handle_interrupt)(struct event *event) {
  if (*((int*) event->data) & irq_set) {

    uint8_t byte;
    if (kbc_read_byte(KBC_REG_OUT_BUF, &byte) != OKAY) {
      printf("kbd_handle_interrupt: error reading byte from output buffer\n");
      return;
    }

    struct kbc_status stat;
    if (kbc_read_status(&stat) != OKAY) {
      printf("kbd_handle_interrupt: error reading status register\n");
      return;
    }

    if (stat.parity_error || stat.timeout_error) {
      printf("kbd_handle_interrupt: parity or timeout error\n");
      return;
    }

    kbd_handle_byte(byte);
  }
}

void (kbd_handle_byte)(uint8_t code) {
  if(code == KBD_LONG_CODE) {
    is_long_code = true;
    return;
  }

  if (is_long_code && code != ENTER) {
    is_long_code = false;
    return;
  }

  is_long_code = false;
  if (!(code & KBD_BREAK_CODE)) {
    char character = kbd_map_code_to_char(code);

    if (character != '\0') {
      event_dispatch(KEY_PRESS, &character, sizeof(character));
    }
  }
}
