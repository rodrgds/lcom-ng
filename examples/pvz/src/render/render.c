#include "pvz_platform.h"
#include "board.h"
#include "constants.h"
#include "render.h"

static void draw_potato_frame(AnimSprite *sprite, int frame, int width, int height,
                              int x_offset, int y_offset, int cell_x, int cell_y) {
    if (sprite == NULL || sprite->pixmaps == NULL || sprite->num_frames == 0) return;
    if (frame < 0 || frame >= sprite->num_frames || sprite->pixmaps[frame] == NULL) return;

    int x = cell_x + (65 - width) / 2 + x_offset;
    int y = cell_y + 56 - height + y_offset;

    vg_draw_pixmap(sprite->pixmaps[frame], width, height, x, y);
}

static void draw_potato_explosion(Plant *plant, int cell_x, int cell_y) {
    AnimSprite *sprite = get_potato_mine_explosion_sprite();
    int frame = plant->explosion_frame;
    if (sprite == NULL || sprite->pixmaps == NULL || sprite->num_frames == 0) return;
    if (frame >= sprite->num_frames) frame = sprite->num_frames - 1;

    int width = get_potato_mine_explosion_frame_width(frame);
    int height = get_potato_mine_explosion_frame_height(frame);
    int x = cell_x + (65 - width) / 2 + potato_mine_explosion_x_offsets[frame];
    int y = cell_y - 8 + potato_mine_explosion_y_offsets[frame];

    vg_draw_pixmap(sprite->pixmaps[frame], width, height, x, y);
}

void draw_plant(Plant* plant) {
    int screen_x = COL_START_X[plant->col];
    int screen_y = ROW_START_Y[plant->row];
    ShooterPlant *shooter = plant->class == SHOOTER ? &plant->shooter : NULL;

    AnimSprite *sprite = NULL;
    const int *x_offset = NULL;
    const int *y_offset = NULL;

    switch (plant->type) {
        case SUNFLOWER:
            sprite = get_sunflower_idle_sprite();
            x_offset = sunflower_x_offsets;
            y_offset = sunflower_y_offsets;
            break;
        case PEASHOOTER:
            if (shooter != NULL && shooter->isShooting) {
                sprite = get_peashooter_shooting_sprite();
                x_offset = peashooter_shooting_x_offsets;
                y_offset = peashooter_shooting_y_offsets;
            } else {
                sprite = get_peashooter_idle_sprite();
                x_offset = peashooter_x_offsets;
                y_offset = peashooter_y_offsets;
            }
            break;
        case REPEATER:
            if (shooter != NULL && shooter->isShooting) {
                sprite = get_repeater_shooting_sprite();
                x_offset = repeater_shooting_x_offsets;
                y_offset = repeater_shooting_y_offsets;
            } else {
                sprite = get_repeater_idle_sprite();
                x_offset = repeater_x_offsets;
                y_offset = repeater_y_offsets;
            }
            break;
        case CABBAGE:
            if (shooter != NULL && shooter->isShooting) {
                sprite = get_cabbage_shooting_sprite();
                x_offset = cabbage_shooting_x_offsets;
                y_offset = cabbage_shooting_y_offsets;
            } else {
                sprite = get_cabbage_idle_sprite();
                x_offset = cabbage_x_offsets;
                y_offset = cabbage_y_offsets;
            }
            break;
        case WALL_NUT:
            sprite = get_wall_nut_idle_sprite();
            x_offset = wall_nut_x_offsets;
            y_offset = wall_nut_y_offsets;
            break;
        case SPIKEWEED:
            sprite = plant->is_attacking ? get_spikeweed_attack_sprite() : get_spikeweed_idle_sprite();
            x_offset = plant->is_attacking ? spikeweed_attack_x_offsets : spikeweed_idle_x_offsets;
            y_offset = plant->is_attacking ? spikeweed_attack_y_offsets : spikeweed_idle_y_offsets;
            break;
        case POTATO_MINE:
            if (plant->is_exploding) {
                draw_potato_explosion(plant, screen_x, screen_y);
            } else if (plant->is_armed) {
                AnimSprite *potato = get_potato_mine_idle_sprite();
                int frame = plant->frame % POTATO_MINE_IDLE_FRAMES;
                draw_potato_frame(potato,
                                  frame,
                                  get_potato_mine_idle_frame_width(frame),
                                  get_potato_mine_idle_frame_height(frame),
                                  potato_mine_idle_x_offsets[frame],
                                  potato_mine_idle_y_offsets[frame],
                                  screen_x,
                                  screen_y);
            } else {
                AnimSprite *potato = get_potato_mine_pop_sprite();
                int frame = plant->arm_timer * POTATO_MINE_POP_FRAMES / POTATO_MINE_ARM_TICKS;
                if (frame >= POTATO_MINE_POP_FRAMES) frame = POTATO_MINE_POP_FRAMES - 1;
                draw_potato_frame(potato,
                                  frame,
                                  get_potato_mine_pop_frame_width(frame),
                                  get_potato_mine_pop_frame_height(frame),
                                  potato_mine_pop_x_offsets[frame],
                                  potato_mine_pop_y_offsets[frame],
                                  screen_x,
                                  screen_y);
            }
            return;
    }

    if (sprite == NULL || sprite->pixmaps == NULL || sprite->num_frames == 0) return;

    int current_frame = plant->frame % sprite->num_frames;
    if ((plant->type == PEASHOOTER || plant->type == REPEATER || plant->type == CABBAGE) &&
        shooter != NULL &&
        shooter->isShooting) {
        current_frame = shooter->shooting_frame % sprite->num_frames;
    } else if (plant->type == SPIKEWEED && plant->is_attacking) {
        int elapsed = SPIKEWEED_ATTACK_ANIM_TICKS - plant->attack_anim_counter;
        current_frame = elapsed * SPIKEWEED_ATTACK_FRAMES / SPIKEWEED_ATTACK_ANIM_TICKS;
        if (current_frame >= SPIKEWEED_ATTACK_FRAMES) {
            current_frame = SPIKEWEED_ATTACK_FRAMES - 1;
        }
    }

    if (sprite->pixmaps[current_frame] == NULL) return;

    uint16_t width = sprite->width;
    uint16_t height = sprite->height;

    if (shooter != NULL && shooter->isShooting) {
        if (plant->type == PEASHOOTER) {
            width = get_peashooter_shooting_frame_width(current_frame);
            height = get_peashooter_shooting_frame_height(current_frame);
        } else if (plant->type == REPEATER) {
            width = get_repeater_shooting_frame_width(current_frame);
            height = get_repeater_shooting_frame_height(current_frame);
        } else if (plant->type == CABBAGE) {
            width = get_cabbage_shooting_frame_width(current_frame);
            height = get_cabbage_shooting_frame_height(current_frame);
        }
    } else if (plant->type == CABBAGE) {
        width = get_cabbage_idle_frame_width(current_frame);
        height = get_cabbage_idle_frame_height(current_frame);
    } else if (plant->type == SPIKEWEED) {
        if (plant->is_attacking) {
            width = get_spikeweed_attack_frame_width(current_frame);
            height = get_spikeweed_attack_frame_height(current_frame);
        } else {
            width = get_spikeweed_idle_frame_width(current_frame);
            height = get_spikeweed_idle_frame_height(current_frame);
        }
    }

    vg_draw_pixmap(
        sprite->pixmaps[current_frame],
        width,
        height,
        screen_x + x_offset[current_frame],
        screen_y + y_offset[current_frame]
    );
}

