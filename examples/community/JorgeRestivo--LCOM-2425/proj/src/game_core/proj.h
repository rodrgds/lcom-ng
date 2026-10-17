/**
 * @file proj.h
 * @brief Project entry points and menu/state management.
 */

#ifndef PROJ_H
#define PROJ_H
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "../menu/menu.h"

/**
 * @brief Keyboard interrupt mask.
 */
uint8_t kb_mask;

/**
 * @brief Timer interrupt mask.
 */
uint8_t timer_mask;

/**
 * @brief Mouse interrupt mask.
 */
uint8_t mouse_mask;

/**
 * @brief Main project loop.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, non-zero on error.
 */
int(proj_main_loop)(int argc, char* argv[]);

#endif
