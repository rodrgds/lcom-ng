#ifndef MORTAR_TOWER
#define MORTAR_TOWER

/** @file mortarTower.h
 *  @brief Torre morteiro (projectil skyfall / area).
 */

#include "genericTower.h"

/** @brief Cria morteiro na posicao.
 *  @param[in] position Posicao.
 *  @return Torre mortar.
 */
GenericTower* mortarTowerNew(Vec2d position);

/** @brief Update do morteiro.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void mortarTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
