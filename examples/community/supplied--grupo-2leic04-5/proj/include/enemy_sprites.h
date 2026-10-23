#ifndef _ENEMY_SPRITES_H_
#define _ENEMY_SPRITES_H_

#include <lcom/xpm.h>

// ALARM sprites (11 frames) - enemy_paralyzer
#include "../media/sprites/alarm/alarm_0.xpm"
#include "../media/sprites/alarm/alarm_1.xpm"
#include "../media/sprites/alarm/alarm_2.xpm"
#include "../media/sprites/alarm/alarm_3.xpm"
#include "../media/sprites/alarm/alarm_4.xpm"
#include "../media/sprites/alarm/alarm_5.xpm"
#include "../media/sprites/alarm/alarm_6.xpm"
#include "../media/sprites/alarm/alarm_7.xpm"
#include "../media/sprites/alarm/alarm_8.xpm"
#include "../media/sprites/alarm/alarm_9.xpm"
#include "../media/sprites/alarm/alarm_10.xpm"

// ANALOG sprites (8 frames) - enemy_walker
#include "../media/sprites/analog/analog_0.xpm"
#include "../media/sprites/analog/analog_1.xpm"
#include "../media/sprites/analog/analog_2.xpm"
#include "../media/sprites/analog/analog_3.xpm"
#include "../media/sprites/analog/analog_4.xpm"
#include "../media/sprites/analog/analog_5.xpm"
#include "../media/sprites/analog/analog_6.xpm"
#include "../media/sprites/analog/analog_7.xpm"

// DIGITAL sprites (7 frames) - enemy_shotter
#include "../media/sprites/digital/digital_0.xpm"
#include "../media/sprites/digital/digital_1.xpm"
#include "../media/sprites/digital/digital_2.xpm"
#include "../media/sprites/digital/digital_3.xpm"
#include "../media/sprites/digital/digital_4.xpm"
#include "../media/sprites/digital/digital_5.xpm"
#include "../media/sprites/digital/digital_6.xpm"

#define ALARM_SPRITE_COUNT 11
#define ANALOG_SPRITE_COUNT 8
#define DIGITAL_SPRITE_COUNT 7

static xpm_map_t const alarm_sprite_maps[ALARM_SPRITE_COUNT] = {
  (xpm_map_t) alarm_0_xpm,
  (xpm_map_t) alarm_1_xpm,
  (xpm_map_t) alarm_2_xpm,
  (xpm_map_t) alarm_3_xpm,
  (xpm_map_t) alarm_4_xpm,
  (xpm_map_t) alarm_5_xpm,
  (xpm_map_t) alarm_6_xpm,
  (xpm_map_t) alarm_7_xpm,
  (xpm_map_t) alarm_8_xpm,
  (xpm_map_t) alarm_9_xpm,
  (xpm_map_t) alarm_10_xpm
};

static xpm_map_t const analog_sprite_maps[ANALOG_SPRITE_COUNT] = {
  (xpm_map_t) analog_0_xpm,
  (xpm_map_t) analog_1_xpm,
  (xpm_map_t) analog_2_xpm,
  (xpm_map_t) analog_3_xpm,
  (xpm_map_t) analog_4_xpm,
  (xpm_map_t) analog_5_xpm,
  (xpm_map_t) analog_6_xpm,
  (xpm_map_t) analog_7_xpm
};

static xpm_map_t const digital_sprite_maps[DIGITAL_SPRITE_COUNT] = {
  (xpm_map_t) digital_0_xpm,
  (xpm_map_t) digital_1_xpm,
  (xpm_map_t) digital_2_xpm,
  (xpm_map_t) digital_3_xpm,
  (xpm_map_t) digital_4_xpm,
  (xpm_map_t) digital_5_xpm,
  (xpm_map_t) digital_6_xpm
};

#endif  // _ENEMY_SPRITES_H_
