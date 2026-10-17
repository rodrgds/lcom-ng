#pragma once

#include <lcom/lcf.h>


#define STATUS_REGISTER 0x64
#define OUTPUT_BUFFER 0x60
#define INPUT_BUFFER_DATA 0x60
#define INPUT_BUFFER_COMMAND 0x64
#define INPUT_BUFFER 0x20
#define DELAY_US 20000
#define ESC_BREAK 0x81
#define KBD_IRQ 0x01


#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60

// Macros para o status register:
#define OBF 0x01 // 00000001
#define IBF 0x02 // 00000010
#define SYS 0x04 // 00000100
#define A2 0x08 // 00001000
#define INH 0x10 // 00010000
#define AUX 0x20 // 00100000
#define TIMEOUT 0x40 // 01000000
#define PARITY 0x80 // 10000000

#define BIT(n) (1 << (n)) // operacao de deslocamento para a esquerda de n bits


/**
 * @brief - Subscribes and enables keyboard interrupts.
 * @param bit_no - address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return - Return 0 upon success and non-zero otherwise.
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief - Unsubscribes keyboard interrupts.
 * @return - Return 0 upon success and non-zero otherwise.
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief - Escreve o comando do teclado para o KBC.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int write_KBC_command_byte();

/**
 * Trata das interrupções do teclado.
 */
void (kbc_ih)();



