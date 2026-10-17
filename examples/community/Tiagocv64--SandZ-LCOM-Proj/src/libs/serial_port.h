#pragma once

#define IRQ_COM1 4
#define BASE_COM1 0x3F8

#define UART_WAIT_TIME 1000

#define RECEIVER_REG 0
#define TRANSMITTER_REG 0

#define RBR 0
#define THR 0
#define IER 1
#define IIR 2
#define LCR 3
#define LSR 5

#define UART_DLL 0
#define UART_DLM 1

#define UART_5_BITS 0
#define UART_6_BITS 1
#define UART_7_BITS 2
#define UART_8_BITS 3
#define UART_1_STOP_BIT 0
#define UART_2_STOP_BIT BIT(2)
#define UART_NO_PARITY 0
#define UART_ODD_PARITY BIT(3)
#define UART_EVEN_PARITY (BIT(3) | BIT(4))
#define UART_BIT_1_PARITY (BIT(3) | BIT(5))
#define UART_BIT_0_PARITY (BIT(3) | BIT(4) | BIT(5))
#define UART_DLAB BIT(7)

#define UART_REC_DATA_AVL_INT BIT(0)
#define UART_TRANS_HOLD_EMPTY_INT BIT(1)
#define UART_REC_DATA_ERROR_INT BIT(2)

#define UART_IIR_NO_INT BIT(0)
#define UART_IIR_RLS (BIT(1) | BIT(2))
#define UART_IIR_RDA BIT(2)

#define UART_DATA_READ   1
#define UART_OVERRUN_ERR BIT(1)
#define UART_PARITY_ERR  BIT(2)
#define UART_FRAME_ERR   BIT(3)

#define UART_TX_RDY BIT(5)

int (uart_subscribe_int)(uint8_t *bit_no);

int (uart_unsubscribe_int)();

int uart_read_reg(uint8_t reg, uint8_t* arg);

int uart_write_reg(uint8_t reg, uint8_t arg);

int uart_enable_DL();

int uart_disable_DL();

int uart_set_bitrate(uint32_t bitrate);

int uart_send_char(uint8_t c);

int uart_send_message(char* cs, int8_t s);

int init_uart_proj();

void handle_mess(char c);

void uart_ih();
