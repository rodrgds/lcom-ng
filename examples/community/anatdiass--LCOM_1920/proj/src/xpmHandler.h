#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Load all the aphabet, numbers and symbols xpms
 * 
 */
void createXPMs();

/**
 * @brief Print xpm of a character
 * 
 * @param character Character to be printed 
 * @param x_pos X coordinate of position where character is printed
 * @param y_pos Y coordinate of position where character is printed
 * @return int Width of drawn xpm
 */
int printCharacter(char character, int x_pos, int y_pos);

/**
 * @brief Print the xpms of all the characters of a word
 * 
 * @param word Word to be printed
 * @param x_pos X coordinate of position where word is printed
 * @param y_pos Y coordinate of position where word is printed
 */
void printWord(char word[], int x_pos, int y_pos);

/**
 * @brief Convert global_counter to minutes and seconds and print the final counter
 * 
 */
void printCounter();

/**
 * @brief Gets current date and time and print them
 * 
 */
void printDate();
