#include "include/enemy.h"
#include "include/level_config.h"
#include "include/config.h"
#include "include/enemy_sprites.h"
#include "include/bullet.h"
#include "include/game_state.h"
#include "../lab5/video.h"
#include <math.h>
#include "include/renderer.h"

#define ENEMY_FALLBACK_COLOR 0xFF0000
int walkers_escaped = 0;

// arrays de frames para cada tipo de inimigo
static uint8_t *alarm_frames[ALARM_SPRITE_COUNT];
static xpm_image_t alarm_img_info;

static uint8_t *analog_frames[ANALOG_SPRITE_COUNT];
static xpm_image_t analog_img_info;

static uint8_t *digital_frames[DIGITAL_SPRITE_COUNT];
static xpm_image_t digital_img_info;

static Enemy *global_enemies = NULL;
static int global_enemy_count = 0;

// carrega todos os sprites dos inimigos
int enemy_load_sprites(void) {
    int failed = 0;

    for (int i = 0; i < ALARM_SPRITE_COUNT; i++) {
        alarm_frames[i] = xpm_load(alarm_sprite_maps[i], XPM_8_8_8, &alarm_img_info);
        if (alarm_frames[i] == NULL) failed = 1;
    }

    for (int i = 0; i < ANALOG_SPRITE_COUNT; i++) {
        analog_frames[i] = xpm_load(analog_sprite_maps[i], XPM_8_8_8, &analog_img_info);
        if (analog_frames[i] == NULL) failed = 1;
    }

    for (int i = 0; i < DIGITAL_SPRITE_COUNT; i++) {
        digital_frames[i] = xpm_load(digital_sprite_maps[i], XPM_8_8_8, &digital_img_info);
        if (digital_frames[i] == NULL) failed = 1;
    }

    return failed;
}

// Inicializa o array de inimigos
void init_enemies(Enemy enemies_arr[], int count) {
    global_enemies = enemies_arr;
    global_enemy_count = count;

    walkers_escaped = 0;

    for (int i = 0; i < count; i++) {
        enemies_arr[i].is_active = false;
    }
}

void update_enemies(Enemy enemies_arr[], int count, Player *player) {
    Bullet *bullets = bullet_get_all();
    int bullet_count = bullet_get_count();
    
    //Colisões bala inimiga -> player
    for (int j = 0; j < bullet_count; j++) {
        if (!bullets[j].is_active) continue;

        if (bullets[j].type != BULLET_ENEMY) continue;

        if (check_enemy_bullet_hits_player(&bullets[j], player)) {
            bullets[j].is_active = false;

            if (player_take_damage()) {
                game_state_trigger_game_over();
                return;
            }
        }
    }

    //Atualização dos inimigos
    for (int i = 0; i < count; i++) {
        if (!enemies_arr[i].is_active) continue;

        Enemy *e = &enemies_arr[i];
        EnemyStats *s = &e->stats;

        // animação
        e->time_left--;
        if (e->time_left <= 0) {
            e->time_left = e->total_time;
            e->current_frame = (e->current_frame + 1) % e->max_frames;
        }

        // dano
        if (e->damage_ticks > 0) e->damage_ticks--;

        //WALKER
        if (e->type == ENEMY_WALKER) {

            e->y += s->speed * s->direction;

            // chegou ao fim do rail (descer)
            if (s->direction == 1 &&
                e->y + e->img_info.height >= e->rail_bot) {

                e->is_active = false;
                walkers_escaped++;

                if (walkers_escaped >= 6) {
                    game_state_trigger_game_over();
                }

                continue;
            }

            // chegou ao fim do rail (subir)
            if (s->direction == -1 &&
                e->y <= e->rail_top) {

                e->is_active = false;
                walkers_escaped++;

                if (walkers_escaped >= 6) {
                    game_state_trigger_game_over();
                }

                continue;
            }

            // contacto direto = game over
            if (check_player_hits_enemy(player, e)) {
                game_state_trigger_game_over();
                return;
            }
        }

        // SHOTTER
        else if (e->type == ENEMY_SHOTTER) {

            if (s->fire_cooldown > 0) {
                s->fire_cooldown--;
            }
            else {
                int b_x = (e->x + e->img_info.width / 2 - renderer_get_enemy_bullet_width() / 2) - 12;
                int b_y = e->y + e->img_info.height / 2 - renderer_get_enemy_bullet_height() / 2;

                int direction = (player->y < e->y) ? -1 : 1;

                bullet_spawn(
                    b_x,
                    b_y,
                    direction,
                    3,
                    BULLET_ENEMY
                );

                s->fire_cooldown = s->fire_rate;
            }
        }

        //PARALYZER
        else if (e->type == ENEMY_PARALYZER) {

            if (s->fire_cooldown > 0) {
                s->fire_cooldown--;
            }
            else {
                player_freeze(30);
                s->fire_cooldown = s->fire_rate;
            }
        }

        //Balas do player -> inimigo
        for (int j = 0; j < bullet_count; j++) {

            if (!bullets[j].is_active) continue;

            if (bullets[j].type != BULLET_PLAYER) continue;

            if (check_bullet_hits_enemy(&bullets[j], e)) {

                bullets[j].is_active = false;

                s->health--;
                e->damage_ticks = 10; // pisca o inimigo por 10 frames

                if (s->health <= 0) {
                    e->is_active = false;
                }

                break; /* a bala já foi consumida */
            }
        }
    }
}

