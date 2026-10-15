#ifndef GAME_STATE_MODULE
#define GAME_STATE_MODULE

/** @file gameState.h
 *  @brief Maquina de estados do jogo (menus, jogo a correr, transicoes).
 */

#include "../../lab5/terminal.h"

typedef struct GameState GameState;

/** @brief Actualiza o estado actual (logica e desenho por frame).
 *  @param[in,out] state Estado concreto (polimorfismo manual).
 *  @param[in] terminal Terminal.
 *  @param[in] deltaTime Delta em segundos.
 */
void gameStateUpdate(GameState* state, Terminal* terminal, float deltaTime);

/** @brief Liberta recursos do estado.
 *  @param[in] state Estado a libertar.
 */
void gameStateFree(GameState* state);

/** @brief Processa transicao (ex.: menu -> jogo); pode devolver novo estado e libertar o anterior.
 *  @param[in,out] state Estado corrente.
 *  @param[in] terminal Terminal.
 *  @return Ponteiro para o proximo GameState (pode ser o mesmo).
 */
GameState* gameStateTransition(GameState* state, Terminal* terminal);

/** @brief Fabrica o estado de jogo em curso (partida na arena).
 *  @param[in] terminal Terminal.
 *  @return Novo estado "running".
 */
GameState* runningGameNew(Terminal* terminal);

/** @brief Fabrica o estado do menu principal.
 *  @param[in] terminal Terminal.
 *  @return Novo estado de menu.
 */
GameState* mainMenuNew(Terminal* terminal);

#endif
