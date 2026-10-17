#ifndef _CODE_TO_CHARACTER_H
#define _CODE_TO_CHARACTER_H

#include <lcom/lcf.h>


/** @defgroup codeToCharacter
 * @{
 *
 *
 *
 * Functions developed to ease the conversion between a code from the keyboard and its
 * ASCII representation
 */

/* Data structure to represent a character-makecode pair*/
typedef struct {
  unsigned char break_code;     /**< @brief Character break code */
  char character;               /**< @brief Character ASCII representation */
} CodeToCharacter;


/**
 * @brief Given a character and a code, creates a new CodeToCharacter object
 * @param c character
 * @param code makecode of the character
 * @return CodeToCharacter object
 */
CodeToCharacter *new_pair(char c, uint8_t code);

/**
 * @brief For each character in the alphabet, create a new CodeToCharacter object
 */
void (fill_code_list)();

/**
 * @brief Given a character, returns the corresponding make code
 * @param c character to be assessed
 * @return c's makecode
 */
unsigned char (get_code_from_character)(char c);

/**
 * @brief Given a make code, returns the corresponding character,
 * @param code make code to be assessed
 * @return makecode correspondent character
 */
char (get_character_from_code)(unsigned char code);

/** @}*/

#endif /* _CODE_TO_CHARACTER_H */
