#ifndef SNAKE_MODEL_H
#define SNAKE_MODEL_H

#include <stdio.h>
#include <stdlib.h>

#include "position.h"

enum direction {NORTH, SOUTH, EAST, WEST};
typedef enum direction direction_t;

struct body_piece;
typedef struct body_piece body_piece_t;

struct snake {
    uint16_t size;
    body_piece_t* body_start; /* Snake Tail */
    body_piece_t* body_end; /* Snake Head */
    direction_t head_direction;
};

typedef struct snake snake_t;

/**
 * @brief Creates a snake object. Allocates memory internally
 * 
 * @param position the position of the snake
 * 
 * @return pointer to the created snake upon success, NULL otherwise 
 */
snake_t* snake_new(position_t* position);

/**
 * @brief Deletes a snake object from memory
 * 
 * @param snake the snake object that is to be deleted
 */
void snake_delete(snake_t* snake);

/**
 * @brief Increases the size of a snake
 * 
 * @param snake snake whose size is to be increased
 * @param position position where the snake will increase to
 */
void snake_increase_size(snake_t* snake, position_t* position);

// /**
//  * @brief Decreases the size of a snake
//  * 
//  * @param snake snake whose size is to be decreased
//  * @return 1 if snake has reached it's minimum size, 0 uppon success
//  */
// int snake_decrease_size(snake_t* snake);


/**
 * @brief Sets a new position for a snake
 * 
 * IMPORTANT: This is only meant to move the snake one
 * 'pixel' at a time and is not garanteed to work otherwise.
 * 
 * @param snake snake object whose position is to be set
 * @param position new position
 */
void snake_set_position(snake_t* snake, position_t* position);


/**
 * @brief Gets the position of a snake
 * 
 * @param snake snake object to get position from
 * 
 * @return position of the snake object
 */
position_t* snake_get_head_position(snake_t* snake);

/**
 * @brief Returns the next snake's next part of the body.
 * 
 * @param body_piece Points to the previous part.
 * @return body_piece_t* Points to the next part of the body.
 */
body_piece_t* snake_get_next_body_piece(body_piece_t* body_piece);

/**
 * @brief Returns the specified body piece's position.
 * 
 * @param body_piece Body piece that will retrieve its position.
 * @return position_t* Points to the body piece's position.
 */
position_t* snake_get_body_piece_position(body_piece_t* body_piece);

#endif
