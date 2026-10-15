#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/game/game.h"
#include "../include/game/game_rand.h"
#include "../include/macros/mouse.h"
#include "../include/ui/boss_draw.h"
#include "../include/ui/slash_combo_popup.h"
#include "../include/ui/ui.h"

uint32_t gameplay_get_sushi_seed(const GameInfo *game) {
    return game ? game->singleplayer.sushi_prng_seed : 0;
}

void gameplay_set_sushi_seed(GameInfo *game, uint32_t seed) {
    if (game) game->singleplayer.sushi_prng_seed = seed;
}

uint32_t gameplay_get_boss_seed(const GameInfo *game) {
    return game ? game->singleplayer.boss_prng_seed : 0;
}

void gameplay_set_boss_seed(GameInfo *game, uint32_t seed) {
    if (game) game->singleplayer.boss_prng_seed = seed;
}

#define GAMEPLAY_TICKS_PER_SECOND 60

static void gameplay_finish_boss(GameInfo *game, boss_status_t status, uint32_t current_tick);
static void gameplay_update_active_boss(GameInfo *game, uint32_t current_tick);

// 1. Sushi management

static void gameplay_remove_sushi(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->singleplayer.num_active_sushi) return;

    for (uint8_t i = index; i + 1 < game->singleplayer.num_active_sushi; i++) {
        game->singleplayer.active_sushi[i] = game->singleplayer.active_sushi[i + 1];
    }
    game->singleplayer.num_active_sushi--;
}

static void gameplay_remove_fragment(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->singleplayer.num_active_fragments) return;

    for (uint8_t i = index; i + 1 < game->singleplayer.num_active_fragments; i++) {
        game->singleplayer.active_fragments[i] = game->singleplayer.active_fragments[i + 1];
    }
    game->singleplayer.num_active_fragments--;
}

static uint8_t gameplay_boss_sequence_length(uint32_t boss_count) {
    uint32_t length = GAME_BOSS_FIRST_SEQUENCE_LENGTH +
                      boss_count * GAME_BOSS_SEQUENCE_STEP;

    return length > MAX_QUEUE_SIZE ? MAX_QUEUE_SIZE : (uint8_t)length;
}

static void gameplay_start_boss(GameInfo *game, uint32_t current_tick) {
    if (game == NULL || game->singleplayer.boss_active) return;

    for (uint8_t i = 0; i < game->singleplayer.num_active_sushi; i++) {
        sushi_destroy(&game->singleplayer.active_sushi[i]);
    }
    game->singleplayer.num_active_sushi = 0;

    for (uint8_t i = 0; i < game->singleplayer.num_active_fragments; i++) {
        sushi_fragment_destroy(&game->singleplayer.active_fragments[i]);
    }
    game->singleplayer.num_active_fragments = 0;

    uint8_t sequence_length = gameplay_boss_sequence_length(game->singleplayer.boss_count);
    uint32_t per_key_ticks = BOSS_TOTAL_TICKS / sequence_length;
    if (per_key_ticks < 240) {
        per_key_ticks = 240;
    }
    uint32_t seed = gameplay_get_boss_seed(game);
    combo_t combo = create_random_combo(sequence_length, CENTER_X(64), 300, &seed);
    if (combo.total_letters == 0) {
        combo_destroy(&combo);
        return;
    }

    game->singleplayer.active_boss = boss_init(combo,
                                               current_tick,
                                               BOSS_TOTAL_TICKS,
                                               per_key_ticks,
                                               &seed);
    gameplay_set_boss_seed(game, seed);
    game->singleplayer.boss_active = true;
    game->singleplayer.boss_count++;
    game->singleplayer.next_boss_score += GAME_BOSS_SCORE_STEP;
}

static void gameplay_check_boss_spawn(GameInfo *game, uint32_t current_tick) {
    if (game == NULL || game->state != GAME || game->singleplayer.boss_active) return;

    if (game->score >= game->singleplayer.next_boss_score) {
        gameplay_start_boss(game, current_tick);
    }
}

