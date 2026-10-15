#ifndef PROJECTILE_H
#define PROJECTILE_H

/** @file projectile.h
 *  @brief Projéteis das torres: movimento, tipo, dano, piercing e alvos homing/skyfall.
 */

#include "../utils/vec2d.h"
#include "../../../lab5/terminal.h"
#include "boxCollider.h"
#include "animatedSprite.h"

#include <stdbool.h>

/** @brief Variante comportamental do projétil. */
typedef enum {
    PROJ_NORMAL,   /**< Trajectoria linear simples. */
    PROJ_PIERCING, /**< Atravessa varios inimigos ate max pierce. */
    PROJ_HOMING,   /**< Segue alvo actualizado por projectileUpdateHomingTarget. */
    PROJ_SKY_FALL, /**< Queda do ceu sobre coordenada alvo. */
	PROJ_NEAR      /**< Alcance curto / area proxima (torre especifica). */
} ProjectileType;

typedef struct projectile Projectile;

/** @brief Cria projétil com tipo, posicao inicial, velocidade e sprite.
 *  @param[in] type Tipo de projétil.
 *  @param[in] x Posicao inicial X.
 *  @param[in] y Posicao inicial Y.
 *  @param[in] vel_x Velocidade X.
 *  @param[in] vel_y Velocidade Y.
 *  @param[in] sprite Animacao do projétil.
 *  @return Ponteiro alocado ou NULL.
 */
Projectile* projectileCreate(ProjectileType type, float x, float y, float vel_x, float vel_y, AnimatedSprite* sprite);

/** @brief Reinicializa um projétil existente (pool) com novos parametros e sprites de seta.
 *  @param[in,out] p Projétil.
 *  @param[in] type Novo tipo.
 *  @param[in] x Nova X.
 *  @param[in] y Nova Y.
 *  @param[in] vel_x Nova vx.
 *  @param[in] vel_y Nova vy.
 *  @param[in] arrowSprites Array de XPM para animacao.
 *  @param[in] index Indice inicial no array de sprites.
 */
void projectileReset(Projectile* p, ProjectileType type, float x, float y, float vel_x, float vel_y, SpriteXPM** arrowSprites, int index);

/** @brief Liberta memoria do projétil.
 *  @param[in] p Projétil.
 */
void projectileFree(Projectile* p);

/** @brief Move o projétil e actualiza estado (tempo de vida, animacao, etc.).
 *  @param[in,out] p Projétil.
 *  @param[in] delta_t Delta em segundos (double).
 *  @param[in] terminal Terminal para desenho.
 */
void projectileUpdate(Projectile* p, double delta_t, Terminal* terminal);

/** @brief Actualiza alvo para projéteis homing.
 *  @param[in,out] p Projétil.
 *  @param[in] target_x Alvo X.
 *  @param[in] target_y Alvo Y.
 */
void projectileUpdateHomingTarget(Projectile* p, float target_x, float target_y);

/** @brief Actualiza ponto de impacto para skyfall.
 *  @param[in,out] p Projétil.
 *  @param[in] target_x Impacto X.
 *  @param[in] target_y Impacto Y.
 */
void projectileUpdateSkyFallTarget(Projectile* p, float target_x, float target_y);

/** @brief Posicao X.
 *  @param[in] p Projétil.
 *  @return Coordenada X.
 */
float projectileGetX(Projectile* p);

/** @brief Posicao Y.
 *  @param[in] p Projétil.
 *  @return Coordenada Y.
 */
float projectileGetY(Projectile* p);

/** @brief Velocidade X.
 *  @param[in] p Projétil.
 *  @return vx.
 */
float projectileGetVelX(Projectile* p);

/** @brief Velocidade Y.
 *  @param[in] p Projétil.
 *  @return vy.
 */
float projectileGetVelY(Projectile* p);

/** @brief Tipo do projétil.
 *  @param[in] p Projétil.
 *  @return ProjectileType.
 */
ProjectileType projectileGetType(Projectile* p);

/** @brief Indica se o projétil ainda esta activo no mundo.
 *  @param[in] p Projétil.
 *  @return true se activo.
 */
bool projectileIsActive(Projectile* p);

/** @brief Liga/desliga o projétil (ex.: apos sair do ecra).
 *  @param[in,out] p Projétil.
 *  @param[in] active Novo estado.
 */
void projectileSetActive(Projectile* p, bool active);

/** @brief Pode aplicar dano neste frame (cooldown interno).
 *  @param[in] p Projétil.
 *  @return true se permitido infligir dano.
 */
bool projectileCanDealDamage(Projectile* p);

/** @brief Caixa de colisao usada contra monstros.
 *  @param[in] p Projétil.
 *  @return Ponteiro para BoxCollider.
 */
BoxCollider* projectileGetCollider(Projectile* p);

/** @brief Numero de inimigos ja atravessados (piercing).
 *  @param[in] p Projétil.
 *  @return Contagem de pierces usados.
 */
int projectileGetPierces(Projectile* p);

/** @brief Incrementa contador de pierce apos acerto.
 *  @param[in,out] p Projétil.
 */
void projectileAddPierce(Projectile* p);

/** @brief Define limite maximo de alvos atravessados.
 *  @param[in,out] p Projétil.
 *  @param[in] max Maximo de pierces permitidos.
 */
void projectileSetMaxPierce(Projectile* p, int max);

/** @brief Ajusta velocidade (aceleracao ou knockback).
 *  @param[in,out] p Projétil.
 *  @param[in] incX Incremento vx.
 *  @param[in] incY Incremento vy.
 */
void projectileUpdadeSpeed(Projectile* p, float incX, float incY);

/** @brief Verifica se ja acertou neste inimigo (evitar multiplos hits no mesmo frame).
 *  @param[in] p Projétil.
 *  @param[in] enemy Collider do inimigo.
 *  @return true se ja registado como hit.
 */
bool projectileHasHit(Projectile* p, BoxCollider* enemy);

/** @brief Regista que este inimigo foi atingido por este projétil.
 *  @param[in,out] p Projétil.
 *  @param[in] enemy Collider atingido.
 */
void projectileAddHit(Projectile* p, BoxCollider* enemy);

#endif
