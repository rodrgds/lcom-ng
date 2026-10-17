#pragma once

#include <stdio.h>
#include <math.h>
#include "videocard.h"
#include "rtc.h"
#include "images.h"

/**
 * @file fruit.h
 * @brief Fruit and FruitBasket object definition and manipulation.
 * 
 * File containing the definitions of Fruit and FruitBasket structs, as well
 * as the most relevant functions for working with them.
*/

/**
 * @brief Acceleration for Fruit objects in pixels/second/second.
 */
#define gravity 200.0

/**
 * @brief Sprite-like struct used for displaying fruits and other elements
 * 
 * Contains the images, positional and contextual information useful for drawing
 * objects on the screen. Used for buttons, fruits and the mouse pointer.
 * Contains two images to allow for easy switching between pressed down/released mouse/button states
 * and whole/cut fruit states. The boolean members are used to decide which image to display and
 * whether the object's position should be updated or not. 
 */
typedef struct {
  xpm_image_t whole; /**< Image for the "whole" state of the object. */
  xpm_image_t chopped; /**< Image for the "cut" state of the object. */

  double x_pos; /**< Horizontal postion of the top-left corner of the object. */
  double y_pos; /**< Vertical postion of the top-left corner of the object */

  double x_speed; /**< Horizontal speed of the object in pixels/second */
  double y_speed; /**< Vertical speed of the object in pixels/second */

  bool is_moving; /**< Determines whether to update the object position according to speed and gravity or not. */
  bool is_chopped; /**< Determines which image to draw the object as. */
  bool has_collided; /**< Stores if the fruit has collided against another one in the past */
  bool is_bomb; /**< Distinguishes bombs from fruits. */
} Fruit;


/**
 * @brief Fruit constructor.
 * 
 * Constructs a new Fruit object with the specified images and properties.
 * 
 * @param xpm_whole the image for the "whole" state of the Fruit.
 * @param xpm_chopped the image for the "cut" state of the Fruit.
 * @param x_init the initial horizontal position of the top-left corner of the Fruit.
 * @param y_init the initial vertical position of the top-left corner of the Fruit.
 * @param x_speed the initial horizontal speed of the Fruit.
 * @param y_speed the initial vertical speed of the Fruit.
 * @param is_moving the moving state of the Fruit.
 * @param is_chopped the "whole/cut" state of the Fruit.
 * @return A pointer to the newly created Fruit.
 */
Fruit* createFruit(xpm_image_t xpm_whole, xpm_image_t xpm_chopped, double x_init, double y_init, double x_speed, double y_speed, bool is_moving, bool is_chopped);


/**
 * @brief Fruit constructor for bombs.
 * 
 * Constructs a new Fruit object with the specified images and properties that is a
 * bomb object.
 * 
 * @param xpm_whole the image for the "whole" state of the Fruit.
 * @param xpm_chopped the image for the "cut" state of the Fruit.
 * @param x_init the initial horizontal position of the top-left corner of the Fruit.
 * @param y_init the initial vertical position of the top-left corner of the Fruit.
 * @param x_speed the initial horizontal speed of the Fruit.
 * @param y_speed the initial vertical speed of the Fruit.
 * @param is_moving the moving state of the Fruit.
 * @param is_chopped the "whole/cut" state of the Fruit.
 * @return A pointer to the newly created Fruit/bomb.
 */
Fruit* createBomb(xpm_image_t xpm_whole, xpm_image_t xpm_chopped, double x_init, double y_init, double x_speed, double y_speed, bool is_moving, bool is_chopped);


/**
 * @brief Fruit destructor.
 * 
 * Destroys a Fruit object, freeing the space previously allocated to it.
 * 
 * @param fruit a pointer to the Fruit to destroy.
 */
void destroyFruit(Fruit* fruit);


/**
 * @brief Displays a Fruit.
 * 
 * Draws a Fruit object on screen, choosing the image to be displayed according to
 * the value of is_chopped.
 * 
 * @param fruit a pointer to the Fruit to display.
 */
void draw_fruit(Fruit* fruit);


/**
 * @brief Updates the positon of a Fruit and displays it.
 * 
 * Changes the position of a Fruit object, if is_moving is set to true, according to it's speed 
 * and the defined gravity. Then it calls draw_fruit to draw the Fruit in it's position.
 * 
 * @param fruit a pointer to the Fruit to update and display.
 */
void move_fruit(Fruit* fruit);


/**
 * @brief Verifies if a Fruit is out of bounds.
 * 
 * Checks the position of a Fruit object against the screen resolution to determine
 * when it has moved outside of the visible area. Takes into account the direction
 * of the Fruit's movement to decide what position makes it completely invisible.
 * 
 * @param fruit a pointer to the Fruit to verify.
 * @return True if the fruit is not visible, false otherwise.
 */
bool fruit_out_of_screen(Fruit* fruit);


/**
 * @brief Launches a Fruit from the bottom of the screen.
 * 
 * Creates a new Fruit object that starts moving upwards from a random position on
 * the bottom of the screen, with the specified images.
 * 
 * @param whole the image for the "whole" state of the Fruit.
 * @param whole the image for the "cut" state of the Fruit.
 * @param bomb whether the new Fruit should be a bomb or not.
 * @return A pointer to the newly created Fruit.
 */
Fruit* launch_fruit(xpm_image_t whole, xpm_image_t chopped, bool bomb);


/**
 * @brief Cuts a Fruit.
 * 
 * Changes the value of is_chopped to false and in case the Fruit is moving
 * upwards, makes it so it starts accelerating downwards. 
 * 
 * @param fruit a pointer to the Fruit to cut.
 */
void cut_fruit(Fruit* fruit);


