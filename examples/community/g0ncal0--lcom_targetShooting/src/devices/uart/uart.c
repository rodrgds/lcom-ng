#include <lcom/lcf.h>

#include "uart.h"
#include <stdint.h>

typedef enum {
  SLEEPING,
  WAITING,
  PLAYER2_INFO,
} ser_info_t;

unsigned short base_addr;
int ser_hook_id;

static queue_t *tx_queue = NULL;
static queue_t *rx_queue = NULL;

queue_t *(ser_get_tx_queue) () {
  return tx_queue;
}

queue_t *(ser_get_rx_queue) () {
  return rx_queue;
}

int(ser_conf)(unsigned short addr, conf_t config) {
  switch (addr) {
    case SER_COM1:
      base_addr = addr;
      ser_hook_id = SER_COM1_IRQ;
      break;
    case SER_COM2:
      base_addr = addr;
      ser_hook_id = SER_COM2_IRQ;
      break;
    default:
      printf("Invalid base address inside %s\n", __func__);
      return EXIT_FAILURE;
  }

  tx_queue = create_queue(QUEUE_SIZE, sizeof(uint8_t));
  rx_queue = create_queue(QUEUE_SIZE, sizeof(uint8_t));
  if (tx_queue == NULL || rx_queue == NULL) {
    printf("Error initializing queues inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (ser_set_bit_rate(config.bit_rate) != 0) {
    printf("Error in ser_set_bit_rate inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_set_line_ctrl(&config) != 0) {
    printf("Error in ser_set_line_ctrl inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_enable_fifo() != 0) {
    printf("Error in ser_enable_fifo inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_enable_int() != 0) {
    printf("Error in ser_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("Null pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  *bit_no = ser_hook_id;
  if (sys_irqsetpolicy(ser_hook_id, IRQ_REENABLE | IRQ_EXCLUSIVE, &ser_hook_id) != EXIT_SUCCESS) {
    printf("Error in sys_irqsetpolicy inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_unsubscribe_int)() {
  if (sys_irqrmpolicy(&ser_hook_id) != EXIT_SUCCESS) {
    printf("Error in sys_irqrmpolicy inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_write_reg)(uint8_t reg, uint8_t data) {
  if (sys_outb(base_addr + reg, data) != EXIT_SUCCESS) {
    printf("Error in sys_outb inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_read_reg)(uint8_t reg, uint8_t *data) {
  if (util_sys_inb(base_addr + reg, data) != EXIT_SUCCESS) {
    printf("Error in util_sys_inb inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_set_bit_rate)(unsigned int bit_rate) {
  uint8_t lcr, lsb, msb;
  bit_rate = SER_MAX_FREQ / bit_rate;
  if (util_get_LSB(bit_rate, &lsb) != 0) {
    printf("Error in util_get_LSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_MSB(bit_rate, &msb) != 0) {
    printf("Error in util_get_MSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_read_reg(SER_LCR, &lcr) != 0) {
    printf("Error in ser_read_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_LCR, lcr | SER_LCR_DLAB) != 0) {
    printf("Error in ser_set_bit_rate inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_DLL, lsb) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_DLM, msb) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_LCR, lcr & ~SER_LCR_DLAB) != 0) {
    printf("Error in ser_set_bit_rate inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_set_line_ctrl)(conf_t *config) {
  lcr_t lcr;
  lcr.value = 0;
  lcr.word_length = SER_LCR_WORD_SIZE(config->no_bits);
  switch (config->stop_bits) {
    case 1:
      lcr.stop_bits = SER_LCR_STOP_1;
      break;
    case 2:
      lcr.stop_bits = SER_LCR_STOP_2;
      break;
    default:
      printf("Invalid stop bits inside %s\n", __func__);
      return EXIT_FAILURE;
  }
  switch (config->parity) {
    case NO_PARITY:
      lcr.parity = SER_LCR_PAR_NONE;
      break;
    case ODD_PARITY:
      lcr.parity = SER_LCR_PAR_ODD;
      break;
    case EVEN_PARITY:
      lcr.parity = SER_LCR_PAR_EVEN;
      break;
    default:
      printf("Invalid parity inside %s\n", __func__);
      return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_LCR, lcr.value) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(ser_enable_fifo)() {
  if (ser_write_reg(SER_FCR, (SER_FCR_EN_FIFO | SER_FCR_CLR_RX_FIFO | SER_FCR_CLR_TX_FIFO | SER_FCR_TRIGGER_1)) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(ser_disable_fifo)() {
  if (ser_write_reg(SER_FCR, 0x00) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(ser_enable_int)() {
  uint8_t mcr;
  if (ser_write_reg(SER_IER, (SER_IER_RX_INT | SER_IER_TX_INT | SER_IER_LINE_ST)) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_read_reg(SER_MCR, &mcr) != 0) {
    printf("Error in ser_read_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_MCR, mcr | SER_MCR_OUT2) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_disable_int)() {
  uint8_t mcr;
  if (ser_write_reg(SER_IER, 0x00) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_read_reg(SER_MCR, &mcr) != 0) {
    printf("Error in ser_read_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_reg(SER_MCR, mcr & ~SER_MCR_OUT2) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_send_byte)(uint8_t b) {
  if (ser_write_reg(SER_THR, b) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(ser_receive_byte)(uint8_t *b) {
  if (b == NULL) {
    printf("Null pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_read_reg(SER_RBR, b) != 0) {
    printf("Error in ser_read_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(ser_send_data)() {
  uint8_t lsr;
  uint8_t data;

  if (ser_read_reg(SER_LSR, &lsr) != 0) {
    printf("Error reading Line Status Register inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  while (!queue_is_empty(tx_queue) && (lsr & SER_LSR_TX_RDY)) {
    if (queue_dequeue(tx_queue, &data) != 0) {
      printf("Error dequeuing data inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (ser_send_byte(data) != 0) {
      printf("Error sending byte inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (ser_read_reg(SER_LSR, &lsr) != 0) {
      printf("Error reading Line Status Register inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int(ser_receive_data)() {
  uint8_t lsr;
  uint8_t data;

  if (ser_read_reg(SER_LSR, &lsr) != 0) {
    printf("Error reading Line Status Register inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  while ((lsr & SER_LSR_RX_RDY)) {
    if (ser_receive_byte(&data) != 0) {
      printf("Error receiving byte inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (queue_enqueue(rx_queue, &data) != 0) {
      printf("Error enqueuing data inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (ser_read_reg(SER_LSR, &lsr) != 0) {
      printf("Error reading Line Status Register inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void(ser_ih)() {
  uint8_t iir, lsr;

  if (ser_read_reg(SER_IIR, &iir) != 0) {
    printf("Error reading Interrupt Identification Register inside %s\n", __func__);
    return;
  }


  if (!(iir & SER_IIR_INT_NP)) {
    /* Interrupt Status: Pending (Interruption) */
    if (iir & SER_IIR_RX_INT) {
      if (ser_receive_data() != 0) {
        printf("Error receiving data inside %s\n", __func__);
        return;
      }
    }
    if (iir & SER_IIR_TX_INT) {
      /* Transmitter Empty */
      if (ser_send_data() != 0) {
        printf("Error sending data inside %s\n", __func__);
        return;
      }
    }
    if (iir & SER_IIR_LINE_ST) {
      /* Line Status */
      if (ser_read_reg(SER_LSR, &lsr) != 0) {
        printf("Error reading Line Status Register inside %s\n", __func__);
        return;
      }
      if (lsr & SER_LSR_OVERRUN_ERR) {
        printf("Serial Overrun Error\n");
      }
      if (lsr & SER_LSR_PAR_ERR) {
        printf("Serial Parity Error\n");
      }
      if (lsr & SER_LSR_FR_ERR) {
        printf("Serial Framing Error\n");
      }
    }

  }
}

int(ser_exit)() {
  if (ser_disable_int() != 0) {
    printf("Error disabling interrupts inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_disable_fifo() != 0) {
    printf("Error disabling FIFO inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_destroy(tx_queue) != 0 || queue_destroy(rx_queue) != 0) {
    printf("Error destroying queues inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  free(tx_queue);
  free(rx_queue);
  return EXIT_SUCCESS;
}

uint8_t player2_bytes[7];
int player2_index = 0;
player2_info_t ser_player2_info;

ser_info_t ser_info = WAITING;
uint8_t ser_scancode;
bool ser_player2_info_is_done = false;
bool ser_scancode_is_done = false;
bool ser_player2_ready = false;

bool(ser_get_player2_ready)() {
  return ser_player2_ready;
}

void(ser_set_player2_ready)(bool ready) {
  ser_player2_ready = ready;
}

bool(ser_get_player2_info_is_done)() {
  return ser_player2_info_is_done;
}


int(ser_get_player2_info)(player2_info_t *pp) {
  if (pp == NULL) {
    printf("Null pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  *pp = ser_player2_info;
  ser_player2_info_is_done = false;
  return EXIT_SUCCESS;
}

int(ser_handle_start)() {
  uint8_t data;
  if (!queue_is_empty(rx_queue)) {
    queue_dequeue(rx_queue, &data);
    if (data == WAITING) {
      ser_info = WAITING;
      ser_player2_ready = true;
    }
  }
  return EXIT_SUCCESS;
}

int(ser_read_data_from_rx_queue)() {
  uint8_t data;
  while (!queue_is_empty(rx_queue)) {
    queue_dequeue(rx_queue, &data);
    switch (ser_info) {
      case WAITING:
        if (data == PLAYER2_INFO) {
          ser_info = PLAYER2_INFO;
        }
        break;
      case PLAYER2_INFO:
        player2_bytes[player2_index] = data;
        player2_index++;
        if (player2_index == 7) {
          ser_player2_info.x = player2_bytes[0] | ((player2_bytes[1]) << 8);
          ser_player2_info.y = player2_bytes[2] | ((player2_bytes[3]) << 8);
          ser_player2_info.target = player2_bytes[4];
          ser_player2_info.score = player2_bytes[5] | ((player2_bytes[6]) << 8);
          player2_index = 0;
          ser_info = WAITING;
          ser_player2_info_is_done = true;
        }
        break;
      default:
        break;
    }
  }
  return EXIT_SUCCESS;
}

int(ser_send_player2_info_to_txqueue)(int16_t x, int16_t y, int8_t target, uint16_t score) {
  uint8_t info = 2, lsb, msb;
  if (queue_enqueue(tx_queue, &info) != 0) {
    printf("Error enqueuing data inside 1 %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_LSB(x, &lsb) != 0) {
    printf("Error in util_get_LSB inside 2 %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &lsb) != 0) {
    printf("Error enqueuing data inside 3 %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_MSB(x, &msb) != 0) {
    printf("Error in util_get_MSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &msb) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_LSB(y, &lsb) != 0) {
    printf("Error in util_get_LSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &lsb) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_MSB(y, &msb) != 0) {
    printf("Error in util_get_MSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &msb) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &target) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_LSB(score, &lsb) != 0) {
    printf("Error in util_get_LSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &lsb) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_get_MSB(score, &msb) != 0) {
    printf("Error in util_get_MSB inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (queue_enqueue(tx_queue, &msb) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_send_data() != 0) {
    printf("Error sending data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_send_waiting_to_txqueue)() {
  uint8_t info = 1;
  if (queue_enqueue(tx_queue, &info) != 0) {
    printf("Error enqueuing data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_send_data() != 0) {
    printf("Error sending data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int ser_reset_queues() {
  if (ser_receive_data() != 0) {
    printf("Error receiving data inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  queue_destroy(tx_queue);
  queue_destroy(rx_queue);
  tx_queue = create_queue(QUEUE_SIZE, sizeof(uint8_t));
  rx_queue = create_queue(QUEUE_SIZE, sizeof(uint8_t));
  if (tx_queue == NULL || rx_queue == NULL) {
    printf("Error initializing queues inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  ser_info = WAITING;
  if (ser_write_reg(SER_FCR, (SER_FCR_EN_FIFO | SER_FCR_CLR_RX_FIFO | SER_FCR_CLR_TX_FIFO | SER_FCR_TRIGGER_1)) != 0) {
    printf("Error in ser_write_reg inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