void draw_map(void) {
    AnimSprite *map = get_map_sprite();
    if (map != NULL && map->pixmaps != NULL && map->pixmaps[0] != NULL) {
        vg_copy_to_buffer(map->pixmaps[0]);
    }
}

void draw_cursor(int x, int y) {
    AnimSprite *cursor = get_cursor_sprite();
    if (cursor != NULL && cursor->pixmaps != NULL && cursor->pixmaps[0] != NULL) {
        vg_draw_pixmap(cursor->pixmaps[0], cursor->width, cursor->height, x, y);
    }
}

void draw_bullet(int x, int y, int frame) {
    AnimSprite *bullet = get_bullet_sprite();
    if (bullet != NULL && bullet->pixmaps != NULL && bullet->num_frames > 0) {
        int current_frame = frame % bullet->num_frames;

        if (bullet->pixmaps[current_frame] != NULL) {
            vg_draw_pixmap(
                bullet->pixmaps[current_frame],
                get_bullet_frame_width(current_frame),
                get_bullet_frame_height(current_frame),
                x,
                y + pea_y_offsets[current_frame]
            );
        }
    }
}

void draw_cabbage_projectile(int x, int y, int frame) {
    AnimSprite *cabbage = get_cabbage_projectile_sprite();
    if (cabbage != NULL && cabbage->pixmaps != NULL && cabbage->num_frames > 0) {
        int current_frame = frame % cabbage->num_frames;

        if (cabbage->pixmaps[current_frame] != NULL) {
            vg_draw_pixmap(
                cabbage->pixmaps[current_frame],
                get_cabbage_projectile_frame_width(current_frame),
                get_cabbage_projectile_frame_height(current_frame),
                x,
                y + cabbage_projectile_y_offsets[current_frame]
            );
        }
    }
}

