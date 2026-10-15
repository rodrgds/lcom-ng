#ifndef __PROJ_RESOURCES_H
#define __PROJ_RESOURCES_H

/* UI and interface elements */
#include "assets/xpm/ui/handopen.xpm"
#include "assets/xpm/ui/handpointing.xpm"
#include "assets/xpm/ui/heart.xpm"
#include "assets/xpm/ui/lose.xpm"
#include "assets/xpm/ui/menu_default.xpm"
#include "assets/xpm/ui/menu_exit.xpm"
#include "assets/xpm/ui/menu_pause.xpm"
#include "assets/xpm/ui/menu_pause_continue.xpm"
#include "assets/xpm/ui/menu_pause_exit.xpm"
#include "assets/xpm/ui/menu_start.xpm"
#include "assets/xpm/ui/win.xpm"

/* Game objects */
#include "assets/xpm/game/brick_0.xpm"
#include "assets/xpm/game/brick_1.xpm"
#include "assets/xpm/game/brick_2.xpm"
#include "assets/xpm/game/door.xpm"
#include "assets/xpm/game/powerup.xpm"
#include "assets/xpm/game/wall.xpm"

/* Text and fonts */
#include "assets/xpm/text/alphabet.xpm"
#include "assets/xpm/text/numbers.xpm"

/* Bomb animation cycle */
#include "assets/xpm/bomb/bomb_1.xpm"
#include "assets/xpm/bomb/bomb_2.xpm"
#include "assets/xpm/bomb/bomb_3.xpm"
#include "assets/xpm/bomb/bomb_4.xpm"

/* Enemy animation cycle */
#include "assets/xpm/enemy/enemy_a_1.xpm"
#include "assets/xpm/enemy/enemy_a_2.xpm"
#include "assets/xpm/enemy/enemy_a_3.xpm"
#include "assets/xpm/enemy/enemy_a_4.xpm"

/* Player animation cycles */
#include "assets/xpm/player/player_down_1.xpm"
#include "assets/xpm/player/player_down_2.xpm"
#include "assets/xpm/player/player_down_3.xpm"
#include "assets/xpm/player/player_down_4.xpm"
#include "assets/xpm/player/player_up_1.xpm"
#include "assets/xpm/player/player_up_2.xpm"
#include "assets/xpm/player/player_up_3.xpm"
#include "assets/xpm/player/player_up_4.xpm"
#include "assets/xpm/player/player_left_1.xpm"
#include "assets/xpm/player/player_left_2.xpm"
#include "assets/xpm/player/player_left_3.xpm"
#include "assets/xpm/player/player_left_4.xpm"
#include "assets/xpm/player/player_right_1.xpm"
#include "assets/xpm/player/player_right_2.xpm"
#include "assets/xpm/player/player_right_3.xpm"
#include "assets/xpm/player/player_right_4.xpm"

/* Explosion animation cycles */
#include "assets/xpm/explosion/explosion_center_1.xpm"
#include "assets/xpm/explosion/explosion_center_2.xpm"
#include "assets/xpm/explosion/explosion_center_3.xpm"
#include "assets/xpm/explosion/explosion_center_4.xpm"
#include "assets/xpm/explosion/explosion_arm_h_1.xpm"
#include "assets/xpm/explosion/explosion_arm_h_2.xpm"
#include "assets/xpm/explosion/explosion_arm_h_3.xpm"
#include "assets/xpm/explosion/explosion_arm_h_4.xpm"
#include "assets/xpm/explosion/explosion_arm_v_1.xpm"
#include "assets/xpm/explosion/explosion_arm_v_2.xpm"
#include "assets/xpm/explosion/explosion_arm_v_3.xpm"
#include "assets/xpm/explosion/explosion_arm_v_4.xpm"
#include "assets/xpm/explosion/explosion_hand_right_1.xpm"
#include "assets/xpm/explosion/explosion_hand_right_2.xpm"
#include "assets/xpm/explosion/explosion_hand_right_3.xpm"
#include "assets/xpm/explosion/explosion_hand_right_4.xpm"
#include "assets/xpm/explosion/explosion_hand_left_1.xpm"
#include "assets/xpm/explosion/explosion_hand_left_2.xpm"
#include "assets/xpm/explosion/explosion_hand_left_3.xpm"
#include "assets/xpm/explosion/explosion_hand_left_4.xpm"
#include "assets/xpm/explosion/explosion_hand_up_1.xpm"
#include "assets/xpm/explosion/explosion_hand_up_2.xpm"
#include "assets/xpm/explosion/explosion_hand_up_3.xpm"
#include "assets/xpm/explosion/explosion_hand_up_4.xpm"
#include "assets/xpm/explosion/explosion_hand_down_1.xpm"
#include "assets/xpm/explosion/explosion_hand_down_2.xpm"
#include "assets/xpm/explosion/explosion_hand_down_3.xpm"
#include "assets/xpm/explosion/explosion_hand_down_4.xpm"

