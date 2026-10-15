#ifndef WORDS_H
#define WORDS_H

#include <lcom/lcf.h>
#include <stdbool.h> 

extern const int guesswords_count;
extern const char *guess_word;
extern bool guessed_letters[26];  
extern char displayed_word[];

#define MAX_WORD_LENGTH 20 //aux for tha max length we allow in a word

//associates scancodes with letters
typedef struct {
    uint8_t scancode;
    char letter;
} KeyMap;

//associates words with categories
typedef struct {
    const char *word;
    const char *category;
} GuessWord;

extern const GuessWord guesswords[];

void generate_guessword(const char *category);

char scancode_to_letter(uint8_t scancode);

#endif

