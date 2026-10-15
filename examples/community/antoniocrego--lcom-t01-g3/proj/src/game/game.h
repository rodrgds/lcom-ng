#ifndef LCOM_GAME_H
#define LCOM_GAME_H

#include <lcom/lcf.h>
#include "hitbox.h"
#include "../sprites/sprites.h"

/**
* @brief Enum representating the different game states
*/
typedef enum game_state{
    MAINMENU,   /**< Main menu state */
    RUNNING,    /**< Running state */
    PAUSED,     /**< Paused state */
    DYING,      /**< Dying state */
    GAMEOVER,   /**< Game over state */
    CUTSCENE,   /**< Cutscene state */
    RESULTS     /**< Results state */
} game_state;


/**
 * @brief Enum representing the different player states
 */
typedef enum player_state{
    WALKING,    /**< Walking state */
    JUMPING     /**< Jumping state */
} player_state;

/**
 * @brief Struct representing a character class
 */
typedef struct character_class{
    int x_pos;                              /**< X position to start drawing top left */
    int y_pos;                              /**< Y position to start drawing top left */
    int x_vel;                              /**< X velocity */
    int y_vel;                              /**< Y velocity */
    uint8_t hp;                                 /**< Health points */
    uint32_t inv_time;                           /**< Invincibility time */
    int (*drawChar)(Sprite*, int, int);     /**< Function pointer to draw the character */
    invis_hitbox hitbox;                    /**< Invisible hitbox */
    Sprite* char_sprite;                    /**< Character sprite */
} character_class;

/**
 * @brief Struct representing a player character
 */
typedef struct player_character{
   character_class character;       /**< Character class */
    invis_hitbox feethitbox;        /**< Feet hitbox */
    player_state state;             /**< Player state */
    bool hasDoubleJump;             /**< Indicates if the player has double jump */
    bool hasSoulBlast;              /**< Indicates if the player has soul blast */
} player_character;

/**
 * @brief Struct representing an enemy character
 */

typedef struct enemy_character{
    character_class character;      /**< Character class */
    invis_hitbox feethitbox;        /**< Feet hitbox */
    invis_hitbox lim_hitbox;        /**< Limit hitbox */
    uint8_t damage;                     /**< Damage */
    uint8_t head_collision;            /**< Head collision flag */
    uint8_t feet_collision;            /**< Feet collision flag */
    int std_x_vel;                  /**< Standard X velocity */
    int std_y_vel;                  /**< Standard Y velocity */
    int (*enemyAI)(struct enemy_character*, invis_hitbox, player_character); /**< Function pointer to enemy AI */
} enemy_character;

/**
 * @brief Struct representing a boss character
 */

typedef struct boss_character{
    enemy_character enemystruct;    /**< Enemy character struct */
    Sprite* array[4];               /**< Array of sprites */
    uint32_t animation_time;             /**< Animation time */
} boss_character;


/**
 * @brief Struct representing a level
 */
typedef struct level{
    uint8_t level_num;                  /**< Level number */
    int (*addHitboxes)();           /**< Function pointer to add hitboxes */
    Sprite* level_sprite;           /**< Level sprite */
} level;

/**
 * @brief Struct representing an attack
 */
typedef struct attack{
    int x_pos;                          /**< X position of the attack */
    int y_pos;                          /**< Y position of the attack */
    int x_vel;                          /**< X velocity of the attack */
    int y_vel;                          /**< Y velocity of the attack */
    uint8_t damage;                         /**< Damage inflicted by the attack */
    invis_hitbox hitbox;                /**< Invisible hitbox of the attack */
    uint32_t end_time;                       /**< End time of the attack */
    Sprite* sprite;                     /**< Sprite associated with the attack */
    int (*drawFunc)(Sprite*, int, int); /**< Function pointer to draw the attack */
} attack;

/**
* @brief Inicializes a player character with default values.

* @return The initialized player character
*/
struct player_character init_player();

/**
 * @brief Initializes an enemy character
 *
 * @param sprite The sprite of the enemy character.
 * @param x The initial X position.
 * @param y The initial Y position.
 * @param hp The initial health points.
 * @param damage The damage value.
 * @param x_vel The initial X velocity.
 * @param y_vel The initial Y velocity.
 * @param enemyAI The function pointer to the enemy AI.
 * @return The initialized enemy character.
 */
struct enemy_character init_enemy(Sprite* sprite, int x, int y, uint8_t hp, uint8_t damage,  int x_vel, int y_vel, int (*enemyAI)(enemy_character*, invis_hitbox, player_character));

