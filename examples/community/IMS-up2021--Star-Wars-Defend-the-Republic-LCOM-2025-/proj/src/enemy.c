/**
 * @file enemy.c
 * @brief Implements enemy character logic, including spawning, movement, attacking, and drawing.
 */

#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include <stdlib.h>
#include <stdio.h>

#include "enemy.h"
#include "health.h"
#include "gameState.h"
#include "characters.h"
#include "controllers/video/graphics.h"

#define ENEMY_SPAWN_DELAY 180 /**< Delay in ticks before a new enemy unit can be spawned. */
#define ATTACK_COOLDOWN_TICKS 120 /**< Cooldown in ticks between enemy unit attacks (2 seconds at 60Hz). */

#include "xpms/enemy1/bg1_pose1.xpm"
#include "xpms/enemy2/bg2_pose1.xpm"
#include "xpms/enemy3/bg3_pose1.xpm"

#include "xpms/enemy1/bg1_attack1.xpm"
#include "xpms/enemy2/bg2_attack1.xpm"
#include "xpms/enemy3/bg3_attack1.xpm"

#include "xpms/enemy1/bg1_attack2.xpm"
#include "xpms/enemy2/bg2_attack2.xpm"
#include "xpms/enemy3/bg3_attack2.xpm"

uint8_t *e1_sprite_data = NULL, *e2_sprite_data = NULL, *e3_sprite_data = NULL, *a1_sprite_data = NULL, *a2_sprite_data = NULL, *a3_sprite_data = NULL, 
            *o1_sprite_data = NULL, *o2_sprite_data = NULL, *o3_sprite_data = NULL;
xpm_image_t enemy1_img, enemy2_img, enemy3_img, enemy_a_1_img, enemy_a_2_img, enemy_a_3_img, 
            enemy2_a_1_img, enemy2_a_2_img, enemy2_a_3_img;

Character *active_enemies[MAX_ENEMIES];
int num_active_enemies = 0;
unsigned int enemy_spawn_tick_counter = 0;
unsigned int timer_frequency_hz_enemy = 60;

/**
 * @brief Predefined spawn positions for enemies.
 * Enemies are typically spawned at the right edge of the screen at various y-coordinates.
 */
CharacterPos positions[5] = {
    {1200, 190}, {1200, 320}, {1200, 449}, {1200, 570}, {1200, 699}
};


/**
 * @brief Creates a new enemy character.
 * Allocates memory for a Character struct and initializes its properties based on the enemy type.
 * @param pos The initial position of the enemy.
 * @param enemy_type The type of enemy to create (0, 1, or 2).
 * @return A pointer to the newly created Character, or NULL if allocation fails or enemy_type is invalid.
 */
Character *createEnemy(CharacterPos pos, int enemy_type) {
    Character *enemy = (Character *)malloc(sizeof(Character));
    if (!enemy) {
        perror("Failed to allocate memory for enemy");
        return NULL;
    }

    enemy->x_pos = pos.x;
    enemy->y_pos = pos.y;
    enemy->damage = 10;
    enemy->health = 100;
    enemy->pixels_per_sec = 120;
    enemy->attack_range = 50;
    enemy->enemy = true;
    enemy->attack_cooldown = 0;
    enemy->attack_anim_timer = 30;
    switch (enemy_type) {
        case 0:
            enemy->sprite = e1_sprite_data;
            enemy->default_sprite = e1_sprite_data;
            enemy->width = enemy1_img.width;
            enemy->height = enemy1_img.height;
            enemy->default_width = enemy1_img.width;
            enemy->default_height = enemy1_img.height;
            enemy->is_attacking = false;
            enemy->attack_sprite = a1_sprite_data;
            enemy->attack_width = enemy_a_1_img.width;
            enemy->attack_height = enemy_a_1_img.height;
            enemy->attack_sprite2 = o1_sprite_data;
            enemy->attack2_width = enemy2_a_1_img.width;
            enemy->attack2_height = enemy2_a_1_img.height;

            break;
        case 1:
            enemy->sprite = e2_sprite_data;
            enemy->default_sprite = e2_sprite_data;
            enemy->width = enemy2_img.width;
            enemy->height = enemy2_img.height;
            enemy->default_width = enemy2_img.width;
            enemy->default_height = enemy2_img.height;
            enemy->is_attacking = false;
            enemy->attack_sprite = a2_sprite_data;
            enemy->attack_width = enemy_a_2_img.width;
            enemy->attack_height = enemy_a_2_img.height;
            enemy->attack_sprite2 = o2_sprite_data;
            enemy->attack2_width = enemy2_a_2_img.width;
            enemy->attack2_height = enemy2_a_2_img.height;

            break;
        case 2:
            enemy->sprite = e3_sprite_data;
            enemy->default_sprite = e3_sprite_data;
            enemy->width = enemy3_img.width;
            enemy->height = enemy3_img.height;
            enemy->default_width = enemy3_img.width;
            enemy->default_height = enemy3_img.height;
            enemy->is_attacking = false;
            enemy->attack_sprite = a3_sprite_data;
            enemy->attack_width = enemy_a_3_img.width;
            enemy->attack_height = enemy_a_3_img.height;
            enemy->attack_sprite2 = o3_sprite_data;
            enemy->attack2_width = enemy2_a_3_img.width;
            enemy->attack2_height = enemy2_a_3_img.height;

            break;
        default:
            free(enemy);
            return NULL;
    }

    return enemy;
}

