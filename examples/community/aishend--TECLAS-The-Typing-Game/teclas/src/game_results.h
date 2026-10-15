/**
 * @file game_results.h
 * @brief Game results structure for a completed typing session.
 */
#ifndef _TECLAS_GAME_RESULTS_H_
#define _TECLAS_GAME_RESULTS_H_

/**
 * @brief Statistics collected at the end of a typing game.
 */
typedef struct {
    int correct_words;
    int wrong_words;
    int avg_errors_x10;
    int wpm;
    int time_played;
    int accuracy;
    int raw_consistency;
    int correct_chars;
    int wrong_chars;
} GameResults;

#endif // _TECLAS_GAME_RESULTS_H_
