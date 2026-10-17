/**

@file proj.h
@brief Header file containing declarations of functions and variables used for controlling every aspect of the game.

*/
#ifndef PROJ_H
#define PROJ_H

#include <lcom/lcf.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "stdbool.h"

#include "Drivers/GraphicCard.h"
#include "Drivers/keyboard.h"
#include "Drivers/timer.h"
#include "Drivers/mouse.h"
#include "Drivers/RTC.h"
#include "Drivers/macros.h"
#include "Sprites/xpms.h"
#include "Lists/LaserList.h"
#include "Lists/EnemyList.h"
#include "Lists/AnimationList.h"
#include "Levels.h"
#include "Handlers.h"
#include "Missile.h"

//set all the timer and keyboard variables
extern uint8_t irq_set_kbc;
extern uint8_t irq_set_timer;
extern uint8_t irq_set_mouse;

//colors
extern uint32_t color_enemy;
extern uint32_t color_laser;
extern uint32_t color_background;

//user related variables
extern uint16_t x;
extern uint16_t y;
extern uint16_t lives;
extern uint16_t score;

//mouse related variables
extern int mouse_x;
extern int mouse_y;
extern int mouse_counter;
extern struct packet mousePacket;
extern uint8_t mouseBytes[3];

//enemy related variables
extern struct EnemyNode *el_t; //el = enemy list
extern struct EnemyNode *EnemyFirst; //first node of the list
extern struct EnemyNode *EnemyAdded; //last node of the list
extern int EnemySize;
extern int current_enemy_sprite;

//laser related variables
extern struct LaserNode *ll_t; //ll = laser list
extern struct LaserNode *LaserFirst; //first node of the list
extern struct LaserNode *LaserAdded; //last node of the list
extern int current_ebullet_sprite;

//Animation related variables
extern struct AnimationNode *al_t; //al = animation list
extern struct AnimationNode *AnimationFirst; //first node of the list
extern struct AnimationNode *AnimationAdded; //last node of the list
extern int AnimationSize;

//driver related variables
extern uint8_t scancode;
extern vbe_mode_info_t mode_inf;

extern int closeProj;

//rtc related variables
extern uint8_t rtc_mode;
extern clock_fields_struct info_rtc;

/**
 * @brief sets color related variables to their correct values
 * 
 */
void normalize_all();
/**
 * @brief interrupt loop used in the gameplay
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int interrupt_loop();
/**
 * @brief sets default values to all user related variables
 * 
 */
void player_defaults();

#endif //PROJ_H
