#ifndef WORM_CONTROLLER_H
#define WORM_CONTROLLER_H

#include "worm.h"
#include "../../engine/input/input.h"

/**
 * @brief Reads all currently pressed keys and applies the resulting movement
 *        commands to the worm. Safe to call every game tick.
 *
 * Checks KEY_LEFT, KEY_RIGHT and KEY_SPACE independently, so combinations
 * such as LEFT + SPACE (walk and jump) are handled correctly.
 *
 * @param worm Target worm. NULL is accepted (worm model guards its operations).
 */

typedef enum {
  WORM_COMMAND_NONE,
  WORM_COMMAND_MOVE_LEFT,
  WORM_COMMAND_MOVE_RIGHT,
  WORM_COMMAND_JUMP,
  WORM_COMMAND_EQUIP_BAZOOKA,
  WORM_COMMAND_EQUIP_GRENADE
} WormCommand;

/**
 * @brief Converts one game-level input key into a worm command.
 * @param key Abstract input key produced by the input layer.
 * @return Corresponding worm command, or WORM_COMMAND_NONE if unrelated.
 */
WormCommand worm_controller_command_from_key(InputKey key);

/**
 * @brief Applies one worm command to the worm model.
 * @param worm Worm to control. NULL is accepted by the model operations.
 * @param command Command to apply.
 */
void worm_controller_apply_command(Worm *worm, WormCommand command);

/**
 * @brief Legacy polling function for state machine.
 */
void worm_controller_apply_keys(Worm *worm);

#endif /* WORM_CONTROLLER_H */
