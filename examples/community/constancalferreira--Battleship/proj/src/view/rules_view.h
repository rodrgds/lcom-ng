#ifndef RULES_VIEW_H
#define RULES_VIEW_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "controller/graphics.h"

#include "model/game_macro.h"
#include "model/game.h"
#include "model/sprite.h"
#include "model/menu.h"
#include "view/sprite_view.h"

/**
 * @brief Draws the static rules screen (XPM)
 * @param buffer Target video buffer
 */
void draw_rules(uint8_t *buffer);

#endif
