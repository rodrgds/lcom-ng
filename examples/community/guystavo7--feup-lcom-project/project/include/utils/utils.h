/** @file utils.h
 *  @brief Constantes de IRQ do projeto e utilitario matematico simples.
 */

#define TIMER_IRQ_LINE			0x00
#define KBC_IRQ_LINE 			0x01
#define MOUSE_IRQ_LINE 			0x0C

#include <stdio.h>

/** @brief Valor absoluto em virgula flutuante (modulo).
 *  @param[in] n Numero real.
 *  @return |n|.
 */
float floatModule(float n);
