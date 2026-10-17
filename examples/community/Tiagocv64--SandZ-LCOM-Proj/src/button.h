#pragma once
#include <lcom/lcf.h>
#include <string.h>

/** @defgroup button button
 * @{
 * Functions for manipulating button objects
 */

/**
 * @brief      Represents one button object.
 */
struct Button {
  int x;				/**< x coordinate on screen */
  int y; 				/**< y coordinate on screen */
  char text[50]; 		/**< text appeaing on button */
  int t_size; 			/**< size of text appeaing on button */
  bool mouseOver; 		/**< flag marking if mouse is over the button */
  void (*func)(void); 	/**< function called on mouse's press over button */
};

/**
 * @brief      Initialize button structure.
 */
void init_button();

/**
 * @brief      Destroy button structure and free memory used by it
 */
void destroy_button();

/**
 * @brief      Creates and returns a button with specified atributes
 *
 * @param[in]  x       { x coordinate }
 * @param[in]  y       { y coordinate }
 * @param      text    { text appeaing on button }
 * @param[in]  t_size  { size of text appeaing on button }  
 * @param[in]  func    { function called on mouse's press over button }
 *
 * @return     { button object with specified atributes }
 */
struct Button create_button(int x, int y, char* text, int t_size, void (*func)(void));

/**
 * @brief      Update specified button object
 *
 * @param      but   The button object to update
 */
void update_button(struct Button* but);

/**
 * @brief      Draws specified button to screen
 *
 * @param[in]  but   The button object to draw
 */
void draw_button(struct Button but);