// 2. Wave and difficulty handling

static difficulty_t gameplay_difficulty_for_score(uint32_t score) {
    if (score >= GAME_HARD_SCORE) return HARD;
    if (score >= GAME_MID_HARD_SCORE) return MID_HARD;
    if (score >= GAME_MID_SCORE) return MID;
    if (score >= GAME_EASY_MID_SCORE) return EASY_MID;
    return EASY;
}

static void gameplay_apply_difficulty(GameInfo *game, difficulty_t difficulty) {
    if (game == NULL) return;

    game->singleplayer.current_difficulty = difficulty;
}

static void gameplay_load_waves(GameInfo *game) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < WAVE_PATTERN_COUNT; i++) {
        const wave_t *wave = waves_get_predefined((wave_pattern_t)i);
        if (wave != NULL) {
            game->singleplayer.waves[i] = *wave;
        }
    }

    gameplay_apply_difficulty(game, EASY);
}

static void gameplay_update_difficulty(GameInfo *game) {
    if (game == NULL) return;

    difficulty_t difficulty = gameplay_difficulty_for_score(game->score);
    if (difficulty != game->singleplayer.current_difficulty) {
        gameplay_apply_difficulty(game, difficulty);
    }
}

static void gameplay_get_difficulty_physics(GameInfo *game, int *speed, int *gravity) {
    if (speed == NULL || gravity == NULL) return;

    difficulty_t difficulty = game == NULL ? EASY : game->singleplayer.current_difficulty;
    const motion_profile_t *motion = waves_get_difficulty_motion(difficulty);
    int min_speed = PHYSICS_TO_PIXELS(-motion->max_vy);
    int max_speed = PHYSICS_TO_PIXELS(-motion->min_vy);

    if (max_speed < min_speed) {
        max_speed = min_speed;
    }

    uint32_t seed = gameplay_get_sushi_seed(game);
    *speed = min_speed + game_rand(&seed) % (max_speed - min_speed + 1);
    gameplay_set_sushi_seed(game, seed);
    *gravity = motion->gravity;
}

static void gameplay_add_item(GameInfo *game, sushi_t item) {
    if (game == NULL || item.xpm == NULL) {
        sushi_destroy(&item);
        return;
    }

    if (game->singleplayer.num_active_sushi >= MAX_ACTIVE_SUSHI) {
        sushi_destroy(&item);
        return;
    }

    game->singleplayer.active_sushi[game->singleplayer.num_active_sushi++] = item;
}

static void gameplay_spawn_wave_item(GameInfo *game, const wave_spawn_request_t *request, int speed, int gravity) {
    if (game == NULL || request == NULL) return;

    uint32_t seed = gameplay_get_sushi_seed(game);

    switch (request->kind) {
        case WAVE_SPAWN_REGULAR: {
            sushi_t sushi = create_random_sushi(speed, gravity, &seed);
            sushi.score_value = (int)request->score_value;
            gameplay_add_item(game, sushi);
            break;
        }
        case WAVE_SPAWN_BOMB:
            gameplay_add_item(game, create_random_bomb(speed, gravity, &seed));
            break;
        default:
            break;
    }

    gameplay_set_sushi_seed(game, seed);
}

static void gameplay_handle_wave_spawn(GameInfo *game, const wave_spawn_request_t *request) {
    if (game == NULL || request == NULL) return;

    for (uint8_t i = 0; i < request->amount; i++) {
        int speed = 0;
        int gravity = 0;
        gameplay_get_difficulty_physics(game, &speed, &gravity);

        gameplay_spawn_wave_item(game, request, speed, gravity);
    }
}
// 3. Gameplay state

static bool sushi_missed_lower_bound(const sushi_t *sushi) {
    return sushi != NULL &&
           sushi->xpm != NULL &&
           sushi_screen_y(sushi) > SCREEN_HEIGHT + SUSHI_MISS_Y_OFFSET &&
           sushi->vy > 0;
}

