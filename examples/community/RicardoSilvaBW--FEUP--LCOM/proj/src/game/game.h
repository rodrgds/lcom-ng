#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "../config.h"
#include "../model/note.h"
#include "../model/sprite.h"

typedef enum {
    MODE_BASIC,
    MODE_CHALLENGE,
    MODE_HARDCORE
} game_mode_t;

extern game_mode_t current_mode;
extern double current_note_speed;

extern note notes[NUM_LANES];

extern sprite laneSprites[NUM_LANES];
extern sprite *target_sprites[NUM_LANES];
extern sprite *lit_arrow_sprites[NUM_LANES];
extern sprite *clock_sprite;
extern sprite *score_sprite;
extern sprite *background_sprite;
extern sprite *idle_cheerleader_sprite;
extern sprite *cheerleader_poses[NUM_LANES];
extern sprite *cheerleader_hc_pose_sprite;
extern sprite *cursor_sprite;
extern sprite *heart_sprite;
extern sprite *heart_hit_sprite;

extern int remaining_secs;
extern int score;
extern int current_cheer_pose;
extern int cheer_pose_timer;
extern int notes_since_heart;
extern int lane_x[NUM_LANES];

int (game_run)(uint8_t kb_irq_set, uint8_t mouse_irq_set, uint8_t timer_irq_set);

#endif
