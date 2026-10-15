#ifndef __SAVE_SCORE_H
#define __SAVE_SCORE_H

#include <stdio.h>
#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include <string.h>

/**
 * @brief Struct representing a scoreboard entry.
 * 
 * Stores information about the player's name, score, and the date and time when the score was achieved.
 */
typedef struct {
    char name[10];
    int score;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
}scoreboard_entry;

extern struct real_time_clock_info rtc_info;

/**
 * @brief Saves the player's name and score to the scoreboard file.
 * 
 * @param name The player's name.
 * @param score The player's score.
 */
void save_name_score(char *name,int score);


/**
 * @brief Reads the scores and associated information from the scoreboard file.
 * 
 * @param names Array to store the names of the players.
 * @param scores Array to store the scores of the players.
 * @param dates Array to store the dates when the scores were achieved.
 * @param size The size of the arrays.
 */
void read_scores(char names[][10], int scores[], char dates[][18], int size);

#endif