void gameplay_clear(GameInfo *game) {
    if (game == NULL) return;

    if (game->singleplayer.boss_active) {
        boss_destroy(&game->singleplayer.active_boss);
        game->singleplayer.boss_active = false;
    }

    for (uint8_t i = 0; i < game->singleplayer.num_active_sushi; i++) {
        sushi_destroy(&game->singleplayer.active_sushi[i]);
    }

    for (uint8_t i = 0; i < game->singleplayer.num_active_fragments; i++) {
        sushi_fragment_destroy(&game->singleplayer.active_fragments[i]);
    }

    game->singleplayer.num_active_sushi = 0;
    game->singleplayer.num_active_fragments = 0;
    slash_combo_reset(&game->singleplayer.slash_combo);
    game->singleplayer.pending_boss_score_popup = 0;
    game->singleplayer.boss_score_popup_ticks = 0;
}

static game_over_stats_t gameplay_final_stats(const GameInfo *game, uint32_t elapsed_ticks) {
    game_over_stats_t stats = {0, 0, 0, 0, 0};
    if (game == NULL) return stats;

    stats = game->singleplayer.stats;
    stats.time_survived_seconds = elapsed_ticks / GAMEPLAY_TICKS_PER_SECOND;
    return stats;
}

static void gameplay_end_game(GameInfo *game, uint32_t elapsed_ticks) {
    if (game == NULL) return;

    uint32_t final_score = game->score;
    game_over_stats_t final_stats = gameplay_final_stats(game, elapsed_ticks);
    uint32_t active_seed = game->singleplayer.current_seed;
    game->lives = 0;
    gameplay_clear(game);
    game->state = GAME_OVER;
    game_over_init(&game->data.game_over, final_score, final_stats, active_seed);
}

static bool gameplay_handle_missed_item(GameInfo *game, uint8_t index) {
    if (game == NULL || index >= game->singleplayer.num_active_sushi) return false;

    sushi_kind_t kind = game->singleplayer.active_sushi[index].kind;
    sushi_destroy(&game->singleplayer.active_sushi[index]);
    gameplay_remove_sushi(game, index);

    if (kind == SUSHI_BOMB) {
        game->singleplayer.stats.bombs_avoided++;
        return false;
    }

    game->singleplayer.stats.sushi_missed++;

    if (game->lives > 0) {
        game->lives--;
    }

    return game->lives == 0;
}

void gameplay_start(GameInfo *game) {
    if (game == NULL) return;

    memset(&game->singleplayer, 0, sizeof(game->singleplayer));

    if (game->data.menu.seed_val > 0) {
        game->singleplayer.current_seed = game->data.menu.seed_val;
    } else {
        game->singleplayer.current_seed = game_seed_from_rtc();
    }
    gameplay_set_sushi_seed(game, game->singleplayer.current_seed);
    gameplay_set_boss_seed(game, game->singleplayer.current_seed);
    srand(game->singleplayer.current_seed);

    gameplay_load_waves(game);
    waves_init(&game->singleplayer.wave_runtime, 0);
    game->singleplayer.next_boss_score = GAME_BOSS_FIRST_SCORE;
    game->score = 0;
    game->lives = LIVES;
}

