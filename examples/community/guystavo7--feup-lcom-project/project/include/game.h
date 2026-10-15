#ifndef GAME
#define GAME

/** @file game.h
 *  @brief Arranque do modo jogo e ciclo principal (frame loop) sobre o terminal.
 */

#include "../../lab5/terminal.h"

/** @brief Inicializa subsistemas do jogo e entra no fluxo principal (menu/estados).
 *  @param[in] terminal Terminal LCF ja configurado.
 */
void startGame(Terminal* terminal);

/** @brief Liberta recursos globais do jogo ao sair.
 */
void exitGame();

/** @brief Um passo do loop de jogo (input, update, draw) para o estado actual.
 *  @param[in] terminal Terminal para desenho.
 *  @param[in] deltaTime Tempo desde o ultimo frame (segundos).
 *  @return false se o jogo deve terminar; true para continuar.
 */
bool gameLoop(Terminal* terminal, float deltaTime);

#endif
