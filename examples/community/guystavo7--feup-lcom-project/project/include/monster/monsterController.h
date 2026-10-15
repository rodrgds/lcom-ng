#pragma once

/** @file monsterController.h
 *  @brief Movimento do monstro ao longo do Path: rigid body, hitbox e integracao com a arena.
 */

#include "monsterModel.h"
#include "../data.h"

typedef struct monsterController MonsterController;

/** @brief Cria controlador ligado ao modelo e ao path partilhado.
 *  @param[in] model Modelo do monstro.
 *  @param[in] path Path de waypoints.
 *  @return Controlador alocado.
 */
MonsterController* monsterControllerNew(MonsterModel* model, Path* path);

/** @brief Passo de movimento e colisao.
 *  @param[in,out] controller Controlador.
 *  @param[in,out] arena Arena.
 *  @param[in] delta_t Delta em segundos.
 */
void monsterControllerUpdate(MonsterController* controller, Arena* arena, float delta_t);

/** @brief Liberta o controlador.
 *  @param[in] controller Instancia.
 */
void monsterControllerFree(MonsterController* controller);

/** @brief Corpo rigido do monstro.
 *  @param[in] m Controlador.
 *  @return RigidBody.
 */
RigidBody* monsterControllerGetRigidBody(MonsterController* m);

/** @brief Hitbox do monstro.
 *  @param[in] m Controlador.
 *  @return BoxCollider.
 */
BoxCollider* monsterControllerGetBoxCollider(MonsterController* m);

/** @brief Path associado.
 *  @param[in] m Controlador.
 *  @return Path.
 */
Path* monsterControllerGetPath(MonsterController* m);

/** @brief Repoe o seguidor ao inicio do path (re-spawn ou loop).
 *  @param[in,out] m Controlador.
 */
void monsterControllerGoBackToStart(MonsterController* m);

/** @brief Deslocamento previsto para predicao de torres.
 *  @param[in] controller Controlador.
 *  @param[in] delta_t Horizonte temporal.
 *  @return Delta de posicao.
 */
Vec2d monsterControllerGetFutureDisplacement(MonsterController* controller, float delta_t);
