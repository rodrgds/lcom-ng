#ifndef PLAYER_IDLE
#define PLAYER_IDLE

/** @file playerIdle.h
 *  @brief Estado "parado" do jogador: animacao idle e transicao para corrida.
 */

#include "playerState.h"

/** @brief Logica por frame em idle (sem movimento significativo).
 *  @param[in,out] state Estado idle.
 *  @param[in,out] model Modelo do jogador.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void idleAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Decide se passa a running ou mantem idle.
 *  @param[in,out] state Estado idle.
 *  @param[in,out] model Modelo.
 *  @return Ponteiro para o proximo estado.
 */
PlayerState* idleTransition(PlayerState* state, PlayerModel* model);

/** @brief Entrada no estado idle (reset animacao, velocidade zero, etc.).
 *  @param[in,out] state Estado idle.
 *  @param[in,out] model Modelo.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void idleOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Singleton/instancia partilhada do estado idle.
 *  @return Ponteiro para PlayerState idle.
 */
PlayerState* getIdleState();

#endif
