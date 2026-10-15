#ifndef KBD_H
#define KBD_H
#include <lcom/lcf.h>

/**
 * @brief Lê o estado do KBC para verificar se já podemos efetuar a leitura do scancode
 * @param status status of the KBC
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (kbd_read_status)(uint8_t *status);

/**
 * @brief Lê um scancode enviado pelo teclado no KBC
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (kbd_read_scancode)();

/**
 * @brief Inicia as interrupções para o keyboard
 * @param bit_no Valor inicial do hook_id
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Desativa as interrupções para o keyboard
 * @returns 1 se ocorreu um erro, 0 se correu tudo bem
*/
int (kbd_unsubscribe_int)();

/**
 * @brief Usado para obter o valor do scancode em outros documentos
 * @returns Valor do scancode
*/
uint8_t (get_scancode)();

/**
 * @brief Usado para obter o valor do hook id em outros documentos
 * @returns Valor do hook_id
*/
int (get_KBD_hook_id)();

/**
 * @brief Envia um scancode falso
 * @param fake_scancode Valor falso do scancode a ser definido.
 */
void set_scancode(uint8_t fake_scancode);

#endif
