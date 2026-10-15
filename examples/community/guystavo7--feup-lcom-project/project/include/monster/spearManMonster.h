#pragma once

/** @file spearManMonster.h
 *  @brief Fabrica monstro lancer/spear.
 */

#include "monster.h"

/** @brief Cria spear man na posicao dada.
 *  @param[in] x Spawn X.
 *  @param[in] y Spawn Y.
 *  @param[in] path Path.
 *  @return Monster lancer.
 */
Monster* spearManMonsterNew(float x, float y, Path* path);
