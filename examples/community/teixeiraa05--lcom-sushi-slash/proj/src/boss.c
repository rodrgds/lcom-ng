#include "../include/game/boss.h"
#include "../include/game/game_rand.h"
#include <stdlib.h>
#include <string.h>
#include "../include/macros/global.h"
#include "../include/macros/physics.h"
#include "../assets/sushi_pieces/SushiRoll4.xpm"

static void boss_randomize_key_position(boss_t *boss) {
    if (boss == NULL) return;

    uint16_t key_width = boss->combo.current_letter.width == 0 ? 64 : boss->combo.current_letter.width;
    uint16_t key_height = boss->combo.current_letter.height == 0 ? 64 : boss->combo.current_letter.height;
    int min_x = 100;
    int min_y = 180;
    int max_x = SCREEN_WIDTH - key_width - 100;
    int max_y = SCREEN_HEIGHT - key_height - 220;

    if (max_x < min_x) max_x = min_x;
    if (max_y < min_y) max_y = min_y;

    combo_set_position(&boss->combo,
                       (uint16_t)(min_x + game_rand(&boss->prng_seed) % (max_x - min_x + 1)),
                       (uint16_t)(min_y + game_rand(&boss->prng_seed) % (max_y - min_y + 1)));
}

boss_t boss_init(combo_t combo,
                 uint32_t start_tick,
                 uint32_t total_ticks,
                 uint32_t per_key_ticks,
                 uint32_t *prng_seed) {
    boss_t boss;
    memset(&boss, 0, sizeof(boss));
    boss.prng_seed = *prng_seed;

    sprite_t *sprite = malloc(sizeof(sprite_t));
    if (sprite != NULL && sprite_load(sprite, (xpm_map_t)SushiRoll4_xpm) != 0) {
        free(sprite);
        sprite = NULL;
    }

    uint16_t sprite_width = sprite == NULL ? 96 : sprite->width;
    int side = game_rand(&boss.prng_seed) % 2;

    int y0 = SCREEN_HEIGHT + SUSHI_SPAWN_Y_OFFSET;
    int x0 = side == 0 ? 100 : SCREEN_WIDTH - 100 - sprite_width;

    int peak_y = 100;
    int h = y0 - peak_y;

    uint32_t flight_ticks = 600;
    long divisor = (long)flight_ticks * flight_ticks;
    long gravity = (8L * h * PHYSICS_SCALE + divisor / 2) / divisor;
    if (gravity < 1) gravity = 1;

    long vy = - (gravity * ((long)flight_ticks - 1)) / 2;

    int x_end = side == 0 ? SCREEN_WIDTH - 100 - sprite_width : 100;
    long vx = ((long)(x_end - x0) * PHYSICS_SCALE) / (long)flight_ticks;

    boss.sushi = create_sushi_boss(0, 0, 0, 0, 0, sprite, 0);
    boss.sushi.x = PHYSICS_FROM_PIXELS(x0);
    boss.sushi.y = PHYSICS_FROM_PIXELS(y0);
    boss.sushi.vx = (int)vx;
    boss.sushi.vy = (int)vy;
    boss.sushi.gravity = (int)gravity;
    boss.combo = combo;
    boss.countdown = total_ticks;
    boss.start_tick = start_tick;
    boss.current_key_start_tick = start_tick;
    boss.total_ticks = total_ticks;
    boss.per_key_ticks = per_key_ticks;
    boss_randomize_key_position(&boss);
    *prng_seed = boss.prng_seed;
    
    return boss;
}

boss_input_result_t boss_verify_combo(boss_t* boss, char input, uint32_t current_tick) {
    if (boss == NULL) return BOSS_INPUT_WRONG;

    if (input >= 'a' && input <= 'z') {
        input = (char)(input - 'a' + 'A');
    }

    char expected = combo_get_current(&boss->combo);
    
    if (expected == 0 || input != expected) {
        return BOSS_INPUT_WRONG;
    }
    
    combo_advance(&boss->combo);
    boss->current_key_start_tick = current_tick;
    if (!combo_is_complete(&boss->combo)) {
        boss_randomize_key_position(boss);
    }

    return combo_is_complete(&boss->combo) ? BOSS_INPUT_COMPLETE : BOSS_INPUT_CORRECT;
}

boss_status_t boss_update(boss_t* boss, uint32_t current_tick) {
    if (boss == NULL) return BOSS_STATUS_FAILURE;
    if (combo_is_complete(&boss->combo)) {
        return BOSS_STATUS_SUCCESS;
    }

    update_sushi(&boss->sushi);

    boss->countdown = boss_total_ticks_left(boss, current_tick);
    if (boss->countdown == 0 || boss_key_ticks_left(boss, current_tick) == 0) {
        return BOSS_STATUS_FAILURE;
    }

    return BOSS_STATUS_ACTIVE;
}

uint32_t boss_total_ticks_left(const boss_t* boss, uint32_t current_tick) {
    if (boss == NULL) return 0;
    if (current_tick <= boss->start_tick) return boss->total_ticks;

    uint32_t elapsed = current_tick - boss->start_tick;
    return elapsed >= boss->total_ticks ? 0 : boss->total_ticks - elapsed;
}

uint32_t boss_key_ticks_left(const boss_t* boss, uint32_t current_tick) {
    if (boss == NULL) return 0;
    if (current_tick <= boss->current_key_start_tick) return boss->per_key_ticks;

    uint32_t elapsed = current_tick - boss->current_key_start_tick;
    return elapsed >= boss->per_key_ticks ? 0 : boss->per_key_ticks - elapsed;
}

void boss_destroy(boss_t* boss) {
    if (boss == NULL) return;

    sushi_destroy(&boss->sushi);
    combo_destroy(&boss->combo);
}

bool boss_has_timed_out(const boss_t* boss, uint32_t current_tick) {
    if (boss == NULL) return true;
    return boss_key_ticks_left(boss, current_tick) == 0 ||
           boss_total_ticks_left(boss, current_tick) == 0;
}

void boss_reset_sequence(boss_t* boss, uint32_t current_tick) {
    if (boss == NULL) return;
    combo_reset(&boss->combo);
    boss_randomize_key_position(boss);
    boss->current_key_start_tick = current_tick;
}
