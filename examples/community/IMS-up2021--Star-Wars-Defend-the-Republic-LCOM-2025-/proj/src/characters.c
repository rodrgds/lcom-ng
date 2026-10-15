/**
 * @file characters.c
 * @brief Implements player character logic, including spawning, movement, attacking, and drawing.
 */

#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include <stdlib.h>
#include <stdio.h>

#include "characters.h"
#include "gameState.h"
#include "health.h"
#include "controllers/video/graphics.h"
#include "enemy.h" // for access to active enemies

#define PLAYER_SPAWN_DELAY 180  /**< Delay in ticks before a new player unit can be spawned. */
#define SCREEN_WIDTH 1275  /**< Width of the game screen in pixels. */
#define ATTACK_COOLDOWN_TICKS 120  /**< Cooldown in ticks between player unit attacks (2 seconds at 60Hz). */

#include "xpms/char1/pose_1.xpm"
#include "xpms/char2/h2_pose1.xpm"
#include "xpms/char3/h3_pose1.xpm"

#include "xpms/char1/attack_1.xpm"
#include "xpms/char2/h2_attack1.xpm"
#include "xpms/char3/h3_attack1.xpm"

#include "xpms/char1/attack_2.xpm"
#include "xpms/char2/h2_attack2.xpm"
#include "xpms/char3/h3_attack2.xpm"

uint8_t *pc1_sprite_data = NULL, *pc2_sprite_data = NULL, *pc3_sprite_data = NULL;
uint8_t *ah1_sprite_data = NULL, *ah2_sprite_data = NULL, *ah3_sprite_data = NULL;
uint8_t *pc1_attack2_img = NULL, *pc2_attack2_img = NULL, *pc3_attack2_img = NULL;
xpm_image_t pc1_img, pc2_img, pc3_img, ah1_img, ah2_img, ah3_img,
            pc1_attack2, pc2_attack2, pc3_attack2;

Character *active_player_chars[MAX_PLAYER_CHARACTERS];
int num_active_player_chars = 0;
unsigned int player_spawn_tick_counter = 0;
unsigned int timer_frequency_hz_hero = 60;

extern CharacterPos player_spawn_positions[5];

/**
 * @brief Initializes player unit sprites. 
 * Loads all XPM images for player characters and their attack animations.
 * @return True if all sprites were loaded successfully, false otherwise.
 */

bool init_player_units(void) {
    pc1_sprite_data = xpm_load(h1_pose_1, XPM_5_6_5, &pc1_img);
    pc2_sprite_data = xpm_load(h2_pose1, XPM_5_6_5, &pc2_img);
    pc3_sprite_data = xpm_load(h3_pose1, XPM_5_6_5, &pc3_img);

    ah1_sprite_data = xpm_load(h1_attack1, XPM_5_6_5, &ah1_img);
    ah2_sprite_data = xpm_load(h2_attack1, XPM_5_6_5, &ah2_img);
    ah3_sprite_data = xpm_load(h3_attack1, XPM_5_6_5, &ah3_img);

    pc1_attack2_img = xpm_load(attack_2, XPM_5_6_5, &pc1_attack2);
    pc2_attack2_img = xpm_load(h2_attack2, XPM_5_6_5, &pc2_attack2);
    pc3_attack2_img = xpm_load(h3_attack2, XPM_5_6_5, &pc3_attack2);

    printf("Player sprites loaded: %s, %s, %s\n", 
           pc1_sprite_data ? "pc1" : "NULL", 
           pc2_sprite_data ? "pc2" : "NULL", 
           pc3_sprite_data ? "pc3" : "NULL");
    printf("Attack sprites loaded: %s, %s, %s\n",
           ah1_sprite_data ? "ah1" : "NULL", 
           ah2_sprite_data ? "ah2" : "NULL", 
           ah3_sprite_data ? "ah3" : "NULL");
    printf("Attack2 sprites loaded: %s, %s, %s\n",
           pc1_attack2_img ? "pc1_attack2" : "NULL", 
           pc2_attack2_img ? "pc2_attack2" : "NULL", 
           pc3_attack2_img ? "pc3_attack2" : "NULL");

    num_active_player_chars = 0;
    player_spawn_tick_counter = 0;

    return (pc1_sprite_data && pc2_sprite_data && pc3_sprite_data && ah1_sprite_data && ah2_sprite_data && ah3_sprite_data);
}

