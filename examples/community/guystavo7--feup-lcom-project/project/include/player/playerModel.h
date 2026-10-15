#ifndef PLAYER_MODEL
#define PLAYER_MODEL

/** @file playerModel.h
 *  @brief Estado fisico e visual do jogador: posicao, direccao, sprites idle/run, hitbox e rigid body.
 */

#include "../components/animatedSprite.h"
#include "../components/rigidBody.h"
#include "../components/boxCollider.h"
#include "./../arena.h"
#include "./../utils/vec2d.h"
#include "../../lab5/terminal.h"
#include <stdbool.h>

typedef struct PlayerModel PlayerModel;

/** @brief Constroi modelo na posicao dada (sprites e collider internos).
 *  @param[in] pos Posicao inicial.
 *  @return Modelo alocado.
 */
PlayerModel* playerModelNew(Vec2d pos);

/** @brief Liberta modelo e recursos graficos.
 *  @param[in] playerModel Modelo.
 */
void playerModelFree(PlayerModel* playerModel);

/** @brief Posicao X do jogador.
 *  @param[in] playerModel Modelo.
 *  @return Coordenada X.
 */
float playerModelGetX(PlayerModel* playerModel);

/** @brief Posicao Y do jogador.
 *  @param[in] playerModel Modelo.
 *  @return Coordenada Y.
 */
float playerModelGetY(PlayerModel* playerModel);

/** @brief Largura do sprite/hitbox logico.
 *  @param[in] playerModel Modelo.
 *  @return Largura.
 */
float playerModelGetWidth(PlayerModel* playerModel);

/** @brief Altura do sprite/hitbox logico.
 *  @param[in] playerModel Modelo.
 *  @return Altura.
 */
float playerModelGetHeight(PlayerModel* playerModel);

/** @brief Componente X do vector de direccao (movimento).
 *  @param[in] playerModel Modelo.
 *  @return dirX normalizado ou escalar conforme implementacao.
 */
float playerModelGetDirX(PlayerModel* playerModel);

/** @brief Componente Y do vector de direccao.
 *  @param[in] playerModel Modelo.
 *  @return dirY.
 */
float playerModelGetDirY(PlayerModel* playerModel);

/** @brief Limite de velocidade escalar.
 *  @param[in] playerModel Modelo.
 *  @return Velocidade maxima permitida.
 */
float playerModelGetMaxVelocity(PlayerModel* playerModel);

/** @brief Sprite animado actualmente seleccionado para desenho.
 *  @param[in] playerModel Modelo.
 *  @return Ponteiro para AnimatedSprite activo.
 */
AnimatedSprite* playerModelGetAnimatedSprite(PlayerModel* playerModel);

/** @brief Caixa de colisao do jogador.
 *  @param[in] playerModel Modelo.
 *  @return Hitbox.
 */
BoxCollider* playerModelGetHitBox(PlayerModel* playerModel);

/** @brief Corpo rigido usado para movimento e colisao com tiles.
 *  @param[in] playerModel Modelo.
 *  @return RigidBody.
 */
RigidBody* playerModelGetRigidBody(PlayerModel* playerModel);

/** @brief Define X.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] x Nova coordenada.
 */
void playerModelSetX(PlayerModel* playerModel, float x);

/** @brief Define Y.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] y Nova coordenada.
 */
void playerModelSetY(PlayerModel* playerModel, float y);

/** @brief Define posicao completa.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] pos Novo Vec2d.
 */
void playerModelSetPos(PlayerModel* playerModel, Vec2d pos);

/** @brief Define componente X da direccao.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] dirX Valor [-1,1] tipicamente.
 */
void playerModelSetDirX(PlayerModel* playerModel, float dirX);

/** @brief Define componente Y da direccao.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] dirY Valor [-1,1] tipicamente.
 */
void playerModelSetDirY(PlayerModel* playerModel, float dirY);

/** @brief Define direccao de movimento de uma vez.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] dir Vector direccao.
 */
void playerModelSetDir(PlayerModel* playerModel, Vec2d dir);

/** @brief Velocidade actual X.
 *  @param[in] playerModel Modelo.
 *  @return vx.
 */
float playerModelGetVelocityX(PlayerModel* playerModel);

/** @brief Velocidade actual Y.
 *  @param[in] playerModel Modelo.
 *  @return vy.
 */
float playerModelGetVelocityY(PlayerModel* playerModel);

/** @brief Define velocidade X.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] x Nova vx.
 */
void playerModelSetVelocityX(PlayerModel* playerModel, float x);

/** @brief Define velocidade Y.
 *  @param[in,out] playerModel Modelo.
 *  @param[in] y Nova vy.
 */
void playerModelSetVelocityY(PlayerModel* playerModel, float y);

/** @brief Sprite da animacao a correr.
 *  @param[in] model Modelo.
 *  @return AnimatedSprite de corrida.
 */
AnimatedSprite* playerModelGetRunningSprite(PlayerModel* model);

/** @brief Sprite da animacao idle.
 *  @param[in] model Modelo.
 *  @return AnimatedSprite idle.
 */
AnimatedSprite* playerModelGetIdleSprite(PlayerModel* model);

/** @brief Substitui o sprite animado activo (troca visual).
 *  @param[in,out] model Modelo.
 *  @param[in] sprite Novo sprite.
 */
void playerModelSetAnimatedSprite(PlayerModel* model, AnimatedSprite* sprite);

/** @brief Indica se o sprite deve ser espelhado (olhar para esquerda).
 *  @param[in] model Modelo.
 *  @return true se espelhado.
 */
bool playerModelGetIsFlipped(PlayerModel* model);

/** @brief Define espelhamento horizontal.
 *  @param[in,out] model Modelo.
 *  @param[in] isFlip true para espelhar.
 */
void playerModelSetIsFlipped(PlayerModel* model, bool isFlip);

#endif
