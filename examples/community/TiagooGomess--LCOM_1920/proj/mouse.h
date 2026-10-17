#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "video_card.h"
#include "sprite.h"

#define STATUS_REGISTER 0x64
#define OUTPUT_BUFFER 0x60
#define INPUT_BUFFER_DATA 0x60
#define INPUT_BUFFER_COMMAND 0x64
#define DELAY_US 20000
#define ESC_BREAK 0x81
#define KBD_IRQ 0x01
#define MOUSE_CTR_IRQ 12

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


// Macros para primeiro byte do pacote
#define LEFT_MOUSE_BUTTON_PRESSED 0x01
#define RIGHT_MOUSE_BUTTON_PRESSED 0x02
#define MIDDLE_MOUSE_BUTTON_PRESSED 0x04
#define BIT_3 0x08 // bit 3 a 1
#define DELTA_X 0x10
#define DELTA_Y 0x20
#define X_OVERFLOW 0x40
#define Y_OVERFLOW 0x80

// Macros relativas ao rato
#define ACK	0xFA
#define NACK 0xFE
#define ERROR 0xFC
#define RESEND 0xFE
#define KBC_MAX_ATTEMPTS 3
#define SET_STREAM_MODE	0xEA
#define SET_REMOTE_MODE 0xF0
#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5
#define READ_DATA 0xEB
#define WRITE_BYTE_TO_MOUSE 0xD4

#define TO_INTEGER 0xFF00

#define BIT(n) (1 << (n)) // operação de deslocamento para a esquerda de n bits


/**
 * @brief - Envia comando para o rato.
 * @param comando - Comando a enviar.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int send_command_to_mouse(uint32_t comando);

/**
 * @brief - Subscribes and enables mouse interrupts.
 * @param bit_no - address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return - Return 0 upon success and non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief - Unsubscribes mouse interrupts.
 * @return - Return 0 upon success and non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief - Ativa o data reporting.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int our_mouse_enable_data_reporting();

/**
 * @brief - Desativa o data reporting.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int mouse_reset_state();

/**
 * Trata das interrupções do rato.
 */
void (mouse_ih)(void);

/**
 * @brief - Faz o parsing dos pacotes do rato.
 * @param pacote - Pacote recebido.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int packet_parsing(struct packet* pacote);

