#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include "./../../snake.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>



/**
 *@brief Função que tenta ler um byte do controlador do teclado
 *
 * @param data eponteiro onde será armazenado o valor lido
 * 
 * @return retorna 0 em caso de sucesso
 */
int(read_KeyboardController_Output)(uint8_t port, uint8_t *data);



/**
 *@brief Função que subscreve e permite as interrupções do teclado
 *
 *
 * @param bit_no endereço de memória que vai ser inicializado com o bit do número ativado na máscara 
 * 
 * @return retorna 0 em caso de sucesso
 */
int(kbd_subscribe_int)(uint8_t *bit_no);


/**
 *@brief Função que calcela a subescrição do teclado
 *
 * 
 * @return retorna 0 em caso de sucesso
 */
int(kbd_unsubscribe_int)();

#endif
