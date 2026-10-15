#ifndef BASIC_TOWER
#define BASIC_TOWER

/** @file basicTower.h
 *  @brief Torre basica (tipo default e projectil normal).
 */

#include "genericTower.h"

/** @brief Cria torre basica na posicao da grelha/mundo.
 *  @param[in] position Posicao de colocacao.
 *  @return GenericTower configurada como basica.
 */
GenericTower* basicTowerNew(Vec2d position);

/** @brief Update especifico da torre basica (chama logica interna + disparo).
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void basicTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
