#ifndef KEY_PRESSED_H
#define KEY_PRESSED_H

#include <lcom/lcf.h>
#include "kbd.h"

/**
 * @file key_pressed.h
 * 
 * @brief KeyPressed management
 * 
 * This module provides functions to create, destroy, and manage the state of keys pressed in the game.
 * It allows checking if specific keys (UP, DOWN, LEFT, RIGHT, ESC) are pressed and updating the state based on keyboard input.
 */

struct key_pressed_imp;
typedef struct key_pressed_imp KeyPressed;

/**
 * @brief Creates a KeyPressed instance.
 * 
 * @return A pointer to the created KeyPressed instance, or NULL if memory allocation fails.
 */
KeyPressed *key_pressed_create();

/**
 * @brief Destroys a KeyPressed instance.
 * 
 * @param kp Pointer to the KeyPressed instance to be destroyed.
 */
void key_pressed_destroy(KeyPressed *kp);

/**
 * @brief Checks if the UP key is pressed.
 * 
 * @param kp Pointer to the KeyPressed instance.
 * @return true if the UP key is pressed, false otherwise.
 */
bool is_up_pressed(KeyPressed *kp);

/**
 * @brief Checks if the DOWN key is pressed.
 * 
 * @param kp Pointer to the KeyPressed instance.
 * @return true if the DOWN key is pressed, false otherwise.
 */
bool is_down_pressed(KeyPressed *kp);

/**
 * @brief Checks if the LEFT key is pressed.
 * 
 * @param kp Pointer to the KeyPressed instance.
 * @return true if the LEFT key is pressed, false otherwise.
 */
bool is_left_pressed(KeyPressed *kp);

/**
 * @brief Checks if the RIGHT key is pressed.
 * 
 * @param kp Pointer to the KeyPressed instance.
 * @return true if the RIGHT key is pressed, false otherwise.
 */
bool is_right_pressed(KeyPressed *kp);

/**
 * @brief Checks if the ESC key is pressed.
 * 
 * @param kp Pointer to the KeyPressed instance.
 * @return true if the ESC key is pressed, false otherwise.
 */
bool is_esc_pressed(KeyPressed *kp);

/**
 * @brief Updates the pressed key based on the keyboard code.
 * 
 * @param kp Pointer to the KeyPressed instance to be updated.
 * @param keyboard_code The keyboard code representing the key event.
 */
void key_pressed_update(KeyPressed *kp, uint8_t keyboard_code);

#endif // KEY_PRESSED_H



