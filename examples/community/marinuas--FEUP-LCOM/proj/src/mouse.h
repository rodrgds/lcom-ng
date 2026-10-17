#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"
#include "game.h"
#include <lcom/utils.h>

/**
 * @brief Subscreve as interrupções do rato
 * @param - Contem o endereço do bit de interrupção
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
/** 
 * @brief Cancela a subscrição das interrupções do teclado
 */
int (mouse_unsubscribe_int)();
/**
 * @brief Habilita o relatório de dados
 */
int enable();
/**
 * @brief Desabilita o relatório de dados
 */
int disable();
/**
 * @brief Manipulador de interrupção do rato
 */
void (mouse_ih)();
/**
 * @brief Atualiza o valor de cada elemento do array packet[] com o valor dos bytes
 */
void get_mouse_status();
/**
 * @brief Analisa o pacote do mouse
 * @param pp - ponteiro para o packet da struct 
 */
void packet_assemble(struct packet *pp);
/**
 * @brief Converte em complemento para 2
 * @param byte - valor a ser convertido
 * @param msb - onde será guardado o valor
 */
int complementTo2(uint8_t byte, int msb);
/**
 * @brief Verifica se os pacotes estão sicronizados
 */
bool isSyncronized();
/**
 * @brief Escreve o comando
 * @param command - comando a ser escrito
 */
int mouse_write_command(uint32_t command );
/**
 * @brief Verifica se os pacotes estão sicronizados
 */
void hook_id_enable();
/**
 * @brief Verifica se os pacotes estão sicronizados
 */
void hook_id_disable();
/**
 * @brief Atualiza a posição do rato sempre se este se move e caso este parar em cima de uma certa posição e for carregado o botão esquerdo do rato, muda o valor do state
 * @param pp - ponteiro para o packet da struct
 * @param state - Maquina de Estados do Jogo
 */
int mouse_pos(struct packet *pp, enum game_state *state);

