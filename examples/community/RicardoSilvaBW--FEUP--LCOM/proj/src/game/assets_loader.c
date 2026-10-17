#include <lcom/lcf.h>
#include <stdlib.h>

#include "assets_loader.h"
#include "game.h"

#include "../config.h"
#include "../controller/video/graphics.h"
#include "../model/note.h"
#include "../model/sprite.h"

#include "../../assets/notes/arrow_up.xpm"
#include "../../assets/notes/arrow_up_lit.xpm"
#include "../../assets/notes/arrow_down.xpm"
#include "../../assets/notes/arrow_down_lit.xpm"
#include "../../assets/notes/arrow_left.xpm"
#include "../../assets/notes/arrow_left_lit.xpm"
#include "../../assets/notes/arrow_right.xpm"
#include "../../assets/notes/arrow_right_lit.xpm"

#include "../../assets/notes/mouse-click-for-hardcore.xpm"
#include "../../assets/notes/mouse-click-for-hardcore-hit.xpm"

#include "../../assets/cheerleader-poses/cheer_arms_up.xpm"
#include "../../assets/cheerleader-poses/cheer_combo.xpm"
#include "../../assets/cheerleader-poses/cheer_idle.xpm"
#include "../../assets/cheerleader-poses/cheer_jump.xpm"
#include "../../assets/cheerleader-poses/cheer_kick.xpm"
#include "../../assets/cheerleader-poses/cheer_step_left.xpm"
#include "../../assets/cheerleader-poses/cheer_step_right.xpm"

#include "../../assets/notes/arrow_target.xpm"

#include "../../assets/backgrounds/bg_court.xpm"
#include "../../assets/other/cursor.xpm"

#include "../../assets/other/clock.xpm"
#include "../../assets/other/star.xpm"

static xpm_map_t arrow_xpms[NUM_LANES] = {
    (xpm_map_t) arrow_up_xpm,
    (xpm_map_t) arrow_down_xpm,
    (xpm_map_t) arrow_left_xpm,
    (xpm_map_t) arrow_right_xpm,
};

static xpm_map_t cheerleader_poses_xpms[NUM_LANES] = {
    (xpm_map_t) cheer_arms_up_xpm,
    (xpm_map_t) cheer_combo_xpm,
    (xpm_map_t) cheer_step_left_xpm,
    (xpm_map_t) cheer_step_right_xpm,
};

static xpm_map_t lit_arrow_xpms[NUM_LANES] = {
    (xpm_map_t) arrow_up_lit_xpm,
    (xpm_map_t) arrow_down_lit_xpm,
    (xpm_map_t) arrow_left_lit_xpm,
    (xpm_map_t) arrow_right_lit_xpm,
};

void (destroy_sprites)() {
    for (int i = 0; i < NUM_LANES; i++) {
        destroy_sprite(&target_sprites[i]);
        destroy_sprite(&lit_arrow_sprites[i]);
        destroy_sprite(&cheerleader_poses[i]);

        if (laneSprites[i].colors != NULL) {
            free(laneSprites[i].colors);
            laneSprites[i].colors = NULL;
        }
    }

    destroy_sprite(&clock_sprite);
    destroy_sprite(&background_sprite);
    destroy_sprite(&idle_cheerleader_sprite);
    destroy_sprite(&cursor_sprite);
    destroy_sprite(&score_sprite);
    destroy_sprite(&heart_sprite);
    destroy_sprite(&heart_hit_sprite);
    destroy_sprite(&cheerleader_hc_pose_sprite);
}

void (initialize_notes)() {
    lane_x[0] = LANE_START_X;
    lane_x[1] = LANE_START_X + LANE_SPACING;
    lane_x[2] = LANE_START_X + 2 * LANE_SPACING;
    lane_x[3] = LANE_START_X + 3 * LANE_SPACING;

    for (int i = 0; i < NUM_LANES; i++) {
        xpm_image_t img;
        uint8_t *colors = xpm_load(arrow_xpms[i], XPM_8_8_8_8, &img);
        
        laneSprites[i].x = lane_x[i] - (img.width * NOTE_SCALE)/2;
        laneSprites[i].y = get_v_res() + (i*80);
        laneSprites[i].width = img.width * NOTE_SCALE;
        laneSprites[i].height = img.height * NOTE_SCALE;
        laneSprites[i].colors = colors;
        laneSprites[i].img = img;

        note_init(&notes[i], &laneSprites[i], current_note_speed, (note_lane) i);
    }
}

void (initialize_hud_assets)(void) {
    background_sprite = create_sprite((xpm_map_t) bg_court_xpm, 0, 0);

    cursor_sprite = create_sprite((xpm_map_t) cursor_xpm, 0, 0);

    idle_cheerleader_sprite = create_sprite((xpm_map_t) cheer_idle_xpm, 0, 0);
    idle_cheerleader_sprite->x = get_h_res() - 250;
    idle_cheerleader_sprite->y = get_v_res() - 350;

    clock_sprite = create_sprite((xpm_map_t) clock_xpm, 10, 10);

    for (int i = 0; i < NUM_LANES; i++) {
        int tx = lane_x[i] - 8;
        target_sprites[i] = create_sprite((xpm_map_t) arrow_target_xpm, tx, ACCEPTANCE_Y_TOP);
        lit_arrow_sprites[i] = create_sprite(lit_arrow_xpms[i], tx, ACCEPTANCE_Y_TOP);
        lit_arrow_sprites[i]->width = lit_arrow_sprites[i]->img.width * NOTE_SCALE;
        lit_arrow_sprites[i]->height = lit_arrow_sprites[i]->img.height * NOTE_SCALE;
        cheerleader_poses[i] = create_sprite(cheerleader_poses_xpms[i], idle_cheerleader_sprite->x, idle_cheerleader_sprite->y);
    }

    heart_sprite = create_sprite((xpm_map_t) mouse_click_for_hardcore_xpm, 0, 0);
    heart_sprite->width = heart_sprite->img.width * 2;
    heart_sprite->height = heart_sprite->img.height * 2;
    heart_hit_sprite = create_sprite((xpm_map_t) mouse_click_for_hardcore_hit_xpm, 0, 0);
    heart_hit_sprite->width = heart_hit_sprite->img.width * 2;
    heart_hit_sprite->height = heart_hit_sprite->img.height * 2;

    cheerleader_hc_pose_sprite = create_sprite((xpm_map_t) cheer_jump_xpm, 0, 0);
    cheerleader_hc_pose_sprite->x = idle_cheerleader_sprite->x;
    cheerleader_hc_pose_sprite->y = idle_cheerleader_sprite->y;

    score_sprite = create_sprite((xpm_map_t) star_xpm, 0, 0);
}

void (assign_note_type)(note* n) {
    if (current_mode != MODE_HARDCORE) n->type = ARROW_NOTE;
    else if (notes_since_heart >= HEART_MIN_GAP && (rand() % 100) < HEART_SPAWN_PROB_PERCENTAGE) {
        n->type = HEART_NOTE;
        notes_since_heart = 0;
    }
    else {
        n->type = ARROW_NOTE;
        notes_since_heart++;
    }
}
