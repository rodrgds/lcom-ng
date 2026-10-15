#include <iso646.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stdbool.h>
#pragma once

/** @file vec2d.h
 *  @brief Vetores 2D em coordenadas cartesianas (float).
 */

#define EPSILON 0.0001f

/** @brief Par ordenado (x, y) em virgula flutuante. */
struct Vec2d {
    float x; /**< Componente horizontal. */
    float y; /**< Componente vertical. */
};

typedef struct Vec2d Vec2d;

/** @brief Aloca e inicializa um vetor com componentes dadas.
 *  @param[in] x Componente x.
 *  @param[in] y Componente y.
 *  @return Ponteiro para Vec2d alocado em heap, ou NULL se falhar a alocacao.
 */
Vec2d* vec2dCreate(float x, float y);

/** @brief Duplica as componentes de um vetor existente num novo bloco alocado.
 *  @param[in] v2 Vetor de origem (nao pode ser NULL).
 *  @return Novo Vec2d com as mesmas componentes que v2.
 */
Vec2d* vec2dCreateWithVec2d(Vec2d* v2);

/** @brief Liberta a memoria de um vetor criado com vec2dCreate ou vec2dCreateWithVec2d.
 *  @param[in] v Ponteiro a libertar (pode ser NULL consoante implementacao; ver codigo).
 */
void vec2dFree(Vec2d* v);

/** @brief Le a componente x.
 *  @param[in] v Vetor valido.
 *  @return Valor da componente x.
 */
float vec2dGetX(Vec2d* v);

/** @brief Le a componente y.
 *  @param[in] v Vetor valido.
 *  @return Valor da componente y.
 */
float vec2dGetY(Vec2d* v);

/** @brief Define a componente x.
 *  @param[in,out] v Vetor a modificar.
 *  @param[in] x Novo valor de x.
 */
void vec2dSetX(Vec2d* v, float x);

/** @brief Define a componente y.
 *  @param[in,out] v Vetor a modificar.
 *  @param[in] y Novo valor de y.
 */
void vec2dSetY(Vec2d* v, float y);

/** @brief Copia as componentes de v2 para v.
 *  @param[in,out] v Destino.
 *  @param[in] v2 Origem.
 */
void vec2dSetWithVec2d(Vec2d* v, Vec2d* v2);

/** @brief Calcula o modulo (norma euclidiana) do vetor.
 *  @param[in] v Vetor valido.
 *  @return sqrt(x*x + y*y).
 */
float vec2dModule(Vec2d* v);

/** @brief Calcula o quadrado do modulo (evita sqrt).
 *  @param[in] v Vetor valido.
 *  @return x*x + y*y.
 */
float vec2dModule2(Vec2d* v);

/** @brief Quadrado da distancia euclidiana entre dois pontos.
 *  @param[in] v1 Primeiro ponto.
 *  @param[in] v2 Segundo ponto.
 *  @return Distancia ao quadrado entre v1 e v2.
 */
float vec2dDistance2Between(Vec2d* v1, Vec2d* v2);

/** @brief Normaliza o vetor para norma 1 (vector unitario na mesma direcao).
 *  @param[in,out] v Vetor a normalizar; se for (0,0), nao altera.
 */
void vec2dNormalize(Vec2d* v);

/** @brief Multiplica ambas as componentes por um escalar.
 *  @param[in,out] v Vetor a escalar.
 *  @param[in] delta Fator multiplicativo.
 */
void vec2dMultiply(Vec2d* v, float delta);

/** @brief Soma v2 a v (v += v2).
 *  @param[in,out] v Acumulador.
 *  @param[in] v2 Vetor a somar.
 */
void vec2dAddVec(Vec2d* v, Vec2d* v2);

/** @brief Subtrai v2 de v (v -= v2).
 *  @param[in,out] v Vetor a atualizar.
 *  @param[in] v2 Vetor a subtrair.
 */
void vec2dSubVec(Vec2d* v, Vec2d* v2);