/**
 * @brief Checks if a line_t intersects a Fruit.
 * 
 * Checks if a line_t object intersects a Fruit object. Used to see if a user
 * mouse gesture results in a Fruit being cut. Takes into account if the line
 * is intersecting a non-transparent pixel of the image.
 * 
 * @param fruit a pointer to the Fruit to check against the line_t.
 * @param line a pointer to the line_t to check agains the Fruit.
 * @return True if the Fruit and line_t intersect, false otherwise.
 */
bool intersect_line(Fruit* fruit, line_t* line);


/**
 * @brief Updates mouse-pointer position.
 * 
 * @param mouse_pointer a pointer to the Fruit corresponding to the mouse-pointer.
 * @param delta_x the horizontal displacement.
 * @param delta_y the vertical displacement.
 * @param lb the pressed/released state of the mouse.
 */
void move_mouse(Fruit* mouse_pointer, int delta_x, int delta_y, bool lb);


// *********** //
// FruitBasket
// *********** //

/**
 * @brief Container for objects of Fruit* type.
 * 
 * Stores Fruit* members in an array of fixed size. Has information about the
 * array size and stores the first empty postion in the array. Useful for supporting
 * creating and removing fruits during program execution. 
 */
typedef struct {
  Fruit** contents; /**< Array of Fruit* types. */

  size_t capacity; /**< Capacity of the Fruit* array. */
  
  int next; /**< First empty postion in the array, where a new Fruit* should be inserted if need be. If the array is full it is set to -1. */
} FruitBasket;


/**
 * @brief FruitBasket constructor.
 * 
 * Constructs a new FruitBasket with specified capacity.
 * 
 * @param capacity of the new FruitBasket
 * @return the new FruitBasket struct.
 */
FruitBasket createFruitBasket(size_t capacity);


/**
 * @brief FruitBasket destructor.
 * 
 * Destroys a FruitBasket object, destroying the Fruit* it contained and the
 * contents array that contained them.
 * 
 * @param fruit_basket a pointer to the FruitBasket to be deleted.
 */
void destroyFruitBasket(FruitBasket* fruit_basket);


/**
 * @brief Clears a FruitBasket.
 * 
 * Destroys the Fruit* contained in the FruitBasket object specified. Sets all
 * aray postions to NULL and next as the first positon of the array.
 * 
 * @param fruit_basket a pointer to the FruitBasket to be cleared.
 */
void emptyFruitBasket(FruitBasket* fruit_basket);


/**
 * @brief Finds next empty postion in a FruitBasket.
 * 
 * Finds the next empty postion in the contents array of a specified FruitBasket
 * If all positions are full returns -1.
 * 
 * @param fruit_basket a pointer to the FruitBasket where to find the next position.
 * @return The postion of the first empty space in the array. If all are full, returns -1.
 */
int find_next(FruitBasket* fruit_basket);


/**
 * @brief Inserts a new Fruit in the FruitBasket specified.
 * 
 * Inserts a given Fruit* in the next postion of a specified FruitBasket, if its contents
 * array is not full.
 * 
 * @param fruit_basket a pointer to the FruitBasket where the Fruit* should be inserted.
 * @param new_fruit the Fruit* to be inserted in the FruitBasket.
 * @return True if the fruit was added successfuly to the FruitBasket, false otherwise.
 */
bool add_fruit(FruitBasket* fruit_basket, Fruit* new_fruit);


/**
 * @brief Removes a Fruit from the FruitBasket specified.
 * 
 * Removes a given Fruit* currently in the FruitBasket from its contents array at a given
 * position, if the Fruit* matches the one in the array.
 * 
 * @param fruit_basket a pointer to the FruitBasket from which to remove the Fruit*.
 * @param rotten_fruit the Fruit* to remove from the FruitBasker.
 * @param pos the position of the Fruit* to remove from the array.
 * @return True if the Fruit* is successfully removed from the FruitBasket, false otherwise.
 */
bool remove_fruit(FruitBasket* fruit_basket, Fruit* rotten_fruit, size_t pos);


/**
 * @brief Shots a specified number of random Fruits.
 * 
 * Creates a specified number of random Fruits and adds them to a FruitBasket.
 * The Fruits will start moving upward with random horizontal speed from a
 * random position in the bottom of the screen.
 * 
 * @param fruit_basket a pointer to the FruitBasket that will store the Fruits.
 * @param num the number of Fruits to shoot.
 */
void shoot_fruit(FruitBasket* fruit_basket, size_t num);

/**
 * @brief Cuts all Fruits contained in a FruitBasket that a line_t intersects.
 * 
 * Checks for intersections between a specified line_t object and the Fruit objects
 * inside the contents array of a specified FruitBasket. In case an intersection is
 * found, the corresponding Fruit object is cut. Returns the number of cut fruits.
 * 
 * @param fruit_basket a pointer to the FruitBasket to check against the line_t.
 * @param line a pointer to the line_t to check against the FruitBasket.
 * @return The number of Fruit objects the line_t intersected.
 */
size_t check_cut_fruit(FruitBasket* fruit_basket, line_t* line);

/**
 * @brief Sprite collision detection between Fruits.
 * 
 * Detects colliding Fruits in a FruitBasket and changes their movement accordingly.
 * Only colisions between whole Fruits that are falling are considered. Two Fruits
 * that have previously collided with other Fruits are not considered so as to avoid
 * weird jittering.
 * 
 * @param fruit_basket a pointer to the FruitBasket to analyse.
 */
void check_colliding_fruit(FruitBasket* fruit_basket);


/**
 * @brief Fire fruit from cannon poition.
 * 
 * @param fruit_basket the FruitBasket where the Fruit will be added.
 * @param cannon the Fruit cannon.
 */
void fire_fruit(FruitBasket* fruit_basket, Fruit* cannon);
