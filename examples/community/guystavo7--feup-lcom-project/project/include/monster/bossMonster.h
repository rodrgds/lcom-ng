#pragma once

/** @file bossMonster.h
 *  @brief Fabrica o boss.
 */

#include "monster.h"

/** @brief Cria boss na posicao inicial.
 *  @param[in] x Spawn X.
 *  @param[in] y Spawn Y.
 *  @param[in] path Path (pode ser curto ou simbolico).
 *  @return Monster boss.
 */
Monster* bossMonsterNew(float x, float y, Path* path);
