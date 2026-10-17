#ifndef ___KEYBOARD_H___
#define ___KEYBOARD_H___

/** @defgroup Keyboard Keyboard
 * @{
 *
 * Funções para programar o keyboard.
 */

int byteCount;
int size;
bool is_over, make;
uint8_t byte;

/**
 * @brief Subscreve os interrupts do keyboard
 * @param bit_no - enderço para a bitmask que identifica os interrupts do keyboard
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
int (kb_subscribe)(uint8_t *bit_no);

/**
 * @brief Termina a subscrição dos interrupts do keyboard
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
int (kb_unsubscribe)();

/**
 * @brief Lê o scan code do keyboard
 * @return Devolve o scan code lido
 */
uint8_t (kb_scan_byte)(bool poll);

/**
 * @brief Toma uma determinda ação com base no scancode do keyboard
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
int (kb_ih)();

/**
 * @brief Lê o scan code do keyboard em modo de polling
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
int (kb_read_poll)();


int (kbc_reenable)();

/**@}*/

#endif /* __KEYBOARD_H__ */
