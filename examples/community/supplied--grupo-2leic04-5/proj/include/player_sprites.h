#ifndef _PLAYER_SPRITES_H_
#define _PLAYER_SPRITES_H_

#include <lcom/xpm.h>

#include "../media/sprites/player/player_0.xpm"
#include "../media/sprites/player/player_1.xpm"
#include "../media/sprites/player/player_2.xpm"
#include "../media/sprites/player/player_3.xpm"
#include "../media/sprites/player/player_4.xpm"
#include "../media/sprites/player/player_5.xpm"
#include "../media/sprites/player/player_6.xpm"
#include "../media/sprites/player/player_7.xpm"
#include "../media/sprites/player/player_8.xpm"
#include "../media/sprites/player/player_9.xpm"
#include "../media/sprites/player/player_frozen.xpm"

#define PLAYER_SPRITE_COUNT 10

// array de mapas dos sprites de animação do jogador (sem o frozen, que é carregado separadamente)
static xpm_map_t const player_sprite_maps[PLAYER_SPRITE_COUNT] = {
  (xpm_map_t) player_0_xpm,
  (xpm_map_t) player_1_xpm,
  (xpm_map_t) player_2_xpm,
  (xpm_map_t) player_3_xpm,
  (xpm_map_t) player_4_xpm,
  (xpm_map_t) player_5_xpm,
  (xpm_map_t) player_6_xpm,
  (xpm_map_t) player_7_xpm,
  (xpm_map_t) player_8_xpm,
  (xpm_map_t) player_9_xpm
};

#endif  // _PLAYER_SPRITES_H_