/**
 * @brief Adds a newly created enemy to the game.
 * Creates an enemy and adds it to the `active_enemies` array.
 * @param pos The initial position of the enemy.
 * @param enemy_type The type of enemy to create.
 */
void add_enemy_to_game(CharacterPos pos, int enemy_type) {
    if (num_active_enemies >= MAX_ENEMIES) return;
    Character *new_enemy = createEnemy(pos, enemy_type);
    if (new_enemy) active_enemies[num_active_enemies++] = new_enemy;
}

/**
 * @brief Updates all active enemies and handles spawning new enemies.
 * This function iterates through active enemies, updating their state:
 * - Spawns new enemies periodically.
 * - Checks for player units in range and initiates attacks.
 * - Manages attack cooldown and animation.
 * - Moves enemies forward if no player unit is in range.
 * - Handles enemies reaching the player base (damaging player base).
 * - Despawns enemies that move off-screen or are defeated.
 */
void update_and_spawn_enemies(void) {
    if (timer_frequency_hz_enemy == 0) return;

    enemy_spawn_tick_counter++;
    if (enemy_spawn_tick_counter >= ENEMY_SPAWN_DELAY) {
        enemy_spawn_tick_counter = 0;
        int enemy_type = rand() % 3;
        int pos_idx = rand() % 5;
        add_enemy_to_game(positions[pos_idx], enemy_type);
    }

    for (int i = 0; i < num_active_enemies;) {
        Character *enemy = active_enemies[i];
        if (!enemy) {
            i++;
            continue;
        }

        bool player_in_range = false;
        enemy->attack_cooldown++;

        for (int j = 0; j < num_active_player_chars; j++) {
            Character *player = active_player_chars[j];
            if (!player || !player->health) continue;

            int dx = (int)(player->x_pos + player->width) - (int)(enemy->x_pos);
            int dy = abs((int)(player->y_pos) - (int)(enemy->y_pos));

            if (dx >= 0 && dx <= (int)enemy->attack_range && dy < 30) {
                player_in_range = true;

                if (enemy->attack_cooldown >= ATTACK_COOLDOWN_TICKS) {
                    player->health = (player->health > enemy->damage) ? player->health - enemy->damage : 0;
                    enemy->attack_cooldown = 0;

                    if (enemy->attack_sprite != NULL) {
                        enemy->sprite = enemy->attack_sprite;
                        enemy->is_attacking = true;
                        enemy->attack_anim_timer = 30; // lasts for 30 ticks
                        enemy->width = enemy->attack_width;
                        enemy->height = enemy->attack_height;
                    }
                }
                break; 
            }
    
        }

        if (enemy->is_attacking) {
            enemy->attack_anim_timer--;

            if (enemy->attack_anim_timer > 15) {
                enemy->sprite = enemy->attack_sprite;
                enemy->width = enemy->attack_width;
                enemy->height = enemy->attack_height;
            } else if (enemy->attack_anim_timer > 0) {
                enemy->sprite = enemy->attack_sprite2;
                enemy->width = enemy->attack2_width;
                enemy->height = enemy->attack2_height;
            } else {
                enemy->sprite = enemy->default_sprite;
                enemy->width = enemy->default_width;
                enemy->height = enemy->default_height;
                enemy->is_attacking = false;
            }
        }

        if (!player_in_range) {
            unsigned int pixels_to_move = enemy->pixels_per_sec / timer_frequency_hz_enemy;
            enemy->x_pos -= pixels_to_move;
        }

        if (enemy->x_pos == 0) {
            player_health->health = player_health->health - 10;
            printf("Player health: %u\n", player_health->health);
        }

        if ((int)(enemy->x_pos + enemy->width) < 0) {
            free(enemy);
            for (int j = i; j < num_active_enemies - 1; j++) {
                active_enemies[j] = active_enemies[j + 1];
            }
            active_enemies[--num_active_enemies] = NULL;
        } else {
            i++;
        }
    }
}