/**
 * @brief Creates a new player character unit.
 * Allocates memory for a Character struct and initializes its properties based on the unit type.
 * @param pos The initial position of the unit.
 * @param unit_type The type of player unit to create (0, 1, or 2).
 * @return A pointer to the newly created Character, or NULL if allocation fails or unit_type is invalid.
 */

Character *create_player_unit(CharacterPos pos, int unit_type) {
    Character *unit = (Character *)malloc(sizeof(Character));
    if (!unit) {
        perror("Failed to allocate memory for player unit");
        return NULL;
    }

    unit->x_pos = pos.x;
    unit->y_pos = pos.y;
    unit->enemy = false;
    unit->attack_cooldown = 0;
    unit->is_attacking = false;
    unit->attack_anim_timer = 0;

    switch (unit_type) {
        case 0:
            unit->default_width = pc1_img.width;
            unit->default_height = pc1_img.height;
            unit->default_sprite = pc1_sprite_data;
            unit->attack_sprite = ah1_sprite_data;
            unit->attack_width = ah1_img.width;
            unit->attack_height = ah1_img.height;
            unit->attack_sprite2 = pc1_attack2_img;
            unit->attack2_width = pc1_attack2.width;
            unit->attack2_height = pc1_attack2.height;
            unit->sprite = unit->default_sprite;
            unit->width = unit->default_width;
            unit->height = unit->default_height;
            unit->damage = 15;
            unit->health = 120;
            unit->pixels_per_sec = 120;
            unit->attack_range = 60;
            break;
        case 1:
            unit->default_width = pc2_img.width;
            unit->default_height = pc2_img.height;
            unit->default_sprite = pc2_sprite_data;
            unit->attack_sprite = ah2_sprite_data;
            unit->attack_width = ah2_img.width;
            unit->attack_height = ah2_img.height;
            unit->attack_sprite2 = pc2_attack2_img;
            unit->attack2_width = pc2_attack2.width;
            unit->attack2_height = pc2_attack2.height;
            unit->sprite = unit->default_sprite;
            unit->width = unit->default_width;
            unit->height = unit->default_height;
            unit->damage = 10;
            unit->health = 80;
            unit->pixels_per_sec = 120;
            unit->attack_range = 100;
            break;
        case 2:
            unit->default_width = pc3_img.width;
            unit->default_height = pc3_img.height;
            unit->default_sprite = pc3_sprite_data;
            unit->attack_sprite = ah3_sprite_data;
            unit->attack_width = ah3_img.width;
            unit->attack_height = ah3_img.height;
            unit->attack_sprite2 = pc3_attack2_img;
            unit->attack2_width = pc3_attack2.width;
            unit->attack2_height = pc3_attack2.height;
            unit->sprite = unit->default_sprite;
            unit->width = unit->default_width;
            unit->height = unit->default_height;
            unit->damage = 25;
            unit->health = 200;
            unit->pixels_per_sec = 120;
            unit->attack_range = 40;
            break;
        default:
            printf("Invalid player unit type: %d\n", unit_type);
            free(unit);
            return NULL;
    }

    return unit;
}

/**
 * @brief Adds a newly created player unit to the game.
 * Creates a player unit and adds it to the `active_player_chars` array.
 * @param pos The initial position of the unit.
 * @param unit_type The type of player unit to create.
 */

void add_player_unit_to_game(CharacterPos pos, int unit_type) {
    if (num_active_player_chars >= MAX_PLAYER_CHARACTERS) {
        printf("Max player characters reached.\n");
        return;
    }

    Character *new_unit = create_player_unit(pos, unit_type);
    if (new_unit) {
        active_player_chars[num_active_player_chars++] = new_unit;
        printf("Player unit type %d spawned at (%u, %u).\n", unit_type, pos.x, pos.y);
    } else {
        printf("Failed to create player unit of type %d.\n", unit_type);
    }
}