void draw_sun(int x, int y, int frame) {
    AnimSprite *sun = get_sun_sprite();
    if (sun != NULL && sun->pixmaps != NULL && sun->num_frames > 0) {
        int current_frame = frame % sun->num_frames;

        if (sun->pixmaps[current_frame] != NULL) {
            vg_draw_pixmap(sun->pixmaps[current_frame], sun->width, sun->height, x, y);
        }
    }
}

void draw_lawnmowers(void) {
    AnimSprite *sprite = get_lawnmower_sprite();
    const Lawnmower *lawnmowers = get_lawnmowers();

    if (sprite == NULL || sprite->pixmaps == NULL || sprite->pixmaps[0] == NULL) {
        return;
    }

    for (int r = 0; r < GRID_ROWS; r++) {
        if (!lawnmowers[r].available && !lawnmowers[r].rolling) {
            continue;
        }

        vg_draw_pixmap(sprite->pixmaps[0],
                       sprite->width,
                       sprite->height,
                       (int) lawnmowers[r].x,
                       ROW_START_Y[r] + LAWNMOWER_ROW_Y_OFFSET);
    }
}

typedef enum {
    POLE_VAULTING_RENDER_WALKING,
    POLE_VAULTING_RENDER_JUMPING,
    POLE_VAULTING_RENDER_NOPOLE,
    POLE_VAULTING_RENDER_EATING
} PoleVaultingRenderState;

static PoleVaultingRenderState get_pole_vaulting_render_state(Zombie *zombie) {
    if (is_zombie_jumping(zombie)) {
        return POLE_VAULTING_RENDER_JUMPING;
    }

    if (pole_vaulting_zombie_has_pole(zombie)) {
        return POLE_VAULTING_RENDER_WALKING;
    }

    return zombie->attacking ? POLE_VAULTING_RENDER_EATING : POLE_VAULTING_RENDER_NOPOLE;
}

static AnimSprite* get_pole_vaulting_sprite(Zombie *zombie) {
    switch (get_pole_vaulting_render_state(zombie)) {
        case POLE_VAULTING_RENDER_JUMPING:
            return get_pole_vaulting_jump_sprite();
        case POLE_VAULTING_RENDER_WALKING:
            return get_pole_vaulting_walking_sprite();
        case POLE_VAULTING_RENDER_EATING:
            return get_pole_vaulting_eating_sprite();
        case POLE_VAULTING_RENDER_NOPOLE:
        default:
            return get_pole_vaulting_nopole_sprite();
    }
}

static void get_pole_vaulting_offsets(Zombie *zombie, const int **x_offsets, const int **y_offsets) {
    switch (get_pole_vaulting_render_state(zombie)) {
        case POLE_VAULTING_RENDER_JUMPING:
            *x_offsets = pole_vaulting_jump_x_offsets;
            *y_offsets = pole_vaulting_jump_y_offsets;
            break;
        case POLE_VAULTING_RENDER_WALKING:
            *x_offsets = pole_vaulting_walking_x_offsets;
            *y_offsets = pole_vaulting_walking_y_offsets;
            break;
        case POLE_VAULTING_RENDER_EATING:
            *x_offsets = pole_vaulting_eating_x_offsets;
            *y_offsets = pole_vaulting_eating_y_offsets;
            break;
        case POLE_VAULTING_RENDER_NOPOLE:
        default:
            *x_offsets = pole_vaulting_nopole_x_offsets;
            *y_offsets = pole_vaulting_nopole_y_offsets;
            break;
    }
}

static void get_pole_vaulting_frame_dimensions(Zombie *zombie, int frame, uint16_t *width, uint16_t *height) {
    switch (get_pole_vaulting_render_state(zombie)) {
        case POLE_VAULTING_RENDER_JUMPING:
            *width = get_pole_vaulting_jump_frame_width(frame);
            *height = get_pole_vaulting_jump_frame_height(frame);
            break;
        case POLE_VAULTING_RENDER_WALKING:
            *width = get_pole_vaulting_walking_frame_width(frame);
            *height = get_pole_vaulting_walking_frame_height(frame);
            break;
        case POLE_VAULTING_RENDER_EATING:
            *width = get_pole_vaulting_eating_frame_width(frame);
            *height = get_pole_vaulting_eating_frame_height(frame);
            break;
        case POLE_VAULTING_RENDER_NOPOLE:
        default:
            *width = get_pole_vaulting_nopole_frame_width(frame);
            *height = get_pole_vaulting_nopole_frame_height(frame);
            break;
    }
}

