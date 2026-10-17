#ifndef _WORD_PICKER_H_
#define _WORD_PICKER_H_

/**
 * @ingroup Game
 * @{
 * 
 * @defgroup word_picker Word Picker
 * 
 * @{
*/

#define MAX_WORD_LENGTH 26 /**< @brief Maximum length of a word*/

/**
 * @brief Chooses a random word and returns it
 * 
 * This function is responsible for opening the file word_list.txt, choosing a random word from it and returning the word.
 * 
 * @return Chosen word
*/
char* get_word();

/**
 *  }@
 * }@
*/

#endif
