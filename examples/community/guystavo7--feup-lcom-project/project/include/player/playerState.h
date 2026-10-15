#ifndef PLAYER_STATE 
#define PLAYER_STATE 

/** @file playerState.h
 *  @brief Estado da maquina de estados do jogador (idle/running): callbacks act/onEnter/transition.
 */

#include "playerModel.h"
#include "../../lab5/terminal.h"
#include "../arena.h"

typedef struct PlayerState PlayerState;

/** @brief Aloca estado vazio (callbacks a definir com playerStateSetFunctions).
 *  @return Novo PlayerState.
 */
PlayerState* playerStateNew();

/** @brief Liberta o estado.
 *  @param[in] state Estado.
 */
void playerStateFree(PlayerState* state);

/** @brief Instala as tres funcoes da maquina de estados.
 *  @param[in,out] state Estado.
 *  @param[in] act Chamada por frame no estado activo.
 *  @param[in] onEnter Chamada ao entrar no estado.
 *  @param[in] transition Decide o proximo estado ou devolve o mesmo.
 */
void playerStateSetFunctions(PlayerState* state, void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), void (*onEnter)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), PlayerState* (*transition)(PlayerState* state, PlayerModel* model));

/** @brief Invoca onEnter configurado.
 *  @param[in,out] state Estado.
 *  @param[in,out] model Modelo do jogador.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void playerStateOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Invoca act configurado.
 *  @param[in,out] state Estado.
 *  @param[in,out] model Modelo.
 *  @param[in] terminal Terminal.
 *  @param[in] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void playerStateAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Invoca transition configurado.
 *  @param[in,out] state Estado actual.
 *  @param[in,out] model Modelo.
 *  @return Proximo estado (pode ser o mesmo ponteiro).
 */
PlayerState* playerStateTransition(PlayerState* state, PlayerModel* model);

#endif
