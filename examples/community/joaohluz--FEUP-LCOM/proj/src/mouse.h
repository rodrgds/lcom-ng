#ifndef ___MOUSE_H___
#define ___MOUSE_H___

/** @defgroup Mouse Mouse
 * @{
 *
 * Funções para programar o rato.
 */

#include "menu.h"

/**
 * @brief Struct com os vários estados do mouse
 * @param INITIAL - estado do mouse antes de iniciar qualquer gesto
 * @param UP - estado do mouse quando o botão esquerdo esta premido e está a ser desenhada uma linha vertical
 * @param RIGHT - estado do mouse quando o botão direito esta premido e está a ser desenhada uma linha horizontal
 * @param COMP_UP - estado do jogo quando o gesto vertical chega ao fim
 * @param COMP_RIGHT - estado do jogo quando o gesto horizontal chega ao fim
 */
typedef enum {INITIAL,UP,RIGHT,COMP_UP, COMP_RIGHT} state_t;

extern state_t state;

/**
 * @brief Subscreve as interrupcoes do rato
 * @param bit_no - numero do bit correspondente ao hook ID
 * @return 0(zero) se a operacao for bem sucedida, senao retorna 1
 */
int (mouse_subscribe)(uint8_t *bit_no);

/**
 * @brief Poe fim a subscricao das interrupcoes do rato
 * @return 0(zero) se a operacao for bem sucedida, senao retorna 1
 */
int (mouse_unsubscribe)();

/**
 * @brief Retorna o conteudo em OUT_BUF
 * @return Conteudo no endereco OUT_BUF, retorna 1 se a operacao nao for bem sucedida
 */
uint8_t (mouse_scan_byte)();

/**
 * @brief Retira os bytes retirados do endereço OUT_BUF e os coloca no array packet, durante o jogo
 */
void (mouse_ih)();

/**
 * @brief Lê os packets com informação dobre o mouse
 * @param mouse - struct usada para guardar a informação do rato
 */
void mouse_menu_ih(Mouse* mouse);

/**
 * @brief Escreve no KBC um comando que permite escrever comandos para o mouse
 * @return 0 em case de sucesso e outro numero caso contrario
 */
int (kbc_write_cmd)();

/**
 * @brief Escreve um comando para o mouse para o KBC
 * @param cmd - comando
 */
void write_command(uint32_t cmd);

/**
 * @brief Imprime o packet com a informação sobre o mouse
 */
void printPacket();

/**
 * @brief Lê o packet com a informação sobre o mouse
 */
void checkMenuPacket(Mouse* mouse);

/**
 * @brief Apaga a informação presente no packet do mouse
 */
void clearPacket();

/**
 * @brief Ativa o stream mode do mouse
 */
void enable_cmd_int();

/**
 * @brief Inativa o stream mode do mouse
 */
void disable_cmd_int();

/**
 * @brief Altera o estado do jogo com base no estado atual e no mouse event
 * @param evt - struct usada para guardar a informacao sobre acoes do mouse
 */
void check_line(struct mouse_ev evt);

/**
 * @brief Altera o mouse event do jogo com base no estado atual do jogo e na ação do mouse cuja informacao esta no packet
 */
void set_mouse_events();

/**
 * @brief Verifica se o desenho da linha vertical com o rato não ten un declive fora da tolerancia autorizada
 */
bool fire_tolerance();

/**
 * @brief Verifica se o desenho da linha horizaontal com o rato não ten un declive fora da tolerancia autorizada
 */
bool protect_tolerance();



/**@}*/


#endif
