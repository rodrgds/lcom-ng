/**
 * @file gameView.h
 * @brief This file contains the functions responsible for drawing everything in the game
 * @date 2024-06-02
 */
#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_

#include <lcom/lcf.h>

#include <math.h>

#include "drivers/gpu.h"
#include "../models/Game.h"
#include "../models/Datetime.h"
#include "../models/Cloud.h"
#include "../models/CatInfo.h"

#define SKY_SPLIT 0.40


int drawGame(Game *game);
/**
 * @brief Draws the game's menu
 * 
 * @param game current game
 * @return 0 upon success, 1 otherwise 
 */
int drawMenu(Game *game);
/**
 * @brief Draws the game's instructions
 * 
 * @return 0 upon success, 1 otherwise 
 */
int drawInstructions();
/**
 * @brief Draws a button in chosen position with chosen text inside
 * 
 * @param text chosen text
 * @param x_center chosen x axis center position
 * @param y_center chosen y axis center position
 * @param width chosen width
 * @param height chosen height
 * @param color chosen color
 * @return 0 upon success, 1 otherwise 
 */
int drawButton(const char text[], int x_center, int y_center, int width, int height, uint8_t color);

// int draw_background();
/**
 * @brief Draws the game's sky
 * 
 * @return 0 upon success, 1 otherwise
 */
int drawSky();
/**
 * @brief Draws the game's grass
 * 
 * @return 0 upon success, 1 otherwise
 */
int drawGrass();

int drawScenery();
/**
 * @brief Draws all the clouds 
 * 
 * @param clouds array of clouds to be drawn
 * @return 0 upon success, 1 otherwise
 */
int drawClouds(Cloud clouds[]);
/**
 * @brief Draws a singular cloud
 * 
 * @param cloud 
 * @return 0 upon success, 1 otherwise 
 */
int drawCloud(Cloud *cloud);
/**
 * @brief Draw all elements needed for gameplay
 * 
 * @param current game 
 * @return 0 upon success, 1 otherwise 
 */
int drawGamePlaying(Game *game);
/**
 * @brief Draws a cat
 * 
 * @param entity 
 * @return 0 upon success, 1 otherwise 
 */
int drawCat(Entity *entity);
/**
 * @brief Draws the mouse
 * 
 * @param x x axis mouse position
 * @param y y axis mouse position
 * @return 0 upon success, 1 otherwise  
 */
int drawMouse(int x, int y);

int drawPetting(Game* game);

#endif
