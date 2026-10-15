#pragma once

/** @file monsterModel.h
 *  @brief Dados do monstro: vida, posicao, velocidade, dimensoes e flags de morte/fim de path.
 */

#include "../../include/utils/vec2d.h"

#include "../../include/monster/path.h"
#include "../../include/components/rigidBody.h"
#include "../../include/components/boxCollider.h"

#define MonsterTag 1
#include <stdio.h>

typedef struct monsterModel MonsterModel;

/** @brief Cria modelo com posicao, tamanho, velocidade inicial e HP.
 *  @param[in] x Posicao X inicial.
 *  @param[in] y Posicao Y inicial.
 *  @param[in] sizeX Largura da hitbox.
 *  @param[in] sizeY Altura da hitbox.
 *  @param[in] velX Velocidade inicial X.
 *  @param[in] velY Velocidade inicial Y.
 *  @param[in] hp Vida inicial.
 *  @return Modelo alocado.
 */
MonsterModel* monsterModelNew(float x, float y, float sizeX, float sizeY, float velX, float velY, int hp);

/** @brief Liberta modelo e componentes internos (rigid, box, etc.).
 *  @param[in] m Modelo.
 */
void monsterModelFree(MonsterModel* m);

/** @brief HP actual.
 *  @param[in] m Modelo.
 *  @return Pontos de vida.
 */
int monsterModelGetHP(MonsterModel* m);

/** @brief Define HP absoluto.
 *  @param[in,out] m Modelo.
 *  @param[in] hp Novo HP.
 */
void monsterModelSetHP(MonsterModel* m, int hp);

/** @brief HP maximo / inicial de referencia.
 *  @param[in] m Modelo.
 *  @return HP inicial configurado.
 */
int monsterModelGetInitialHP(MonsterModel* m);

/** @brief Define HP inicial (para resets).
 *  @param[in,out] m Modelo.
 *  @param[in] hp Valor inicial.
 */
void monsterModelSetInitialHP(MonsterModel* m, int hp);

/** @brief Soma delta ao HP.
 *  @param[in,out] m Modelo.
 *  @param[in] val Variacao (negativa = dano).
 */
void monsterModelSumHP(MonsterModel* m, int val);

/** @brief Flag de morte.
 *  @param[in] m Modelo.
 *  @return true se morto.
 */
bool monsterModelGetDead(MonsterModel* m);

/** @brief Define estado de morte explicitamente.
 *  @param[in,out] m Modelo.
 *  @param[in] dead Novo valor.
 */
void monsterModelSetDead(MonsterModel* m, bool dead);

/** @brief Indica se o monstro ja completou o path.
 *  @param[in] m Modelo.
 *  @return true se chegou ao fim.
 */
bool monsterModelGetFinishedPath(MonsterModel* m);

/** @brief Define fim de path (ex.: ao atingir ultimo waypoint).
 *  @param[in,out] m Modelo.
 *  @param[in] finishedPath Novo valor.
 */
void monsterModelSetFinishedPath(MonsterModel* m, bool finishedPath);

/** @brief Posicao X.
 *  @param[in] m Modelo.
 *  @return Coordenada X.
 */
float monsterModelGetX(MonsterModel* m);

/** @brief Posicao Y.
 *  @param[in] m Modelo.
 *  @return Coordenada Y.
 */
float monsterModelGetY(MonsterModel* m);

/** @brief Largura logica.
 *  @param[in] m Modelo.
 *  @return sizeX.
 */
float monsterModelGetSizeX(MonsterModel* m);

/** @brief Altura logica.
 *  @param[in] m Modelo.
 *  @return sizeY.
 */
float monsterModelGetSizeY(MonsterModel* m);

/** @brief Dimensoes como Vec2d.
 *  @param[in] m Modelo.
 *  @return Par (largura, altura).
 */
Vec2d monsterModelGetSize(MonsterModel* m);

/** @brief Velocidade X.
 *  @param[in] m Modelo.
 *  @return vx.
 */
float monsterModelGetVelX(MonsterModel* m);

/** @brief Velocidade Y.
 *  @param[in] m Modelo.
 *  @return vy.
 */
float monsterModelGetVelY(MonsterModel* m);

/** @brief Copia posicao actual para pos.
 *  @param[in] m Modelo.
 *  @param[out] pos Destino.
 */
void monsterModelGetPos(MonsterModel* m, Vec2d* pos);

/** @brief Define posicao (actualiza modelo e possivelmente collider).
 *  @param[in,out] m Modelo.
 *  @param[in] pos Nova posicao.
 */
void monsterModelSetPos(MonsterModel* m, Vec2d* pos);

/** @brief Copia velocidade para pos (nome legacy do vector velocidade).
 *  @param[in] m Modelo.
 *  @param[out] pos Saida (vx,vy).
 */
void monsterModelGetVel(MonsterModel* m, Vec2d* pos);

/** @brief Define velocidade a partir de vector.
 *  @param[in,out] m Modelo.
 *  @param[in] pos Vector velocidade.
 */
void monsterModelSetVel(MonsterModel* m, Vec2d* pos);

/** @brief Define coordenada X.
 *  @param[in,out] m Modelo.
 *  @param[in] x Nova X.
 */
void monsterModelSetX(MonsterModel* m, float x);

/** @brief Define coordenada Y.
 *  @param[in,out] m Modelo.
 *  @param[in] y Nova Y.
 */
void monsterModelSetY(MonsterModel* m, float y);

/** @brief Define componente vx.
 *  @param[in,out] m Modelo.
 *  @param[in] x Nova vx.
 */
void monsterModelSetVelX(MonsterModel* m, float x);

/** @brief Define componente vy.
 *  @param[in,out] m Modelo.
 *  @param[in] y Nova vy.
 */
void monsterModelSetVelY(MonsterModel* m, float y);

/** @brief Define direccao de movimento normalizada ou escalar (dx,dy).
 *  @param[in,out] m Modelo.
 *  @param[in] dx Componente X da direccao.
 *  @param[in] dy Componente Y da direccao.
 */
void monsterModelSetDir(MonsterModel* m, float dx, float dy);

/** @brief Define direccao a partir de vector.
 *  @param[in,out] m Modelo.
 *  @param[in] v Vector direccao.
 */
void monsterModelSetDirWithVec(MonsterModel* m, Vec2d* v);
