#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "keyboard.h"
#include "videocard.h"
#include "mouse.h"


#define N 50 /// Snake's matrix size horizontally
#define M 40 /// Snake's matrix size vertically
#define SPEED 25

enum direction { RIGHT, UP , LEFT , DOWN , EMPTY}; /// Enum representing snake's possible directions

struct Tile{
    bool Head;
    bool Body;
    bool Tail;
    enum direction dir;
};

struct Snake{
    short x;
    short y;
    unsigned int size;
    enum direction snake_dir;
};
/**
* @brief Initializes the snake
*
* Initializes the snake by passing correct values to the structs
* @return Returns nothing
*/
void snake_init();
/**
* @brief Prints snake
*
* Draws snake to the buffer by drawing passed xpms
*
* @param head_up_adr - pointer to starting addresss of head up xpm
* @param head_up_img - image of head up of type xpm_image_t
* @param head_down_adr - pointer to starting addresss of head down xpm
* @param head_down_img - image of head down of type xpm_image_t
* @param head_left_adr - pointer to starting addresss of head left xpm
* @param head_left_img - image of head left of type xpm_image_t
* @param head_right_adr - pointer to starting addresss of head right xpm
* @param head_right_img - image of head right of type xpm_image_t
* @param body_h_adr - pointer to starting addresss of body in horizontal xpm
* @param body_h_img - image of body in horizontal of type xpm_image_t
* @param body_v_adr - pointer to starting addresss of body in vertical xpm
* @param body_v_img - image of body in vertical of type xpm_image_t
* @param tail_right_adr - pointer to starting addresss of tail to the right xpm
* @param tail_right_img - image of tail to the right of type xpm_image_t
* @param tail_left_adr - pointer to starting addresss of tail to the left xpm
* @param tail_left_img - image of tail to the left of type xpm_image_t
* @param tail_down_adr - pointer to starting addresss of tail downwards xpm
* @param tail_down_img - image of tail downwards of type xpm_image_t
* @param tail_up_adr - pointer to starting addresss of tail upwards xpm
* @param tail_up_img - image of tail upwards of type xpm_image_t
* @return Returns nothing
*/
void printSnake(uint8_t * head_up_adr, xpm_image_t head_up_img,
    uint8_t * head_down_adr, xpm_image_t head_down_img, uint8_t * head_left_adr, 
    xpm_image_t head_left_img, uint8_t * head_right_adr, xpm_image_t head_right_img,
    uint8_t* body_h_adr, xpm_image_t body_h_img,uint8_t* body_v_adr, xpm_image_t body_v_img, 
    uint8_t* tail_right_adr, xpm_image_t tail_right_img, uint8_t* tail_left_adr, 
    xpm_image_t tail_left_img,uint8_t* tail_down_adr, xpm_image_t tail_down_img,uint8_t* tail_up_adr, 
    xpm_image_t tail_up_img );
/**
* @brief Handles snake movement
*
* Updates matrix and verifies if snake collides with itself
*
* @param snake_dir - enum type direction representing one of possible snake's directions
* @return Returns 1 on colision and 0 if no problem found
*/
int movement(enum direction snake_dir);
/**
* @brief Updates tail
*
* Updates snake's tail to be in the right direction and following head properly
* @return Returns nothing
*/
void UpdateTail();
/**
* @brief Gets new Snake
*
* Gets a new Snake of struct Snake
* @return Returns object of struct Snake
*/
struct Snake getNewSnake();
