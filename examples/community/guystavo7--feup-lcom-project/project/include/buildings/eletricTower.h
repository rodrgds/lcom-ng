#ifndef ELETRICT_TOWER
#define ELETRICT_TOWER

/** @file eletricTower.h
 *  @brief Torre electrica (efeito / dano especifico).
 */

#include "genericTower.h"

/** @brief Cria torre electrica.
 *  @param[in] position Posicao.
 *  @return Torre electrica.
 */
GenericTower* eletrictTowerNew(Vec2d position);

/** @brief Update da torre electrica.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void eletrictTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
