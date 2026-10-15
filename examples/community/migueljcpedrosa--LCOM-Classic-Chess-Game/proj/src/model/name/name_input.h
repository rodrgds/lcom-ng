/**
 * @file name_input
 * @brief This file has the implementation of the functions related to the player name input.
 */

#ifndef _NAME_H_
#define _NAME_H_

#include "../../view/sprite.h"

/**
 * @brief Struct representing the NameInput state.
 *
 * This struct includes the name string, a limit for the name length, sprite, and x and y coordinates for positioning.
 */
typedef struct {

  int x;
  int y;
  char* name;
  unsigned int limit;
  Sprite* sprite;

} NameInput;

extern NameInput* nameInput;
extern char* nameBackup;

/**
 * @brief Creates the player name menu.
 *
 * This function initializes the menu with a defined character limit, sprite, and positioning coordinates. 
 * The name is initially set as an empty string.
 * 
 * @return void.
 */
void (create_name)();

/**
 * @brief Destroys the player name menu and backs up the current name.
 *
 * This function creates a backup of the current name and then frees the memory allocated for the name string. 
 * The backup is stored in a global variable.
 * 
 * @return void.
 */
void (destroy_name)();

/**
 * @brief Adds a letter to the current name.
 * 
 * @param letter The character to be added to the name.
 *
 * This function adds the given character to the end of the current name if it doesn't exceed the character limit. 
 * It also updates the null terminator at the end of the string.
 * 
 * @return void.
 */
void (add_letter)(char letter);

/**
 * @brief Deletes the last letter from the current name.
 *
 * This function removes the last character from the current name and updates the null terminator at the end of the string.
 * 
 * @return void.
 */
void (delete_letter)();

#endif
