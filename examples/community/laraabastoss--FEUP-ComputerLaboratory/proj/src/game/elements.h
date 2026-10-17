#ifndef _LCOM_ELEMENTS_H_
#define _LCOM_ELEMENTS_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "devices/video/graphics.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "sprites/sprite.h"
#include "handle_int.h"

/// @brief loads a previously allocated array with the current level using a txt file
/// @param level current level of the user
void updateArrayWithLevel(int level);

/// @brief alocates memory for an array of size 1200
void alocateLevelArrayMemory();

/// @brief frees the allocated array for the level
void freeLevelArray();

/// @brief moves the plataform upwards
/// @param max_height maximum height the plataform can go to
void checkPlatformLogic(int max_height);

/// @brief updates FireBoy to the desired next position, checking if he can go there according to the bit map and his current state
/// @param x x coordinate the FireBoy wants to go to
/// @param y y coordinate the FireBoy wants to go to
void updateFireboy(int x, int y);

/// @brief updates WaterGirl to the desired next position, checking if she can go there according to the bit map and her current state
/// @param x x coordinate the WaterGirl wants to go to
/// @param y y coordinate the WaterGirl wants to go to
void updateWatergirl(int x, int y);

/// @brief checks if Fireboy is on top of a plataform, if not he falls
/// @param x x coordinate that the FireBoy is in
/// @param y y coordinate that the FireBoy is in
void checkGravityFireboy(int x, int y);

/// @brief  checks if WaterGirl is on top of a plataform, if not she falls
/// @param x x coordinate that the WaterGirl is in
/// @param y y coordinate that the WaterGirl is in
void checkGravityWatergirl(int x, int y);

#endif

