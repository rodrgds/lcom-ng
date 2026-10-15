#ifndef BASIC_LONG_TOWER
#define BASIC_LONG_TOWER

/** @file basicLongTower.h
 *  @brief Torre basica de longo alcance (TOWER_KIND_LONG).
 */

#include "genericTower.h"

/** @brief Cria torre longa.
 *  @param[in] position Posicao.
 *  @return Torre long range.
 */
GenericTower* basicLongTowerNew(Vec2d position);

/** @brief Update da torre longa.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void basicLongTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