/**
 * @brief AI for standard enemies.
 *
 * @param enemy The enemy character.
 * @param hbox The invisible hitbox.
 * @param player The player character.
 * @return The result of the AI operation.
 */
int standard_enemy_ai(struct enemy_character* enemy, invis_hitbox hbox, player_character player);

/**
 * @brief AI for flying enemies.
 *
 * @param enemy The enemy character.
 * @param hbox The invisible hitbox.
 * @param player The player character.
 * @return The result of the AI operation.
 */
int flying_enemy_ai(enemy_character* enemy, invis_hitbox hbox, player_character player);

/**
 * @brief Applies knockback to a character.
 *
 * @param character Pointer to the character to apply knockback to.
 * @param dir Direction of the knockback (left or right).
 * @param knockback Magnitude of the knockback.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int knockback(character_class* character, uint8_t dir, uint16_t knockback);

/**
 * @brief No AI implementation for an enemy character.
 *
 * @param enemy Pointer to the enemy character.
 * @param hbox Invisible hitbox.
 * @param player Player character.
 * @return The result of the AI operation.
 */
int no_ai(struct enemy_character* enemy, invis_hitbox hbox, player_character player);

/**
 * @brief Initializes all hitboxes.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int init_all_hitboxes();

/**
 * @brief Generates a randomly positioned enemy character.
 *
 * @return The generated enemy character.
 */
enemy_character generateRandomPositionEnemy();

/**
 * @brief Updates the position of an attack.
 *
 * @param attack Pointer to the attack to update.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int update_attack_position(attack* attack);

/**
 * @brief Updates the position of a player character.
 *
 * @param player Pointer to the player character to update.
 * @param x New X position.
 * @param y New Y position.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int update_player_position(player_character* player, int x, int y);

/**
 * @brief Updates the position of an enemy character.
 *
 * @param enemy Pointer to the enemy character to update.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int update_enemy_position(enemy_character* enemy);

/**
 * @brief Determines the direction of an attack based on the player's position and the mouse coordinates.
 *
 * @param player Player character.
 * @param mouseX X coordinate of the mouse.
 * @param mouseY Y coordinate of the mouse.
 * @return The direction of the attack (left or right).
 */
uint8_t attack_dir(player_character player, int mouseX, int mouseY);

/**
 * @brief Clears an attack.
 *
 * @param attack Pointer to the attack to clear.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int clear_attack(attack* attack);

/**
 * @brief Performs a slash attack for the player character.
 *
 * @param player Player character.
 * @param slash_entity Pointer to the slash attack entity.
 * @param mouseX X coordinate of the mouse.
 * @param mouseY Y coordinate of the mouse.
 * @param timerCounter Timer counter value.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int slash(player_character player, attack* slash_entity, int mouseX, int mouseY, uint32_t timerCounter);

/**
 * @brief Performs a blast attack for the player character.
 *
 * @param player Player character.
 * @param blast_entity Pointer to the blast attack entity.
 * @param mouseX X coordinate of the mouse.
 * @param mouseY Y coordinate of the mouse.
 * @param timerCounter Timer counter value.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int blast(player_character player, attack* blast_entity, int mouseX, int mouseY, uint32_t timerCounter);

/**
 * @brief Handles interactions between the player character and enemies.
 *
 * @param player Pointer to the player character.
 * @param timerCounter Timer counter value
 * @param slash_entity Pointer to the slash attack entity.
 * @param blast_entity Pointer to the blast attack entity.
 * @param game_state Pointer to the game state.
 * @param store_timer Pointer to the store timer value.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int enemies_interactions(player_character *player, uint32_t timerCounter, attack *slash_entity, attack *blast_entity, game_state *game_state, uint32_t* store_timer);

/**
 * @brief Draws the HUD (Heads-Up Display) on the screen.
 *
 * @param health Current health value.
 * @param healthMax Maximum health value.
 * @param pos_X X position of the HUD.
 * @param pos_Y Y position of the HUD.
 */
void drawHUD(uint8_t health, uint8_t healthMax, int pos_X, int pos_Y);

/**
 * @brief Generates Level 1.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel1();

/**
 * @brief Generates Level 2.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel2();

/**
 * @brief Generates Level 3.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel3();

/**
 * @brief Generates Level 4.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel4();

/**
 * @brief Generates Level 5.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel5();

/**
 * @brief Generates Level 6.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel6();

/**
 * @brief Generates Level 7.
 *
 * @return Returns 0 on success, or a non-zero value on failure.
 */
int generateLevel7();


#endif
