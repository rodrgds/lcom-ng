#ifndef PLAYER_RUNNING
#define PLAYER_RUNNING

/** @file playerRunning.h
 *  @brief Estado "a correr": movimento com teclas e transicao para idle.
 */

#include "playerState.h"

/** @brief Logica por frame em movimento.
 *  @param[in,out] state Estado running.
 *  @param[in,out] model Modelo.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void runningAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Transicao running -> idle quando nao ha input de movimento.
 *  @param[in,out] state Estado running.
 *  @param[in,out] model Modelo.
 *  @return Proximo estado.
 */
PlayerState* runningTransition(PlayerState* state, PlayerModel* model);

/** @brief Entrada no estado running.
 *  @param[in,out] state Estado running.
 *  @param[in,out] model Modelo.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void runningOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Instancia partilhada do estado running.
 *  @return Ponteiro para PlayerState running.
 */
PlayerState* getRunningState();

#endif