void gameplay_update(GameInfo *game, int current_tick, int *last_spawn_tick) {
    if (game == NULL || last_spawn_tick == NULL) return;

    gameplay_update_difficulty(game);
    game->score += slash_combo_update(&game->singleplayer.slash_combo,
                                      (uint32_t)current_tick);
    if (game->singleplayer.boss_score_popup_ticks > 0) {
        game->singleplayer.boss_score_popup_ticks--;
    }
    gameplay_check_boss_spawn(game, (uint32_t)current_tick);

    if (game->singleplayer.boss_active) {
        waves_pause_tick(&game->singleplayer.wave_runtime);
        if (last_spawn_tick != NULL) {
            (*last_spawn_tick)++;
        }
    } else {
        wave_spawn_request_t request;
        bool has_request;
        wave_status_t wave_status;
        uint32_t s_seed = gameplay_get_sushi_seed(game);
        do {
            wave_status = waves_update(&game->singleplayer.wave_runtime,
                                       game->singleplayer.waves,
                                       WAVE_PATTERN_COUNT,
                                       game->singleplayer.current_difficulty,
                                       (uint32_t)current_tick,
                                       &s_seed,
                                       &request,
                                       &has_request);
            if (has_request) {
                gameplay_handle_wave_spawn(game, &request);
                *last_spawn_tick = current_tick;
            }
        }
        while (has_request && wave_status == WAVE_STATUS_RUNNING);
        gameplay_set_sushi_seed(game, s_seed);
    }

    gameplay_update_active_boss(game, (uint32_t)current_tick);
    if (game->state != GAME) return;

    uint8_t i = 0;
    while (i < game->singleplayer.num_active_sushi) {
        update_sushi(&game->singleplayer.active_sushi[i]);

        if (sushi_missed_lower_bound(&game->singleplayer.active_sushi[i])) {
            if (gameplay_handle_missed_item(game, i)) {
                gameplay_end_game(game, (uint32_t)current_tick);
                return;
            }
        }
        else if (game->singleplayer.active_sushi[i].xpm == NULL) {
            gameplay_remove_sushi(game, i);
        }
        else {
            i++;
        }
    }

    i = 0;
    while (i < game->singleplayer.num_active_fragments) {
        update_sushi_fragment(&game->singleplayer.active_fragments[i]);
        if (game->singleplayer.active_fragments[i].xpm == NULL) {
            gameplay_remove_fragment(game, i);
        }
        else {
            i++;
        }
    }
}

void gameplay_draw(const GameInfo *game, const Assets *assets, uint32_t current_tick) {
    if (game == NULL) return;

    for (uint8_t i = 0; i < game->singleplayer.num_active_sushi; i++) {
        const sushi_t *sushi = &game->singleplayer.active_sushi[i];
        int x = sushi_screen_x(sushi);
        int y = sushi_screen_y(sushi);
        if (sushi->xpm != NULL && x >= 0 && y >= 0) {
            ui_draw_sprite(sushi->xpm, (uint16_t)x, (uint16_t)y);
        }
    }

    for (uint8_t i = 0; i < game->singleplayer.num_active_fragments; i++) {
        const sushi_fragment_t *fragment = &game->singleplayer.active_fragments[i];
        int x = sushi_fragment_screen_x(fragment);
        int y = sushi_fragment_screen_y(fragment);
        if (fragment->xpm != NULL && x >= 0 && y >= 0) {
            ui_draw_sprite(fragment->xpm, (uint16_t)x, (uint16_t)y);
        }
    }

    ui_combo_popup_draw(&game->singleplayer.slash_combo, assets);
    ui_boss_draw(&game->singleplayer.active_boss,
                 game->singleplayer.boss_active,
                 assets,
                 current_tick);
    ui_boss_reward_popup_draw(game->singleplayer.pending_boss_score_popup,
                              game->singleplayer.boss_score_popup_ticks,
                              assets);
}

// 4. Cutting

