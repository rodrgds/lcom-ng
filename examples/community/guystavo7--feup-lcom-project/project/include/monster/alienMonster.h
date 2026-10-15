#pragma once

/** @file alienMonster.h
 *  @brief Fabrica monstro tipo alien.
 */

#include "monster.h"

/** @brief Cria alien na posicao dada ao longo do path.
 *  @param[in] x Spawn X.
 *  @param[in] y Spawn Y.
 *  @param[in] path Path.
 *  @return Monster alien.
 */
Monster* alienMonsterNew(float x, float y, Path* path);
