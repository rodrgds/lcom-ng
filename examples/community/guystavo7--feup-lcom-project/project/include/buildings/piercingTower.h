#ifndef PIERCING_TOWER
#define PIERCING_TOWER

/** @file piercingTower.h
 *  @brief Torre de projéteis perfurantes.
 */

#include "genericTower.h"

/** @brief Cria torre piercing.
 *  @param[in] position Posicao.
 *  @return Torre piercing.
 */
GenericTower* piercingTowerNew(Vec2d position);

/** @brief Update da torre piercing.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void piercingTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
