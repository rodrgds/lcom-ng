//
// Created by skidr on 29/10/2019.
//

#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"
#include <stdint.h>
#include <stdio.h>

#include "timer.h"

/**
 * @file mouse.h
 * @brief Project Module with functions to manage the mouse
 */

/** @defgroup mouse mouse
 * @{
 * Functions for mouse management
 */

/**
 * @brief Function to subscribe mouse interruptions
 * @return 0 upon success
 */
int mouse_subscribe_int(uint32_t *bit_no);

/**
 * @brief Function to unsubscribe mouse interruptions
 * @return 0 upon success
 */
int mouse_unsubscribe_int(void);

/**
 * @brief Function to handle mouse interruptions
 * Updates the structs related to mouse
 */
void (mouse_ih)(void);

/**
 * @brief Processes the mouse packet
 */
void process_packet(void);

/**
 * @brief Processes the mouse in remote mode 
 */
void process_remote(void);

/**
 * @brief Function to issue a command to the mouse
 */
int mouse_issue_cmd(uint32_t command);

typedef enum {I1, D1, L1, I2, D2, L2, C} state_t;
typedef enum {MOV1, A1, B, MOV2, A2, RIGHT_B, LEFT_LB, RB_UP, LB_UP} event_t;
void check_line(event_t *evt);


#endif //LAB4_MOUSE_H
