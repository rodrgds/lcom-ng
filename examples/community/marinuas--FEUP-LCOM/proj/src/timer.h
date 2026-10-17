#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Altera a frequência de operação de um temporizador
 * @param timer - Timer para configurar. (Entre 0 a 2)
 * @param freq - Frequência de operação do temporizador
*/
int (timer_set_frequency)(uint8_t timer, uint32_t freq);
/** 
 * @brief Função que subscreve as interrupções do timer
 * @param - Contem o endereço do bit de interrupção
 */
int (timer_subscribe_int)(uint8_t *bit_no);
/** 
 * @brief Função que cancela a subscrição das interrupções do teclado
 */
int (timer_unsubscribe_int)();
/** 
 * @brief Incrementa o counter
 */
void (timer_int_handler)();
/** 
 * @brief Lê a configuração do cronômetro
 * @param timer - timer cuja configuração deve ser lida
 * @param st - Endereço da posição da memória a ser preenchido com a configuração do temporizador
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);
/** 
 * @brief Mostra a configuração do cronômetro
 * @param timer - timer cuja configuração deve ser exibida
 * @param st - status lido através do comando read-back
 * @param timer_status_field field - exibir de forma human friendly
 */
int (timer_display_conf)(uint8_t timer, uint8_t st,enum timer_status_field field); 
