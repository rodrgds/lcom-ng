#ifndef POSITION_H
#define POSITION_H

#include <lcom/lcf.h>


struct position
{
  uint16_t x, y;
};

typedef struct position position_t;

/**
 * @brief Creates a new position object in memory
 * 
 * @param x x coordinates of the position object
 * @param y y coordinates of the position object
 * 
 * @return pointer to the newly created object
 */
position_t* position_new(uint16_t x, uint16_t y);



/**
 * @brief Creates a new position object in memory @ random location
 * 
 * @return pointer to the newly created object
 */
position_t* position_new_randompos(uint16_t board_xsize, uint16_t board_ysize); 

/**
 * @brief Creates a new position object in memory
 * 
 * @param position the position of the new position object
 * 
 * @return pointer to the newly created object
 */
position_t* position_new_from_position(position_t* position);

/**
 * @brief Deletes a position object from memory
 * 
 * @param pos pointer to object that is to be deleted
 */
void  position_delete(position_t* pos);


/**
 * @brief Gets the position one unit to the right
 * 
 * @param pos initial position
 * 
 * @return new position to the right
 */
position_t* position_get_right(position_t* pos);

/**
 * @brief Gets the position one unit to the left
 * 
 * @param pos initial position
 * 
 * @return new position to the left
 */
position_t* position_get_left (position_t* pos);

/**
 * @brief Gets the position one unit up
 * 
 * @param pos initial position
 * 
 * @return new position up
 */
position_t* position_get_up   (position_t* pos);

/**
 * @brief Gets the position one unit down
 * 
 * @param pos initial position
 * 
 * @return new position down
 */
position_t* position_get_down (position_t* pos);

/**
 * @brief Gets the x coordinate of a position object
 * 
 * @param pos the position to get x from
 * 
 * @return x coordinate of the position object
 */
uint16_t position_get_x(position_t* pos);

/**
 * @brief Gets the y coordinate of a position object
 * 
 * @param pos the position to get y from
 * 
 * @return y coordinate of the position object
 */
uint16_t position_get_y(position_t* pos);

#endif
