#pragma once

#include <lcom/lcf.h>

#define INTERRRUPTS_PER_FRAME_MOUSE 15
#define INTERRUPTS_PER_FRAME_GAME 2


/**
 * @brief - Subscreve e ativa as interrupções de todos os dispositivos; ativa o data reporting do rato.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int subscribe_and_enable_everything();

/**
 * @brief - Retira a subscrição e desativa as interrupções de todos os dispositivos; desativa o data reporting do rato.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
int unsubscribe_everything();

/**
 * Vai buscar a primeira palavra ao ficheiro das palavras (words.txt).
 */
char* getFirstWord();

/**
 * Atribui o nível de dificuldade do jogo, de acordo com o tempo.
 */
void set_level();

/**
 * Muda a velocidade com que as palavras caem, de acordo com o nível de dificuldade do jogo.
 */
void level_handler();

/**
 * Mete e retira o jogo da pausa.
 */
void pauseGameHandlerKeyboard();

/**
 * @brief - Se houver um record, pergunta o nome do jogador.
 * @param topPlayerName - Nome do jogador atual (o que o utilizador já digitou até ao momento).
 * @return - Nome do jogador atual.
 */
char* askNameIfRecordHandlerKeyboard(char* topPlayerName);

/**
 * Atualizada a palavra atual do jogo, de acordo com o que o jogador digita.
 */
void gameWordsHandlerKeyboard();

/**
 * @brief - Trata das interrupções do rato no menu principal.
 * @return - true de for para fazer break no loop principal do jogo; false caso contrário.
 */
bool mainMenuHandlerMouse();

/**
 * @brief - Trata das interrupções do rato na leaderboard.
 * @return - true de for para fazer break no loop principal do jogo; false caso contrário.
 */
bool leaderboardHandlerMouse();

/**
 * @brief - Trata das interrupções do rato nas instruções.
 * @return - true de for para fazer break no loop principal do jogo; false caso contrário.
 */
bool instructionsHandlerMouse();

/**
 * @brief - Trata das interrupções do rato quando se está a pedir o nome do jogador.
 * @param topPlayerName - Nome do jogador atual.
 * @return - true de for para fazer break no loop principal do jogo; false caso contrário.
 */
bool askNameHandlerMouse(char* topPlayerName);

/**
 * @brief - Trata das interrupções do rato no menu do game over.
 * @return - true de for para fazer break no loop principal do jogo; false caso contrário.
 */
bool gamerOverHandlerMouse();

/**
 * @brief - Trata de tudo o que for relacionado com o jogo em si (nível de dificuldade, tempo passado, pontuação, 
 * verificar se o jogador perdeu, verificar se a pontuação obtida corresponde a um recorde, desenhar tudo).
 * @return - true de for para fazer break no loop principal do jogo; false caso contrário.
 */
bool gameHandler();

/**
 * Desenha o fundo da pausa, caso o jogo se encontre em pausa.
 */
void pausedGameHanler();

/**
 * @brief - Desenha o fundo próprio de quando se pede o nome do jogador, assim como o rato.
 * @param topPlayerName - Nome do jogador atual.
 */
void askNameHandler(char* topPlayerName);

/**
 * Desenha o fundo do menu principal, caso o jogo se encontre o menu principal.
 */
void menuHandler();

/**
 * Desenha o fundo da leaderboard, caso o jogo se encontre na leaderboard.
 */
void leaderboardHandler();

/**
 * Desenha o fundo das instruções, caso o jogo se encontre nas instruções.
 */
void instructionsHandler();

/**
 * Desenha o fundo do menu do game over, caso o jogo se encontre aí.
 */
void gameOverHandler();


