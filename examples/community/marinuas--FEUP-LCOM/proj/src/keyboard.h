#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "i8254.h"
#include "timer.h"

/** 
 * @brief Função que subscreve as interrupções do teclado
 * @param - Contem o endereço do bit de interrupção
 */
int (kbd_subscribe_int)(uint8_t *bit_no) ;
/** 
 * @brief Função que cancela a subscrição das interrupções do teclado
 */
int (kbd_unsubscribe_int)();
/** 
 * @brief Função que chama o read_kbc()
 */
void (kbc_ih)();
/** 
 * @brief Manipulador de interrupção do teclado
 */
int read_kbc();
/** 
 * @brief Retorna tamanho do scancode
 * @param data - valor do byte do scancode
 */
int size(uint8_t data);
/** 
 * @brief Verifica se é makecode ou breakcode
 * @param data - valor do byte do scancode
 */
bool isMake(uint8_t data);
/** 
 * @brief Manipulador de pooling do teclado
 */
void get_data();
/** 
 * @brief Manipulador de pooling do teclado
 */
int read_kbc_poll();
/** 
 * @brief Reativa teclado
 */
int write_command();

