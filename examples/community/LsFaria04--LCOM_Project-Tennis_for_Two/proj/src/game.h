#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "states.h"
#include "player.h"
#include "player2.h"
#include "ball.h"
#include "game_score.h"
#include "menu.h"

#include "devices/i8042.h"
#include "devices/i8254.h"
#include "devices/kbd.h"
#include "devices/mouse.h"
#include "devices/rtc.h"
#include "devices/VBE.h"
#include "devices/video.h"
#include "xpms.h"

#define NET_Y_POS 370

/**
 * @brief Loop principal do jogo onde se encontra a função driver receive
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (gameLoop)();

/**
 * @brief Executa ações que necessitam das informações fornecidas pelo Timer após uma interrupção deste (atualizar posições, gerar novos frames, animações, etc..)
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (timerHandler)();

/**
 * @brief Executa ações relacionadas com o Keyboard após uma interrupção deste (atualizar a direção de movimento dos jogadores, ...)
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (keyboardHandler)();

/**
 * @brief Executa ações relacionadas com o Mouse após uma interrupção deste (Iniciar o movimento de batida na bola, mover o cursor no menu, ...)
 * @returns 1 se ocorreu algum erro ou zero caso tenha corrido tudo bem
*/
int (mouseHandler)();

/**
 * @brief Carrega o background no inicio da execução do programa
*/
int (loadBackground)();

/**
 * @brief Liberta a memória alocada pelo programa durante a sua execução
*/
void (destroyElements)();



#endif
