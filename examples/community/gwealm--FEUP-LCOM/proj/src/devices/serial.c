#include "devices/serial.h"

#include "events/events.h"

#include "game/model/local_player.h"

static int irq_set;
static int hook_id;

static bool can_send;
static queue *out;
static queue *in;

int (serial_init)() {
  if (serial_subscribe_interrupts() != 0) {
    printf("serial_init: could not subscribe to interrupts\n");
    return 1;
  }

  if(serial_set_baudrate()) {
    printf("serial_init: could not set baudrate\n");
    return 1;
  }

  if (serial_set_line_pattern(NO_PARITY, TWO, EIGHT) != OKAY) {
    printf("serial_init: could not set line format\n");
    return 1;
  }

  if (serial_enable_fifo() != OKAY) {
    printf("serial_init: could not enable fifo\n");
    return 1;
  }

  serial_interrupts interrupts;
  interrupts.set.received_data_available = true;
  interrupts.set.receiver_line_status = true;
  interrupts.set.transmitter_holding_register_empty = true;

  if (serial_set_interrupts(&interrupts) != OKAY) {
    printf("serial_init: could not set interrupts\n");
    return 1;
  }

  out = queue_create(10 * SERIAL_PACKET_SIZE, sizeof(uint8_t));
  in = queue_create(SERIAL_PACKET_SIZE, sizeof(uint8_t));
  can_send = true;

  event_on(INTERRUPT, serial_handle_interrupt);

  return OKAY;
}

int (serial_cleanup)() {
  event_unregister(INTERRUPT, serial_handle_interrupt);

  queue_destroy(out);
  queue_destroy(in);

  return OKAY;
}

int (serial_set_baudrate)() {
  if(sys_outb(SERIAL_COM1_LCR, SERIAL_LCR_ENABLE_DLAB)) {
    printf("serial_set_baudrate: could not set dlab bit\n");
    return 1;
  }

  if(sys_outb(SERIAL_COM1_DLAB_LOW_BYTE, 0x00)) {
    printf("serial_set_baurate: could not set dlab low byte\n");
    return 1;
  }

  if(sys_outb(SERIAL_COM1_DLAB_HIGH_BYTE, 0x09)) {
    printf("serial_set_baudrate: could not set dlab high byte\n");
    return 1;
  }

  if(sys_outb(SERIAL_COM1_LCR, SERIAL_LCR_DISABLE_DLAB)) {
    printf("serial_set_baudrate: could not disable dlab\n");
    return 1;
  }

  return OKAY;
}

