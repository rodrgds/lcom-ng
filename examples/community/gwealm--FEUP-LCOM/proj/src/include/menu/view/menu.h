#pragma once

#include <lcom/lcf.h>

#include "menu/model/menu.h"

/**
 * @brief Draws menu
 * 
 */
void (menu_draw)();

/**
 * @brief Draws menu title
 * 
 */
void (menu_draw_title)();

/**
 * @brief Draws menu options
 * 
 */
void (menu_draw_option)(menu_option *opt);