void gameplay_handle_cut(GameInfo *game, MouseGameState *mouse_state, uint32_t current_tick) {
    if (game == NULL || mouse_state == NULL) return;

    if (!mouse_state->lb_pressed || !mouse_state->is_slashing) {
        mouse_state->prev_x = mouse_state->x;
        mouse_state->prev_y = mouse_state->y;
        return;
    }

    int cut_x = mouse_state->x + MOUSE_CURSOR_CENTER_OFFSET_X;
    int cut_y = mouse_state->y + MOUSE_CURSOR_CENTER_OFFSET_Y;
    int prev_cut_x = mouse_state->prev_x + MOUSE_CURSOR_CENTER_OFFSET_X;
    int prev_cut_y = mouse_state->prev_y + MOUSE_CURSOR_CENTER_OFFSET_Y;

    mouse_state->prev_x = mouse_state->x;
    mouse_state->prev_y = mouse_state->y;

    uint8_t i = 0;
    while (i < game->singleplayer.num_active_sushi) {
        sushi_t *sushi = &game->singleplayer.active_sushi[i];

        if (!sushi_segment_intersects(sushi,
                                      prev_cut_x,
                                      prev_cut_y,
                                      cut_x,
                                      cut_y,
                                      MOUSE_SLASH_HIT_RADIUS)) {
            i++;
            continue;
        }

        if (sushi->kind == SUSHI_BOMB) {
            gameplay_end_game(game, current_tick);
            return;
        }

        if (sushi->kind == SUSHI_REGULAR) {
            if (game->singleplayer.num_active_fragments + 2 > MAX_ACTIVE_FRAGMENTS) {
                return;
            }

            sushi_fragment_t left_fragment;
            sushi_fragment_t right_fragment;
            if (cut_sushi(sushi, &left_fragment, &right_fragment) != 0) {
                i++;
                continue;
            }

            int impact_x = (prev_cut_x + cut_x) / 2;
            int impact_y = (prev_cut_y + cut_y) / 2;
            uint32_t score_value = (uint32_t)sushi->score_value;

            game->score += score_value;
            game->score += slash_combo_register_cut(&game->singleplayer.slash_combo,
                                                    current_tick,
                                                    score_value,
                                                    impact_x,
                                                    impact_y);
            game->singleplayer.stats.sushi_cut++;
            if (game->singleplayer.slash_combo.count > game->singleplayer.stats.max_combo) {
                game->singleplayer.stats.max_combo = game->singleplayer.slash_combo.count;
            }
            game->singleplayer.active_fragments[game->singleplayer.num_active_fragments++] = left_fragment;
            game->singleplayer.active_fragments[game->singleplayer.num_active_fragments++] = right_fragment;
            gameplay_remove_sushi(game, i);
            gameplay_check_boss_spawn(game, current_tick);
        }
        else {
            i++;
        }
    }
}

static void gameplay_finish_boss(GameInfo *game, boss_status_t status, uint32_t current_tick) {
    if (game == NULL || !game->singleplayer.boss_active) return;

    uint32_t sequence_length = game->singleplayer.active_boss.combo.total_letters;
    boss_destroy(&game->singleplayer.active_boss);
    game->singleplayer.boss_active = false;

    if (status == BOSS_STATUS_SUCCESS) {
        uint32_t reward = sequence_length * BOSS_POINTS_PER_KEY;
        game->score += reward;
        game->singleplayer.pending_boss_score_popup = reward;
        game->singleplayer.boss_score_popup_ticks = GAME_BOSS_REWARD_POPUP_TICKS;
        gameplay_check_boss_spawn(game, current_tick);
        return;
    }

    if (game->lives > 0) {
        game->lives--;
    }

    if (game->lives == 0) {
        gameplay_end_game(game, current_tick);
    }
}

static void gameplay_update_active_boss(GameInfo *game, uint32_t current_tick) {
    if (game == NULL || !game->singleplayer.boss_active) return;

    boss_status_t status = boss_update(&game->singleplayer.active_boss, current_tick);
    if (status != BOSS_STATUS_ACTIVE) {
        gameplay_finish_boss(game, status, current_tick);
    }
}

void gameplay_handle_boss_key(GameInfo *game, char input, uint32_t current_tick) {
    if (game == NULL || !game->singleplayer.boss_active) return;

    if (boss_has_timed_out(&game->singleplayer.active_boss, current_tick)) {
        gameplay_finish_boss(game, BOSS_STATUS_FAILURE, current_tick);
        return;
    }

    boss_input_result_t result = boss_verify_combo(&game->singleplayer.active_boss,
                                                   input,
                                                   current_tick);
    if (result == BOSS_INPUT_WRONG) {
        boss_reset_sequence(&game->singleplayer.active_boss, current_tick);
    }
    else if (result == BOSS_INPUT_COMPLETE) {
        gameplay_finish_boss(game, BOSS_STATUS_SUCCESS, current_tick);
    }
}

bool gameplay_boss_active(const GameInfo *game) {
    return game != NULL && game->singleplayer.boss_active;
}
