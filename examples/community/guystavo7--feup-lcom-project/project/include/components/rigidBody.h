#ifndef RIGIDBODY
#define RIGIDBODY

/** @file rigidBody.h
 *  @brief Movimento simples com velocidade e resolucao de colisoes contra a arena.
 */

#include <stdbool.h>

#include "../arena.h"
#include "boxCollider.h"
#include "../utils/vec2d.h"

typedef struct RigidBody RigidBody;

/** @brief Aloca um corpo rigido com velocidade zero por omissao.
 *  @return Ponteiro para RigidBody novo.
 */
RigidBody* rigidBodyNew();

/** @brief Liberta o corpo rigido.
 *  @param[in] rb Instancia.
 */
void rigidBodyFree(RigidBody* rb);

/** @brief Integra posicao com velocidade e trata colisoes com tiles/paredes da arena.
 *  @param[in,out] rb Corpo.
 *  @param[in,out] pos Posicao do objeto (actualizada).
 *  @param[in] arena Arena para consulta de obstaculos.
 *  @param[in,out] hitBox Caixa de colisao associada ao objecto.
 *  @param[in] deltaTime Passo de tempo.
 */
void rigidBodyUpdate(RigidBody* rb, Vec2d* pos, Arena* arena, BoxCollider* hitBox, float deltaTime);

/** @brief Define velocidade em pixeis por segundo (ou unidades de jogo / s).
 *  @param[in,out] rb Corpo.
 *  @param[in] xSpeed Componente X da velocidade.
 *  @param[in] ySpeed Componente Y da velocidade.
 */
void rigidBodySetVelocity(RigidBody* rb, int xSpeed, int ySpeed);

#endif
