#ifndef WORDS_H
#define WORDS_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>


extern char resulting_string[20];
extern bool complete_string;
char* words[3];

/**
 * @brief Get a random word from the word bank
 * 
 * @return char*    Returns a random word
 */
char* get_random_word();

/**
 * @brief Displays a given string on the screen on the coordinates that are
 *        passed as arguments
 * 
 * @param string    String that will be displayed
 * @param x_pos     Coordinate on the X axis
 * @param y_pos     Coordinate on the Y axis
 */
void draw_string(const char* string, uint16_t x_pos, uint16_t y_pos);

/**
 * @brief Draws a characther sprite on the screen each time the funtion is called, only stops
 *        writing if either the string has reached its maximum length ( in which case it will wait 
 *        for the player to pass the newline character as a parameter, thus completing the string,  
 *        or for the backspace characther, so the player can resume writing ) or the newline 
 *        characther has been passed as an argument. There is one special case for the backspace 
 *        characther, when it is passed as an argument the last characther before the backspace is  
 *        erased from the screen and the string.
 * 
 * @param new_char            Characther that will modify the string
 * @param x_pos               Coordinate on the X axis
 * @param y_pos               Coordinate on the Y axis
 * @param background_color    Color of the background where the string will be drawn
 */
void write_string(char new_char, uint16_t x_pos, uint16_t y_pos, int background_color);

/**
 * @brief Resets all of the variables used in write_string function so that it can be used again
 *        without any trash from when it was last used
 */
void clear_string();

/**
 * @brief Gets three different words from the word bank 
 */
void get_words();

/**
 * @brief Sets the chosen_word variable as the string that is passed as an argument
 * 
 * @param string    String that will be set as the chosen_word
 */
void set_word(char* string);

#endif
