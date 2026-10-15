/**
 * @file help_view.h
 * @brief Handles the visual representation of the Help screen
 * 
 * This file contains declarations for functions related to rendering
 * the Help screen, which provides information about game controls and
 * instructions to the player.
 */

#ifndef HELP_VIEW_H
#define HELP_VIEW_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "controller/graphics.h"

#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"
#include "model/menu.h"
#include "view/sprite_view.h"

/**
 * @brief Draws the static help/controls screen (XPM)
 * @param buffer Target video buffer
 */
void draw_help(uint8_t *buffer);

#endif
