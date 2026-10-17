#ifndef _SRC_GAME_LOGIC_H_
#define _SRC_GAME_LOGIC_H_

#include "XPMs/background_cube.xpm"
#include "XPMs/end_screen.xpm"
#include "XPMs/finish_button.xpm"
#include "XPMs/ice_cube.xpm"
#include "XPMs/key.xpm"
#include "XPMs/level1.xpm"
#include "XPMs/level10.xpm"
#include "XPMs/level2.xpm"
#include "XPMs/level3.xpm"
#include "XPMs/level4.xpm"
#include "XPMs/level5.xpm"
#include "XPMs/level6.xpm"
#include "XPMs/level7.xpm"
#include "XPMs/level8.xpm"
#include "XPMs/level9.xpm"
#include "XPMs/lock.xpm"
#include "XPMs/mouse.xpm"
#include "XPMs/path_cube.xpm"
#include "XPMs/player.xpm"
#include "XPMs/quit_button.xpm"
#include "XPMs/red_cube.xpm"
#include "XPMs/start_button.xpm"
#include "XPMs/thin_ice_game.xpm"
#include "XPMs/thin_ice_start.xpm"
#include "XPMs/water.xpm"
#include "devices/keyboard/KBC.h"
#include "devices/mouse/mouse.h"
#include "devices/video/graphics.h"
#include "game_view.h"
#include "sprite.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#define MODE 0x115
#define FPS 60

/**
 * @brief Define se o jogo está a correr.
 *
 */
typedef enum {
  RUNNING,
  EXIT,
} GameState;

/**
 * @brief Define o menu em que o jogo se encontra.
 *
 */
typedef enum {
  START,
  GAME,
  FINISH
} MenuState;

/**
 * @brief Define o nível em que o jogo se encontra.
 *
 */
typedef enum {
  LEVEL_1,
  LEVEL_2,
  LEVEL_3,
  LEVEL_4,
  LEVEL_5,
  LEVEL_6,
  LEVEL_7,
  LEVEL_8,
  LEVEL_9,
  LEVEL_10
} GameLevel;

/**
 * @brief Define as coordenadas do player.
 *
 */
typedef struct {
  int x;
  int y;
} PlayerPosition;

/**
 * @brief Incrementa o counter das interrupções do timer e dá swap aos buffers.
 *
 */
void update_timer();

/**
 * @brief Gere os inputs do teclado e atribui funcionalidade às teclas.
 *
 */
void update_keyboard();

/**
 * @brief Gere os inputs do rato. Processa interrupts e chama funções para atualizar a informação do rato e verificar se clicou em algo relevante.
 *
 */
void update_mouse();

/**
 * @brief Verifica se o rato clica nos botões que permitem ao jogador começar ou acabar o jogo e muda o estado do jogo consoante o botão.
 *
 */
void update_start_and_finish_button();

/**
 * @brief Cria as todas as sprites necessárias para o jogo.
 *
 */
void sprites_creation();

#endif
