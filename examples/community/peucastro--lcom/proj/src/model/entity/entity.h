#ifndef __PROJ_ENTITY_H
#define __PROJ_ENTITY_H

#include "model/board/board.h"
#include "model/sprite/anim_sprite.h"

struct Game;

/** @defgroup entity entity
 * @{
 *
 * Module for managing game entities
 *
 * Provides core functionality for game objects like players, enemies, and bombs.
 * Defines the base Entity struct and common operations for all entity types.
 */

/**
 * @brief Enum representing entity facing directions
 */
typedef enum {
  UP,    /**< @brief Entity is facing upward */
  RIGHT, /**< @brief Entity is facing right */
  DOWN,  /**< @brief Entity is facing downward */
  LEFT   /**< @brief Entity is facing left */
} Direction;

/**
 * @brief Component for handling smooth movement between grid positions
 *
 * Manages interpolated movement from one grid cell to another over time.
 */
typedef struct {
  float px, py;         /**< @brief Current pixel position for smooth movement */
  int16_t tx, ty;       /**< @brief Target grid coordinates */
  int16_t sx, sy;       /**< @brief Source grid coordinates */
  bool moving;          /**< @brief Whether entity is currently moving */
  uint16_t tick;        /**< @brief Current movement tick counter */
  uint16_t total_ticks; /**< @brief Total ticks required for complete movement */
} MoveComp;

/**
 * @brief Base game entity structure
 *
 * Represents any game object with position, visual representation, and behavior.
 */
typedef struct Entity {
  int16_t x, y;   /**< @brief Grid position coordinates */
  Direction dir;  /**< @brief Entity's facing direction */
  Sprite *sprite; /**< @brief Current visual representation */
  bool active;    /**< @brief Whether entity is currently in play */
  int16_t data;   /**< @brief Entity-specific data (lives, damage, etc.) */

  MoveComp move;    /**< @brief Movement component for smooth transitions */
  AnimSprite *anim; /**< @brief Animated sprite for this entity (optional) */

  /**< @brief Update function pointer for entity-specific behavior */
  void (*update)(struct Entity *self, struct Game *game, uint32_t timer_counter);
} Entity;

/**
 * @brief Initialize an entity
 *
 * Sets up an entity with initial position, sprite, and data values.
 * Initializes movement component and sets default update function.
 *
 * @param e Entity to initialize
 * @param x X-coordinate on the game grid
 * @param y Y-coordinate on the game grid
 * @param sp Sprite for visual representation
 * @param data Entity-specific data value
 *
 * @return 0 on success, non-zero otherwise
 */
int(init_entity)(Entity *e, int16_t x, int16_t y, Sprite *sp, int16_t data);

/**
 * @brief Reset an entity to inactive state
 *
 * Clears all entity data and sets it to inactive.
 * Destroys any associated animated sprite.
 *
 * @param e Entity to reset
 * @return 0 on success, non-zero otherwise
 */
int(reset_entity)(Entity *e);

/**
 * @brief Default update function for entities
 *
 * Handles smooth movement interpolation and animation updates.
 * Called each game tick for entities using default behavior.
 *
 * @param self Pointer to the entity being updated
 * @param game Pointer to the current game state
 * @param counter Current timer counter value
 */
void(default_update)(Entity *self, struct Game *game, uint32_t counter);

/**@}*/

#endif /* __PROJ_ENTITY_H */
