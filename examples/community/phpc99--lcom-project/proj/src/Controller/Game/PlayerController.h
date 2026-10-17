#ifndef _PLAYERCONTROLLER_H_
#define _PLAYERCONTROLLER_H_

#include "src/include.h"
#include "src/Controller/Game/GameController.h"
#include "src/Model/Elements/player.h"
#include "src/Controller/Game/MapController.h"

/**
 * @file PlayerController.h
 * @brief Declares functions and constants for managing player movement and input handling.
 *
 * Handles the player's position updates based on physics
 * and user input.
 */

#define GRAVITY 1
#define JUMP_VELOCITY -12
#define MOVE_SPEED 6

extern bool left_pressed;
extern bool right_pressed;
extern bool jump;

void update_player();
void handle_player_input(uint8_t scan_code[2]);

#endif
