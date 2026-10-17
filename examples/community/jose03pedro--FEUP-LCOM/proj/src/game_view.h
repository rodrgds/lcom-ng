#ifndef _SRC_GAME_VIEW_H_
#define _SRC_GAME_VIEW_H_

#include "devices/mouse/mouse.h"
#include "devices/video/graphics.h"
#include "game_levels/level1.h"
#include "game_levels/level10.h"
#include "game_levels/level2.h"
#include "game_levels/level3.h"
#include "game_levels/level4.h"
#include "game_levels/level5.h"
#include "game_levels/level6.h"
#include "game_levels/level7.h"
#include "game_levels/level8.h"
#include "game_levels/level9.h"
#include "game_logic.h"
#include "sprite.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

/**
 * @brief Define os limites de uma caixa, utilizado nos blocos que têm uma função no jogo.
 *
 */
typedef struct {
  int top_left_x;
  int top_left_y;
  int bot_right_x;
  int bot_right_y;
} Box;

/**
 * @brief Aloca memória para os 2 buffers.
 *
 * @param mode
 * @return int
 */
void set_frame_buffers(uint16_t mode);
/**
 * @brief Verifica que frame deve ser desenhado consoante gameState e menuState e chama as funções necessárias para o desenhar.
 *
 */
void draw_new_screen();
/**
 * @brief Desenha o menu inicial e os botões relevantes.
 *
 */
void draw_initial_menu();
/**
 * @brief Desenha a interface do jogo, a "consola" e o background.
 *
 */
void draw_game_background();
/**
 * @brief Verifica se o player caiu na água e reinicia o nível se for o caso.
 *
 */
void check_fell_into_water();
/**
 * @brief Verifica se o player se moveu e transforma a posição anterior em água.
 *
 */
void ice_melt();
/**
 * @brief Compara o water_counter e o path_counter, verificando se o player derreteu todo o gelo para poder passar de nível.
 *
 * @return true
 * @return false
 */
bool check_all_ice_melted();
/**
 * @brief Põe os valores de todos os draw_counters a 0.
 *
 */
void reset_all_draw_counters();
/**
 * @brief Desenha o menu final.
 *
 */
void draw_finish_menu();
/**
 * @brief Desenha o cursor do rato onde necessário.
 *
 */
void draw_mouse();
/**
 * @brief Desenha uma determinada sprite nas coordenadas (x, y).
 *
 * @param sp
 * @param x
 * @param y
 * @return int
 */
void draw_sprite(Sprite_t *sp, int x, int y);

#endif