static AnimSprite* get_zombie_sprite(Zombie *zombie) {
    switch (zombie->type) {
        case POLE_VAULTING_ZOMBIE:
            return get_pole_vaulting_sprite(zombie);
        case CONEHEAD_ZOMBIE:
            return zombie->attacking ? get_conehead_eating_sprite() : get_conehead_walking_sprite();
        case REGULAR_ZOMBIE:
        default:
            return zombie->attacking ? get_regular_eating_sprite() : get_regular_walking_sprite();
    }
}

static void get_zombie_offsets(Zombie *zombie, const int **x_offsets, const int **y_offsets) {
    switch (zombie->type) {
        case POLE_VAULTING_ZOMBIE:
            get_pole_vaulting_offsets(zombie, x_offsets, y_offsets);
            break;
        case CONEHEAD_ZOMBIE:
            *x_offsets = zombie->attacking ? conehead_eating_x_offsets : conehead_walking_x_offsets;
            *y_offsets = zombie->attacking ? conehead_eating_y_offsets : conehead_walking_y_offsets;
            break;
        case REGULAR_ZOMBIE:
        default:
            *x_offsets = zombie->attacking ? regular_eating_x_offsets : regular_walking_x_offsets;
            *y_offsets = zombie->attacking ? regular_eating_y_offsets : regular_walking_y_offsets;
            break;
    }
}

static void get_zombie_frame_dimensions(Zombie *zombie, int frame, uint16_t *width, uint16_t *height) {
    if (zombie->type == POLE_VAULTING_ZOMBIE) {
        get_pole_vaulting_frame_dimensions(zombie, frame, width, height);
        return;
    }

    AnimSprite *sprite = get_zombie_sprite(zombie);
    *width = sprite->width;
    *height = sprite->height;
}

void draw_zombie(Zombie* zombie) {
    int screen_x = COL_START_X[GRID_COLS - 1];
    int screen_y = ROW_START_Y[zombie->row];

    AnimSprite *body_sprite = get_zombie_sprite(zombie);
    const int *body_x_offsets = NULL;
    const int *body_y_offsets = NULL;
    get_zombie_offsets(zombie, &body_x_offsets, &body_y_offsets);

    if (body_sprite == NULL || body_sprite->pixmaps == NULL || body_sprite->num_frames == 0) return;

    int current_frame = zombie->frame % body_sprite->num_frames;
    uint16_t width;
    uint16_t height;
    if (body_sprite->pixmaps[current_frame] == NULL) return;
    get_zombie_frame_dimensions(zombie, current_frame, &width, &height);

    int zombie_x = screen_x + zombie->curr_position + body_x_offsets[current_frame];
    int zombie_y = screen_y + body_y_offsets[current_frame];

    vg_draw_pixmap(body_sprite->pixmaps[current_frame], width, height, zombie_x, zombie_y);
}

int load_all_sprites(void) {
    if (load_sunflower_sprites() != 0) goto fail;
    if (load_peashooter_sprites() != 0) goto fail;
    if (load_repeater_sprites() != 0) goto fail;
    if (load_cabbage_sprites() != 0) goto fail;
    if (load_wall_nut_sprites() != 0) goto fail;
    if (load_potato_mine_sprites() != 0) goto fail;
    if (load_spikeweed_sprites() != 0) goto fail;
    if (load_regular_walking_sprites() != 0) goto fail;
    if (load_regular_eating_sprites() != 0) goto fail;
    if (load_cone_zombie_sprites() != 0) goto fail;
    if (load_pole_vaulting_zombie_sprites() != 0) goto fail;
    if (load_map_sprite() != 0) goto fail;
    if (load_cursor_sprite() != 0) goto fail;
    if (load_bullet_sprite() != 0) goto fail;
    if (load_cabbage_projectile_sprite() != 0) goto fail;
    if (load_sun_sprite() != 0) goto fail;
    if (load_hud_sprite() != 0) goto fail;
    if (load_shovel_sprite() != 0) goto fail;
    if (load_lawnmower_sprite() != 0) goto fail;
    if (load_seed_packet_sprite() != 0) goto fail;
    return 0;

fail:
    free_all_sprites();
    return 1;
}

void free_all_sprites(void) {
    free_sunflower_sprites();
    free_peashooter_sprites();
    free_repeater_sprites();
    free_cabbage_sprites();
    free_wall_nut_sprites();
    free_potato_mine_sprites();
    free_spikeweed_sprites();
    free_regular_walking_sprites();
    free_regular_eating_sprites();
    free_cone_zombie_sprites();
    free_pole_vaulting_zombie_sprites();
    free_map_sprite();
    free_cursor_sprite();
    free_bullet_sprite();
    free_cabbage_projectile_sprite();
    free_sun_sprite();
    free_hud_sprite();
    free_shovel_sprite();
    free_lawnmower_sprite();
    free_seed_packet_sprite();
}
