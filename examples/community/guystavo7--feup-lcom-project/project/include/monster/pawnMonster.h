#pragma once

/** @file pawnMonster.h
 *  @brief Fabrica monstro tipo pawn.
 */

#include "monster.h"

/** @brief Cria pawn na posicao inicial seguindo path.
 *  @param[in] x Spawn X.
 *  @param[in] y Spawn Y.
 *  @param[in] path Path de movimento.
 *  @return Monster pawn alocado.
 */
Monster* pawnMonsterNew(float x, float y, Path* path);