/**
 * @brief Draws all active enemies on the screen.
 * Iterates through `active_enemies` and draws each enemy's current sprite.
 */
void draw_enemies(void) {
    for (int i = 0; i < num_active_enemies; i++) {
        Character *enemy = active_enemies[i];
        if (enemy && enemy->sprite) {
            uint16_t *pixels = (uint16_t *)enemy->sprite;
            for (uint16_t y = 0; y < enemy->height; y++) {
                for (uint16_t x = 0; x < enemy->width; x++) {
                    uint16_t color = pixels[y * enemy->width + x];
                    if (color == TRANSPARENT_COLOR) continue;
                    vg_draw_pixel(enemy->x_pos + x, enemy->y_pos + y, color);
                }
            }
        }
    }
}

/**
 * @brief Initializes enemy sprites.
 * Loads all XPM images for enemy characters and their attack animations.
 * @return True if all default sprites were loaded successfully, false otherwise. Note: attack sprites are also loaded but only default sprite success is checked in return.
 */
bool init_enemies(void) {
    e1_sprite_data = xpm_load(bg1_pose1_xpm, XPM_5_6_5, &enemy1_img);
    e2_sprite_data = xpm_load(bg2_pose1_xpm, XPM_5_6_5, &enemy2_img);
    e3_sprite_data = xpm_load(bg3_pose1_xpm, XPM_5_6_5, &enemy3_img);

    a1_sprite_data = xpm_load(bg1_attack1_xpm, XPM_5_6_5, &enemy_a_1_img);
    a2_sprite_data = xpm_load(bg2_attack1_xpm, XPM_5_6_5, &enemy_a_2_img);
    a3_sprite_data = xpm_load(bg3_attack1_xpm, XPM_5_6_5, &enemy_a_3_img);

    o1_sprite_data = xpm_load(bg1_attack2_xpm, XPM_5_6_5, &enemy2_a_1_img);
    o2_sprite_data = xpm_load(bg2_attack2_xpm, XPM_5_6_5, &enemy2_a_2_img);
    o3_sprite_data = xpm_load(bg3_attack2_xpm, XPM_5_6_5, &enemy2_a_3_img);

    printf("Enemy sprites loaded: %s, %s, %s\n", 
           e1_sprite_data ? "e1" : "NULL", 
           e2_sprite_data ? "e2" : "NULL", 
           e3_sprite_data ? "e3" : "NULL");
    printf("Enemy attack sprites loaded: %s, %s, %s\n",
              a1_sprite_data ? "a1" : "NULL", 
              a2_sprite_data ? "a2" : "NULL", 
              a3_sprite_data ? "a3" : "NULL");
    printf("Enemy attack2 sprites loaded: %s, %s, %s\n",
              o1_sprite_data ? "o1" : "NULL", 
              o2_sprite_data ? "o2" : "NULL", 
              o3_sprite_data ? "o3" : "NULL");


    num_active_enemies = 0;
    enemy_spawn_tick_counter = 0;

    return (e1_sprite_data && e2_sprite_data && e3_sprite_data);
}

/**
 * @brief Sets the timer frequency for enemy character updates.
 * @param hz The new frequency in Hertz.
 */
void set_timer_frequency_for_enemies(unsigned int hz) {
    timer_frequency_hz_enemy = hz;
}
