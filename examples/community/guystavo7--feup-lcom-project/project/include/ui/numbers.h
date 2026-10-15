#ifndef NUMBERS
#define NUMBERS

/** @file numbers.h
 *  @brief Desenho de digitos/sprites para mostrar valores numericos no ecra.
 */

#include "../../../lab5/terminal.h"
#include "../../../lab5/spriteXMP.h"
#include "../arena.h"
#include "../utils/vec2d.h"

typedef struct Number Number;

/** @brief Cria widget numerico com valor inicial.
 *  @param[in] n Numero inteiro a representar.
 *  @return Number alocado.
 */
Number* numberNew(int n);

/** @brief Liberta recursos do numero.
 *  @param[in] number Instancia.
 */
void numberFree(Number* number);

/** @brief Desenha o numero na posicao indicada.
 *  @param[in,out] number Widget.
 *  @param[in] position Canto ou origem de desenho.
 *  @param[in] terminal Terminal.
 */
void numberUpdate(Number* number, Vec2d position, Terminal* terminal);

/** @brief Valor actual.
 *  @param[in] number Widget.
 *  @return Inteiro mostrado.
 */
int numberGetNumber(Number* number);

/** @brief Altera o valor mostrado.
 *  @param[in,out] number Widget.
 *  @param[in] n Novo numero.
 */
void numberSetNumber(Number* number, int n);

#endif
