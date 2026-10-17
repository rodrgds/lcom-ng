//
// Created by joao on 23-04-2024.
//
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "Controller/Timer/timer.h"
#include "Controller/Timer/i8254.h"
#include "Controller/Video/graphic.h"
#include "Controller/Keyboard/keyboard.h"
#include "Controller/Mouse/mouse.h"
#include "Controller/RTC//rtc.h"
#include "configs.h"
#include "sprite.h"
#include "Viewer/view.h"
#ifndef SHARED_MODEL_H
#define SHARED_MODEL_H


/**
 * @brief Creates all the necessary sprites for the game.
 * 
 * This function initializes and allocates memory for all the sprite instances used in the game,
 * loading each sprite's pixel data from corresponding XPM images.
 */

void create_sprites();

/**
 * @brief Destroys and frees memory for all the sprites created.
 * 
 * This function releases the memory allocated for each sprite instance,
 * ensuring that all dynamically allocated memory is properly freed to avoid memory leaks.
 */

void destroy_sprites();

#endif //SHARED_MODEL_H
