#pragma once

/** @file monster.h
 *  @brief Facade do monstro: modelo + controlador + vista; movimento ao longo do path e HP.
 */

#include "monsterController.h"
#include "monsterModel.h"
#include "monsterView.h"
#include "../data.h"
#include "../../lab5/terminal.h"

typedef struct monster Monster;

/** @brief Tamanho em bytes da estrutura Monster (para alocacao generica/pools).
 *  @return sizeof(Monster).
 */
unsigned int monsterSizeof();

/** @brief Fabrica um monstro generico com parametros base e animacao construida por callback.
 *  @param[in] x Posicao inicial X.
 *  @param[in] y Posicao inicial Y.
 *  @param[in] sizeX Largura da hitbox.
 *  @param[in] sizeY Altura da hitbox.
 *  @param[in] velocity Velocidade base de deslocamento ao longo do path.
 *  @param[in] hp Pontos de vida iniciais.
 *  @param[in] animationBuilder Funcao que devolve AnimatedSprite configurado.
 *  @param[in] path Grafo de waypoints a seguir.
 *  @return Ponteiro para Monster alocado.
 */
Monster* monsterNew(float x, float y, float sizeX, float sizeY, float velocity, int hp, AnimatedSprite* (*animationBuilder)(), Path* path);

/** @brief Altera HP do monstro (dano negativo ou cura positiva).
 *  @param[in,out] m Monstro.
 *  @param[in] val Delta de HP.
 */
void monsterSumHP(Monster* m, int val);

/** @brief Indica se HP <= 0 ou flag de morte activa.
 *  @param[in] m Monstro.
 *  @return true se morto.
 */
bool monsterIsDead(Monster* m);

/** @brief Verdadeiro quando o monstro chegou ao fim do path (base para vida/custo).
 *  @param[in] m Monstro.
 *  @return true se percorreu todo o caminho.
 */
bool monsterGetFinishedPath(Monster* m);

/** @brief Passo de simulacao: IA de movimento, animacao e colisoes.
 *  @param[in,out] m Monstro.
 *  @param[in,out] arena Arena.
 *  @param[in] terminal Terminal para desenho.
 *  @param[in] delta_t Delta em segundos.
 */
void monsterUpdate(Monster* m, Arena* arena, Terminal* terminal, float delta_t);

/** @brief Liberta monstro e sub-componentes.
 *  @param[in] m Monstro.
 */
void monsterFree(Monster* m);

/** @brief Acesso ao rigid body do controlador.
 *  @param[in] m Monstro.
 *  @return RigidBody ou NULL.
 */
RigidBody* monsterGetRigidBody(Monster* m);

/** @brief Hitbox principal do monstro.
 *  @param[in] m Monstro.
 *  @return BoxCollider.
 */
BoxCollider* monsterGetBoxCollider(Monster* m);

/** @brief Reinicia posicao e estado parcial (reutilizacao de pool).
 *  @param[in,out] m Monstro.
 *  @param[in] pos Nova posicao de spawn.
 */
void monsterReset(Monster* m, Vec2d* pos);

/** @brief Deslocamento previsto no proximo passo (previsao para colisao/torre).
 *  @param[in] m Monstro.
 *  @param[in] delta_t Horizonte de tempo em segundos.
 *  @return Vector deslocamento incremental.
 */
Vec2d monsterGetFutureDisplacement(Monster* m, float delta_t);