void draw_enemies(Enemy enemies_arr[], int count) { 
    if (enemies_arr == NULL) return;
 
    for (int i = 0; i < count; i++) {
        if (!enemies_arr[i].is_active) continue;
 
        Enemy *e = &enemies_arr[i];
 
        if (e->frame_maps == NULL ||
            e->current_frame < 0 ||
            e->current_frame >= e->max_frames ||
            e->frame_maps[e->current_frame] == NULL) {
            my_vg_draw_rectangle((uint16_t)e->x, (uint16_t)e->y, 32, 32, ENEMY_FALLBACK_COLOR);
            continue;
        }

        if (e->damage_ticks > 0) {
            renderer_draw_sprite_red_tint(e->img_info, e->frame_maps[e->current_frame], e->x, e->y);
        } else {
            renderer_draw_sprite(e->img_info, e->frame_maps[e->current_frame], e->x, e->y);
        }
    }
}

Enemy* enemy_get_all(void) {
    return global_enemies;
}

int enemy_get_count(void) {
    return global_enemy_count;
}

// Associa os ponteiros de sprites e metadados de frames ao inimigo de acordo com o seu tipo
void enemy_assign_sprites(Enemy *enemy) {
    switch (enemy->type) {
        case ENEMY_WALKER:
            enemy->frame_maps = analog_frames;
            enemy->img_info = analog_img_info;
            enemy->max_frames = ANALOG_SPRITE_COUNT;
            enemy->total_time = 10;
            break;

        case ENEMY_SHOTTER:
            enemy->frame_maps = digital_frames;
            enemy->img_info = digital_img_info;
            enemy->max_frames= DIGITAL_SPRITE_COUNT;
            enemy->total_time = 15;
            break;

        case ENEMY_PARALYZER:
            enemy->frame_maps = alarm_frames;
            enemy->img_info = alarm_img_info;
            enemy->max_frames = ALARM_SPRITE_COUNT;
            enemy->total_time = 12;
            break;
    }
}

void enemy_spawn(Enemy *enemy, int x, int y, EnemyType type, int wave_id, const EnemyStats *stats) {
    if (enemy == NULL) return;

    // obtem defaults e aplica overrides fornecidos
    EnemyStats resolved;
    level_default_stats(type, &resolved);
    if (stats != NULL) resolved = *stats;

    enemy->x = x;
    enemy->type = type;
    enemy->is_active = true;
    enemy->wave_id = wave_id;
    enemy->stats = resolved;
    enemy->stats.fire_cooldown = 0; // começa sem cooldown
    enemy->damage_ticks = 0; // começa sem dano

    enemy->current_frame = 0;
    enemy_assign_sprites(enemy);
    enemy->time_left = enemy->total_time;

    // Rails para walkers
    if (type == ENEMY_WALKER) {

      // LEVEL 1
        if (x == L1_RAIL_A_X) { enemy->rail_top = L1_RAIL_A_TOP; enemy->rail_bot = L1_RAIL_A_BOT; }
        else if (x == L1_RAIL_B_X) { enemy->rail_top = L1_RAIL_B_TOP; enemy->rail_bot = L1_RAIL_B_BOT; }
        else if (x == L1_RAIL_C_X) { enemy->rail_top = L1_RAIL_C_TOP; enemy->rail_bot = L1_RAIL_C_BOT; }
        else if (x == L1_RAIL_D_X) { enemy->rail_top = L1_RAIL_D_TOP; enemy->rail_bot = L1_RAIL_D_BOT; }

        // LEVEL 2
        else if (x == L2_RAIL_A_X) { enemy->rail_top = L2_RAIL_A_TOP; enemy->rail_bot = L2_RAIL_A_BOT; }
        else if (x == L2_RAIL_B_X) { enemy->rail_top = L2_RAIL_B_TOP; enemy->rail_bot = L2_RAIL_B_BOT; }
        else if (x == L2_RAIL_C_X) { enemy->rail_top = L2_RAIL_C_TOP; enemy->rail_bot = L2_RAIL_C_BOT; }

        // LEVEL 4
        else if (x == L4_RAIL_A_X) { enemy->rail_top = L4_RAIL_A_TOP; enemy->rail_bot = L4_RAIL_A_BOT; }
        else if (x == L4_RAIL_B_X) { enemy->rail_top = L4_RAIL_B_TOP; enemy->rail_bot = L4_RAIL_B_BOT; }

      if (enemy->stats.direction == 1) {
          enemy->y = enemy->rail_top - enemy->img_info.height;
      } else {
          enemy->y = enemy->rail_bot;
      }
        
    } else {
        enemy->rail_top = 0;
        enemy->rail_bot = SCREEN_H;
        enemy->y = y;
    }
}

// Devolve o índice do primeiro slot livre is_active = false, ou -1 se não houver
int enemy_find_free_slot(Enemy enemies[], int max) {
    for (int i = 0; i < max; i++) {
        if (!enemies[i].is_active) return i;
    }
    return -1;
}

// true se ainda houver inimigos ativos pertencentes a wave_id
bool enemy_wave_has_active(Enemy enemies[], int max, int wave_id) {
    for (int i = 0; i < max; i++) {
        if (enemies[i].is_active && enemies[i].wave_id == wave_id) return true;
    }
    return false;
}
