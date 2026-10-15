/**
 * @file words.h
 * @brief Typing session layout and input handling.
 */
#ifndef TECLAS_WORDS_H
#define TECLAS_WORDS_H

#include "font.h"
#include "word.h"
#include <stdint.h>

/** @brief Total number of word slots in the typing pool. */
#define TYPING_POOL_SIZE 80

/** @brief Number of visible word rows displayed at once. */
#define TYPING_NUM_ROWS 3

/**
 * @brief State of an active typing session.
 */
typedef struct {
    Word pool[TYPING_POOL_SIZE];
    int current;
    int first_visible;
    int row_end[TYPING_NUM_ROWS];
    uint16_t word_x[TYPING_POOL_SIZE];
    int acc_correct_chars;
    int acc_wrong_chars;
    int acc_correct_words;
    int acc_wrong_words;
} TypingSession;

/**
 * @brief Compute the layout (x positions and row breaks) for all words.
 *
 * @param s       Pointer to the typing session.
 * @param x_start Left boundary in pixels.
 * @param x_end   Right boundary in pixels.
 */
void typing_layout(TypingSession *s, int x_start, int x_end);

/**
 * @brief Handle a character being typed.
 *
 * @param s Pointer to the typing session.
 * @param c ASCII character typed by the user.
 */
void typing_type(TypingSession *s, char c);

/**
 * @brief Handle a backspace (erase the last typed character).
 *
 * @param s Pointer to the typing session.
 */
void typing_erase(TypingSession *s);

/**
 * @brief Advance to the next word and scroll the view if needed.
 *
 * @param s       Pointer to the typing session.
 * @param x_start Left boundary in pixels.
 * @param x_end   Right boundary in pixels.
 */
void typing_advance(TypingSession *s, int x_start, int x_end);

#endif
