#pragma once

/** @defgroup Score Score
 * @{
 *
 * Functions from the Scores Module
 */

/**
 * @brief Read Scores from File
 * 
 * @return Return OK upon success and NOT_OK otherwise
*/
int readScores();

/**
 * @brief Update Scores from Current Score 
*/
void updateScores();

/**
 * @brief Write Scores to File
 * 
 * @return Return OK upon success and NOT_OK otherwise
*/
int writeScores();

/**
 * @brief Event Handler for Mouse on Score
 *
 * @param p Mouse packet to be parsed
*/
void ScoreMouseEventHandler(struct packet p);

/**
 * @brief Event Handler for Keyboard on Score State
*/
void ScoreKbdEventHandler();

/**
 * @brief Draw Digits on Screen 
 * 
 * @param value to be parsed and drawn on screen
 * @param h X position for the first digit
 * @param v Y position for the first digit
 * @param n number of digits
*/
void drawDigits(int value,int h, int v, int n);

/**
 * @brief Scores Graphic Interface
*/
void ScoresDrawInterface();
