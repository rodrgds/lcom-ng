#pragma once

#include <lcom/lcf.h>

/**
 * @brief Initializes the wordlist file
 * 
 */
int (game_wordlist_init)(char *path);

/**
 * @brief Closes the wordlist file
 * 
 */
int (game_wordlist_cleanup)();

/**
 * @brief Searches the wordlist for a target word
 * 
 * @param word The word to look for
 * @return true if the word is in the list, false otherwise
 */
bool (game_wordlist_contains)(char *word);

/**
 * @brief Selects a random word from the wordlist
 * 
 */
char *(game_wordlist_select_random)();
