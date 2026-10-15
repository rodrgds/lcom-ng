#ifndef GAME_SCORE_H
#define GAME_SCORE_H

#include <lcom/lcf.h>
#include "xpms.h"
#include "devices/video.h"
#include "sprite.h"
#include "color.h"

/**
 * @brief Inicia as estruturas que vão conter os sprites usados no quadro de pontuações
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (loadInitialXPMScore)();

/**
 * @brief Desenha no ecrã as pontuações no quadro de pontuações
 * @param playerNumb Número do jogador cuja pontuação vai ser desenhada
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
 * 
*/
int (drawScore)(int playerNumb);

/**
 * @brief Desenha o texto que aparece antes das pontuações e que permite distinguir as pontuações do player 1 e 2
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (drawScoreText)();

/**
 * @brief Atualiza o XPM a usar tendo em conta a nova pontuação de um certo jogador
 * @param playerNumb Número do jogador que vai ter um novo XPM para a sua pontução
 * @param score Nova pontuação
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (updateXPMScore)(int playerNumb, int score);

/**
 * @brief Escolhe o XPM a carregar tendo em conta a pontuação fornecida
 * @param score Pontuação
 * @param scoreSprite Sprite a ser atualizado com o novo pixmap carregado do XPM
*/
void (chooseXPMScore)(int score, Sprite *scoreSprite);

/**
 * @brief Liberta a memória alocada para os Sprites das pontuações
*/
void (freeXPMScore)();

#endif
