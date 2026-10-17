#pragma once
#include <string.h>

/** @defgroup edit_text edit_text
 * @{
 * Functions for manipulating edit_text objects
 */

/**
 * @brief      Represents one EditText object.
 */
struct EditText {
  int x;				/**< x coordinate on screen */
  int y; 				/**< y coordinate on screen */
  char text[50]; 		/**< text appeaing on editText */
  int t_size; 			/**< size of text appeaing on editText */
  bool mouseOver; 		/**< flag marking if mouse is over the editText */
  bool focused;			/**< flag marking if editText is focused and ready for input from user */
  bool under_draw;		/**< flag marking if editText cursor is draw */
  int under_count;		/**< counter denoting the time of cursor transition */
};

/**
 * @brief      Converts a keyboard makecode to an ASCII leter
 *
 * @param      c     { char pointer to write the corresponding ASCII leter }
 * @param[in]  d     { keyboard makecode }
 *
 * @return     { flag denoting if an ASCII leter corresponding to the keyboard makecode was found }
 */
bool convert_to_char(char* c, uint8_t d);

/**
 * @brief      Initialize edittext structure.
 */
void init_edittext();

/**
 * @brief      Destroy edittext structure and free memory used by it
 */
void destroy_edittext();

/**
 * @brief      Creates an edittext with specified atributes.
 *
 * @param[in]  x        { x coordinate }
 * @param[in]  y        { y coordinate }
 * @param[in]  focused  { flag marking if editText is focused }
 *
 * @return     { editText object with specified atributes }
 */
struct EditText create_edittext(int x, int y, bool focused);

/**
 * @brief      Transmit data from keybpard to editText
 *
 * @param      edt   { The ediText object to receive data }
 * @param[in]  d     { makecode from keyboard }
 */
void receive_input(struct EditText* edt, uint8_t d);

/**
 * @brief      Update specified editText object
 *
 * @param      edt   { The ediText object to update }
 */
void update_edittext(struct EditText* edt);

/**
 * @brief      Draws specified editText to screen
 *
 * @param[in]  edt   The editText object to draw
 */
void draw_edittext(struct EditText edt);
