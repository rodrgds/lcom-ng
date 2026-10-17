#pragma once
#include "Bitmap.h"

/** @defgroup UI_game UI_game
 * @{
 * Functions for manipulating game UI drawing
 */

/**
 * @brief      Initialize background
 */
void init_background();

/**
 * @brief      Initialize UI structure
 */
void initUI();

/**
 * @brief      Destroy UI structure and free memory used by it
 */
void destroyUI();

/**
 * @brief      Clear UI information from previous frame
 */
void clearUI();

/**
 * @brief      Draw corresponding background depending on hour of day
 */
void draw_background();

/**
 * @brief      Draw title Bitmap on screen centered to x axis
 *
 * @param      bmp   { title Bitmap }
 * @param[in]  y     { y coordinate }
 */
void draw_title(Bitmap* bmp, int y);

/**
 * @brief      Draw current player lives
 */
void drawLives();

/**
 * @brief      Draw current player points
 */
void drawPoints();

/**
 * @brief      Draw current player and adversary points
 */
void drawPointsTwo();

/**
 * @brief      Draw text informing a wave of zombies progression
 */
void drawWave();

/**
 * @brief      Draw time played from start of game
 */
void drawTimer();

/**
 * @brief      Draw magazine of current selected weapon
 */
void drawMagazine();

/**
 * @brief      Draw currently reloading text
 */
void drawReloadingText();

/**
 * @brief      Draw current time provided from RTC
 */
void drawTimeRTC();

/**
 * @brief      Draw weapon slots and selected weapon
 */
void drawWeaponSlot();

/**
 * @brief      Schedule lives drawing for next frame
 *
 * @param[in]  lives  { current player's lives }
 */
void scheduleLives(int lives);

/**
 * @brief      Schedule points drawing for next frame
 *
 * @param[in]  points  { current player's points }
 */
void schedulePoints(int points);

/**
 * @brief      Schedule points drawing from player and adversary for next frame
 *
 * @param[in]  points1  { current player's points }
 * @param[in]  points2  { current player's adversary points }
 */
void schedulePointsTwo(int points1, int points2);

/**
 * @brief      Schedule next wave text drawing for next frame
 *
 * @param[in]  wave  { currently entering wave number }
 */
void scheduleWave(int wave);

/**
 * @brief      Schedule timer drawing for next frame
 *
 * @param[in]  timer_s  { current time played }
 */
void scheduleTimer(int timer_s);

/**
 * @brief      Schedule current and max weapon magazine drawing for next frame
 *
 * @param[in]  current  { current magazine }
 * @param[in]  max      { max magazine }
 */
void scheduleMagazine(int current, int max);

/**
 * @brief      Schedule reloading text drawing for next frame
 */
void scheduleReloadingText();

/**
 * @brief      Schedule weapons slot drawing for next frame 
 *
 * @param[in]  active_id  { id of currently selected weapon }
 */
void scheduleWeaponSlot(int active_id);

/**
 * @brief      Draw UI
 */
void drawUI();
