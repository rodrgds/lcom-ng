/**
 * @file canvas.h
 *
 */

#ifndef _LCOM_CANVAS_H_
#define _LCOM_CANVAS_H_

/* DEVICES */
#include "../../devices/KBC/mouse.h"
#include "../../devices/RTC/rtc.h"
#include "../../devices/video_card/video.h"

/* MODELS */
#include "../models/list.h"
#include "../models/object.h"

/* STATES */
#include "../states/gamestate.h"

/* VIEWS */
#include "image.h"

#include "../../images/P1Win.xpm"
#include "../../images/P2Win.xpm"

uint32_t walls_color;
uint32_t arena_color;
uint32_t final_color;

/**
 * @brief unitialies global variables to values given to the function
 * 
 */
void (__canvas__)(uint32_t new_arena_color, uint32_t new_walls_color, uint32_t new_final_color);

/**
 * @brief draws the arena
 * 
 */
int (canvas_draw_arena)();
/**
 * @brief draws the final screen telling who the winner is
 * 
 */
int (canvas_draw_final)(GameState* state);
/**
 * @brief draws given object
 * 
 * @param obj object to be drawn
 * 
 */
int (canvas_draw_object)(Object* obj);
/**
 * @brief draws the crosshair
 * 
 */
int (canvas_draw_crosshair)(Object* crosshair);
/**
 * @brief draws all the walls
 * 
 * @param list list of walls to be drawn
 * 
 */
int (canvas_draw_walls)(List* list);
/**
 * @brief draws the bullets
 * 
 * @param list list of bullets to be drawn
 * 
 */
int (canvas_draw_bullets)(List* list);
/**
 * @brief draws all the components of the game
 * 
 * @param game gamestate
 * 
 */
int (canvas_refresh_game)(GameState* game);
/**
 * @brief starting at a given hour changes the colors of the game
 * 
 * @param color reference to the color to be changed
 * 
 */

int (nightTransform)(uint32_t* color);

#endif // _LCOM_CANVAS_H_
