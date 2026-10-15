#include "../include/init.h"

#include <stdlib.h>

// Include XPMs for asset loading
#include "../assets/background.xpm"
#include "../assets/title.xpm"
#include "../assets/cursor_point.xpm"
#include "../assets/mascote/mascote1.xpm"
#include "../assets/mascote/mascote2.xpm"
#include "../assets/mascote/mascote3.xpm"
#include "../assets/mascote/mascote4.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose01.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose02.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose03.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose04.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose05.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose06.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose07.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose08.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose09.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose10.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose11.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose12.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose13.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose14.xpm"
#include "../assets/mascote/SushiWarriorLose/SushiWarriorLose15.xpm"
#include "../assets/heart.xpm"
#include "../assets/sushi_pieces/bomb.xpm"

static int allocate_sprite_array(sprite_t *sprites[], size_t count) {
    if (sprites == NULL) return 1;

    for (size_t i = 0; i < count; i++) {
        sprites[i] = malloc(sizeof(sprite_t));
        if (sprites[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(sprites[j]);
                sprites[j] = NULL;
            }
            return 1;
        }
    }

    return 0;
}

static void destroy_sprite_array(sprite_t *sprites[], size_t count) {
    if (sprites == NULL) return;

    for (size_t i = 0; i < count; i++) {
        if (sprites[i] != NULL) {
            sprite_destroy(sprites[i]);
            free(sprites[i]);
            sprites[i] = NULL;
        }
    }
}

static void assets_destroy_loaded(Assets *assets) {
    if (assets == NULL) return;

    sprite_destroy(&assets->background_sprite);
    sprite_destroy(&assets->name_sprite);
    font_destroy(&assets->game_font);
    sprite_destroy(&assets->mouse_cursor);
    sprite_destroy(&assets->heart);
    sprite_destroy(&assets->bomb);
    destroy_sprite_array(assets->mascote, MASCOT_IDLE_FRAME_COUNT);
    destroy_sprite_array(assets->mascote_lose, MASCOT_LOSE_FRAME_COUNT);
}

int assets_init(Assets *assets) {
    if (assets == NULL) return 1;

    if (sprite_load(&assets->background_sprite, (xpm_map_t)background_xpm) != 0) {
        return 1;
    }

    if (sprite_load(&assets->name_sprite, (xpm_map_t)title_xpm) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    if (font_create(&assets->game_font, 16) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    if (sprite_load(&assets->mouse_cursor, (xpm_map_t)cursor_point_xpm) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    if (sprite_load(&assets->heart, (xpm_map_t)heart_xpm) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    if (sprite_load(&assets->bomb, (xpm_map_t)bomb_xpm) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    if (allocate_sprite_array(assets->mascote, MASCOT_IDLE_FRAME_COUNT) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    if (allocate_sprite_array(assets->mascote_lose, MASCOT_LOSE_FRAME_COUNT) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    xpm_map_t mascote_xpms[MASCOT_IDLE_FRAME_COUNT] = {
        (xpm_map_t)mascote1_xpm,
        (xpm_map_t)mascote2_xpm,
        (xpm_map_t)mascote3_xpm,
        (xpm_map_t)mascote4_xpm
    };

    if (anim_sprite_load(&assets->mascot_anim, assets->mascote, MASCOT_IDLE_FRAME_COUNT, mascote_xpms, ANIM_NORMAL_DELAY) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    xpm_map_t mascote_lose_xpms[MASCOT_LOSE_FRAME_COUNT] = {
        (xpm_map_t)SushiWarriorLose1_xpm,
        (xpm_map_t)SushiWarriorLose2_xpm,
        (xpm_map_t)SushiWarriorLose3_xpm,
        (xpm_map_t)SushiWarriorLose4_xpm,
        (xpm_map_t)SushiWarriorLose5_xpm,
        (xpm_map_t)SushiWarriorLose6_xpm,
        (xpm_map_t)SushiWarriorLose7_xpm,
        (xpm_map_t)SushiWarriorLose8_xpm,
        (xpm_map_t)SushiWarriorLose9_xpm,
        (xpm_map_t)SushiWarriorLose10_xpm,
        (xpm_map_t)SushiWarriorLose11_xpm,
        (xpm_map_t)SushiWarriorLose12_xpm,
        (xpm_map_t)SushiWarriorLose13_xpm,
        (xpm_map_t)SushiWarriorLose14_xpm,
        (xpm_map_t)SushiWarriorLose15_xpm
    };

    if (anim_sprite_load(&assets->mascot_lose_anim, assets->mascote_lose, MASCOT_LOSE_FRAME_COUNT, mascote_lose_xpms, ANIM_NORMAL_DELAY) != 0) {
        assets_destroy_loaded(assets);
        return 1;
    }

    return 0;
}

void assets_destroy(Assets *assets) {
    if (assets == NULL) return;

    assets_destroy_loaded(assets);
}