int (serial_subscribe_interrupts)() {
  hook_id = SERIAL_IRQ; irq_set = BIT(hook_id);
  return sys_irqsetpolicy(SERIAL_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (serial_unsubscribe_interrupts)() {
  return sys_irqrmpolicy(&hook_id);
}

void (serial_handle_interrupt)(struct event *ev) {
  if (*((int*) ev->data) & irq_set) {
    serial_interrupts interrupts;
    while (true) {
      if (serial_get_interrupts(&interrupts) != OKAY) {
        printf("serial_handle_interrupt: could not get interrupt identification\n");
        return;
      }

      if (!interrupts.get.interrupt_pending) {
        return;
      }

      switch(interrupts.get.interrupt) {
        case RECEIVER_LINE_STATUS:
          serial_handle_line_status();
          return;

        case RECEIVED_DATA_AVAILABLE:
          serial_handle_received_data();
          return;

        case TRANSMITTER_HOLDING_REGISTER_EMPTY:
          serial_handle_transmitter_holding_register_empty();
          return;

        default:
          return;
      }
    }
  }
}

int (serial_get_interrupts)(serial_interrupts *interrupts) {
  uint8_t word;
  if (util_sys_inb(SERIAL_COM1_IIR, &word) != 0) {
    printf("serial_get_interrupts: could not read IIR\n");
    return 1;
  }

  uint8_t interrupt = (word & SERIAL_IIR_INT) >> 1;

  interrupts->get.interrupt = interrupt;
  interrupts->get.interrupt_pending = !(word & SERIAL_IIR_INT_PENDING);

  return OKAY;
}

int (serial_set_interrupts)(const serial_interrupts *interrupts) {
  uint8_t word = 0;
  
  if (interrupts->set.modem_status) {
    word |= SERIAL_IER_MODEM_STATUS;
  }

  if (interrupts->set.receiver_line_status) {
    word |= SERIAL_IER_RECEIVER_LINE_STATUS;
  }

  if (interrupts->set.transmitter_holding_register_empty) {
    word |= SERIAL_IER_TRANSMITTER_HOLDING_REGISTER_EMPTY;
  }

  if (interrupts->set.received_data_available) {
    word |= SERIAL_IER_RECEIVED_DATA_AVAILABLE;
  }

  if (sys_outb(SERIAL_COM1_IER, word) != 0) {
    printf("serial_set_interrupts: could not write to IER\n");
    return 1;
  }

  return OKAY;
}

int (serial_set_line_pattern)(serial_parity parity, serial_stop_bits stop_bits, serial_word_length word_length) {
  uint8_t word = 0;

  word |= parity << 3;
  word |= stop_bits << 2;
  word |= word_length;

  if (sys_outb(SERIAL_COM1_LCR, word) != 0) {
    printf("serial_set_line_pattern: could not write to LCR\n");
    return 1;
  }

  return OKAY;
}

int (serial_enable_fifo)() {
  uint8_t word = 0;

  word |= SERIAL_FCR_ENABLE_FIFO;
  word |= SERIAL_FCR_CLEAR_RECEIVE;
  word |= SERIAL_FCR_CLEAR_TRANSMIT;

  if (sys_outb(SERIAL_COM1_FCR, word) != 0) {
    printf("serial_enable_fifo: could not write to FCR\n");
    return 1;
  }

  return OKAY;
}

void (serial_handle_line_status)() {
  uint8_t word = 0;
  if (util_sys_inb(SERIAL_COM1_LSR, &word) != 0) {
    printf("serial_handle_errors: could not read LSR\n");
    return;
  }

  if (word & SERIAL_LSR_FIFO_ERROR) {
    printf("serial_handle_line_status: fifo error\n");
  }

  if (word & SERIAL_LSR_EMPTY_DATA_HOLDING_REGISTERS) {
    printf("serial_handle_line_status: empty data holding registers\n");
  }

  if (word & SERIAL_LSR_EMPTY_TRANSMITTER_HOLDING_REGISTER) {
    printf("serial_handle_line_status: empty transmiiter holding register\n");
  }

  if (word & SERIAL_LSR_BREAK_INTERRUPT) {
    printf("serial_handle_line_status: break interrupt\n");
  }

  if (word & SERIAL_LSR_FRAMING_ERROR) {
    printf("serial_handle_line_status: framing error\n");
  }

  if (word & SERIAL_LSR_PARITY_ERROR) {
    printf("serial_handle_line_status: parity error\n");
  }

  if (word & SERIAL_LSR_OVERRUN_ERROR) {
    printf("serial_handle_line_status: overrun error\n");
  }

  if (word & SERIAL_LSR_DATA_READY) {
    printf("serial_handle_line_status: data ready\n");
  }
}

void (serial_handle_received_data)() {
  uint8_t data;
  if (util_sys_inb(SERIAL_COM1_RBR, &data) != 0) {
    printf("serial_handle_received_data: failed to read data from register\n");
    return;
  }

  queue_push(in, &data);
  if (is_full(in)) {
    serial_packet packet;

    uint8_t type = *(uint8_t*) queue_pop(in);
    packet.type = type;

    for (size_t i = 0; i < SERIAL_PACKET_SIZE - 1; i++) {
      uint8_t byte = *(uint8_t*) queue_pop(in);
      packet.content[i] = byte;
    }

    event_dispatch(SERIAL_PACKET, &packet, sizeof(serial_packet));
  }
}

void (serial_handle_transmitter_holding_register_empty)() {
  if (is_empty(out)) {
    can_send = true;
    return;
  }

  uint8_t byte = *(uint8_t*) queue_pop(out);
  if (sys_outb(SERIAL_COM1_THR, byte) != 0) {
    printf("serial_handle_transmitter_holding_register_empty: could not write to THR\n");
  }
}

void (serial_send_byte)(uint8_t byte) {
  if (!can_send) {
    queue_push(out, &byte);
    return;
  }

  if (sys_outb(SERIAL_COM1_THR, byte) != 0) {
    printf("serial_send_byte: could not write to THR\n");
    return;
  }

  can_send = false;
}

void (serial_send_packet)(serial_packet *packet) {
  serial_send_byte(packet->type);
  for (size_t i = 0; i < sizeof(packet->content); i++) {
    serial_send_byte(packet->content[i]);
  }
}
