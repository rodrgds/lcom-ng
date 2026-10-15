/**
 * @file word.h
 * @brief Word and per-letter typing state types.
 */
#ifndef _TECLAS_WORD_H_
#define _TECLAS_WORD_H_

#include <stdint.h>
#include <string.h>

/** @brief Maximum number of characters a word can have. */
#define MAX_WORD_LEN 32

/**
 * @brief Per-letter typing state.
 */
typedef enum {
    UNTYPED,
    CORRECT,
    WRONG
} LetterState;

/**
 * @brief A word entry in the typing session.
 */
typedef struct {
    const char *word;
    int word_len;
    LetterState states[MAX_WORD_LEN];
    int typed_len;
} Word;

#endif