#include "model/entity/entity.h"
#include "model/sprite/anim_sprite.h"
#include "model/sprite/sprite.h"

#define ENEMY_ANIM_FRAMES 4
#define ENEMY_ANIM_SPEED 2
#define PLAYER_ANIM_FRAMES 4
#define PLAYER_ANIM_SPEED 3
#define BOMB_ANIM_FRAMES 4
#define BOMB_ANIM_SPEED 6
#define EXPLOSION_ANIM_FRAMES 4
#define EXPLOSION_ANIM_SPEED 4

/** @defgroup resources resources
 * @{
 *
 * Functions for managing game resources
 */

/**
 * @brief Struct representing game resources
 *
 * Contains pointers to all sprites used in the game, organized by type.
 */
typedef struct {
  Sprite *brick_sprites[3];                   /**< @brief Brick sprites (damaged states) */
  Sprite *door_sprite;                        /**< @brief Door sprite */
  Sprite *enemy_a_sprites[ENEMY_ANIM_FRAMES]; /**< @brief Enemy animation sprites */
  Sprite *handopen_sprite;                    /**< @brief Open hand cursor sprite */
  Sprite *handpointing_sprite;                /**< @brief Pointing hand cursor sprite */
  Sprite *heart_sprite;                       /**< @brief Heart/life indicator sprite */
  Sprite *lose_sprite;                        /**< @brief Game over screen sprite */
  Sprite *menu_sprite[3];                     /**< @brief Main menu sprites */
  Sprite *menu_pause[3];                      /**< @brief Pause menu sprites */

  Sprite *player_up_sprites[PLAYER_ANIM_FRAMES];    /**< @brief Player upward movement sprites */
  Sprite *player_left_sprites[PLAYER_ANIM_FRAMES];  /**< @brief Player leftward movement sprites */
  Sprite *player_down_sprites[PLAYER_ANIM_FRAMES];  /**< @brief Player downward movement sprites */
  Sprite *player_right_sprites[PLAYER_ANIM_FRAMES]; /**< @brief Player rightward movement sprites */

  Sprite *bomb_sprites[BOMB_ANIM_FRAMES];           /**< @brief Bomb animation sprites */

  Sprite *explosion_center_sprites[EXPLOSION_ANIM_FRAMES];     /**< @brief Explosion center sprites */
  Sprite *explosion_horiz_sprites[EXPLOSION_ANIM_FRAMES];      /**< @brief Horizontal explosion arm sprites */
  Sprite *explosion_vert_sprites[EXPLOSION_ANIM_FRAMES];       /**< @brief Vertical explosion arm sprites */
  Sprite *explosion_hand_up_sprites[EXPLOSION_ANIM_FRAMES];    /**< @brief Upward explosion end cap sprites */
  Sprite *explosion_hand_down_sprites[EXPLOSION_ANIM_FRAMES];  /**< @brief Downward explosion end cap sprites */
  Sprite *explosion_hand_left_sprites[EXPLOSION_ANIM_FRAMES];  /**< @brief Leftward explosion end cap sprites */
  Sprite *explosion_hand_right_sprites[EXPLOSION_ANIM_FRAMES]; /**< @brief Rightward explosion end cap sprites */

  Sprite *powerup_sprite;  /**< @brief Powerup sprite */
  Sprite *numbers_sprite;  /**< @brief Numbers font sprite sheet */
  Sprite *alphabet_sprite; /**< @brief Alphabet font sprite sheet */
  Sprite *win_sprite;      /**< @brief Victory screen sprite */
  Sprite *wall_sprite;     /**< @brief Wall sprite */
} Resources;

/**
 * @brief Creates all game resources
 *
 * Allocates and initializes all the sprites used in the game.
 *
 * @return 0 upon success, non-zero otherwise
 */
int(create_resources)(void);

/**
 * @brief Destroys all game resources
 *
 * Frees the memory allocated for all the sprites used in the game.
 */
void(destroy_resources)(void);

/**
 * @brief Retrieves the game resources
 *
 * Provides access to the `Resources` struct containing all the game sprites.
 *
 * @return Pointer to the `Resources` struct
 */
const Resources *(get_resources) (void);

/**
 * @brief Retrieves the animated sprite for player movement in the specified direction
 *
 * Returns a pre-created AnimSprite object for the player's movement animation
 * in the given direction. These animations are initialized when resources are created.
 *
 * @param dir Direction for which to get the animation (UP, DOWN, LEFT, RIGHT)
 * @return Pointer to the AnimSprite for the specified direction
 */
AnimSprite *get_player_anim(Direction dir);

/**@}*/

#endif /* __PROJ_RESOURCES_H */