/**
 * @brief Updates all active player units and handles spawning logic.
 * This function iterates through active player units, updating their state:
 *  - Checks for enemies in range and initiates attacks.
 *  - Manages attack cooldown and animation.
 *  - Moves units forward if no enemy is in range.
 *  - Handles units reaching the screen edge (damaging enemy base).
 *  - Despawns units that move off-screen.
 *  This function does not actually spawn units; spawning is handled by `add_player_unit_to_game`.
 */

void update_and_spawn_player_units(void) {
    if (timer_frequency_hz_hero == 0) return;

    for (int i = 0; i < num_active_player_chars;) {
        Character *unit = active_player_chars[i];
        if (!unit) {
            i++;
            continue;
        }

        bool enemy_in_range = false;
        unit->attack_cooldown++;

        for (int j = 0; j < num_active_enemies; j++) {
            Character *enemy = active_enemies[j];
            if (!enemy || !enemy->health) continue;

            int dx = (int)(enemy->x_pos) - (int)(unit->x_pos + unit->width);
            int dy = abs((int)(enemy->y_pos) - (int)(unit->y_pos));

            if (dx >= 0 && dx <= (int)unit->attack_range && dy < 30) {
                enemy_in_range = true;

                if (unit->attack_cooldown >= ATTACK_COOLDOWN_TICKS) {
                    enemy->health = (enemy->health > unit->damage) ? enemy->health - unit->damage : 0;
                    unit->attack_cooldown = 0;

                    if (unit->attack_sprite != NULL) {
                        unit->sprite = unit->attack_sprite;
                        unit->width = unit->attack_width;
                        unit->height = unit->attack_height;
                        unit->is_attacking = true;
                        unit->attack_anim_timer = 30;
                    }
                }
                break;  // only attack the first valid enemy in range
            }
            
        }

        // Handle attack animation duration
        if (unit->is_attacking) {
            unit->attack_anim_timer--;

            if (unit->attack_anim_timer > 15) {
                unit->sprite = unit->attack_sprite;
                unit->width = unit->attack_width;
                unit->height = unit->attack_height;
            } else if (unit->attack_anim_timer > 0) {
                unit->sprite = unit->attack_sprite2;
                unit->width = unit->attack2_width;
                unit->height = unit->attack2_height;
            } else {
                unit->sprite = unit->default_sprite;
                unit->width = unit->default_width;
                unit->height = unit->default_height;
                unit->is_attacking = false;
            }
        }

        // Only move if no enemy is in range
        if (!enemy_in_range) {
            unsigned int pixels_to_move = unit->pixels_per_sec / timer_frequency_hz_hero;
            unit->x_pos += pixels_to_move;
        }
        
        if (unit->x_pos == SCREEN_WIDTH) {
            enemy_health->health = enemy_health->health - 10;
            printf("Enemy health: %u\n", enemy_health->health);
        }

        // Despawn unit if off-screen
        if ((int)unit->x_pos > SCREEN_WIDTH) {
            free(unit);
            for (int j = i; j < num_active_player_chars - 1; j++) {
                active_player_chars[j] = active_player_chars[j + 1];
            }
            active_player_chars[--num_active_player_chars] = NULL;
        } else {
            i++;
        }
    }

}

/**
 *  @brief Draws all active player units on the screen.
 *  Iterates through `active_player_chars` and draws each unit's current sprite.
 */

void draw_player_units(void) {
    for (int i = 0; i < num_active_player_chars; i++) {
        Character *unit = active_player_chars[i];
        if (unit && unit->sprite) {
            uint16_t *pixels = (uint16_t *)unit->sprite;
            for (uint16_t y = 0; y < unit->height; y++) {
                for (uint16_t x = 0; x < unit->width; x++) {
                    uint16_t color = pixels[y * unit->width + x];
                    if (color == TRANSPARENT_COLOR) continue;
                    vg_draw_pixel(unit->x_pos + x, unit->y_pos + y, color);
                }
            }
        }
    }
}

/**
 * @brief Sets the timer frequency for player character updates.
 * @param hz The new frequency in Hertz.
 */

void set_timer_frequency_for_heroes(unsigned int hz) {
    timer_frequency_hz_hero = hz;
}
