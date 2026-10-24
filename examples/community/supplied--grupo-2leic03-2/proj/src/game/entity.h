/**
 * @file entity.h
 * @brief Base entity structures.
 */
#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Base structure for game entities.
 */
typedef struct {
  int x;      /**< X position */
  int y;      /**< Y position */
  int vx;     /**< X velocity */
  int vy;     /**< Y velocity */
  int width;  /**< Entity width */
  int height; /**< Entity height */
  bool active;/**< True if the entity is active */
} Entity;

#endif
