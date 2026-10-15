#ifndef FAST_TOWER
#define FAST_TOWER

/** @file fastTower.h
 *  @brief Torre rapida (cadencia alta, dano conforme data.h).
 */

#include "genericTower.h"

/** @brief Cria torre rapida na posicao.
 *  @param[in] position Posicao.
 *  @return Torre fast.
 */
GenericTower* fastTowerNew(Vec2d position);

/** @brief Update da torre rapida.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void fastTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
