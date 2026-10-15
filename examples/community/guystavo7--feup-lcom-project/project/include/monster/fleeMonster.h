#pragma once

/** @file fleeMonster.h
 *  @brief Fabrica monstro em fuga (comportamento especifico de movimento).
 */

#include "monster.h"

/** @brief Cria flee monster.
 *  @param[in] x Spawn X.
 *  @param[in] y Spawn Y.
 *  @param[in] path Path.
 *  @return Monster flee.
 */
Monster* fleeMonsterNew(float x, float y, Path* path);
