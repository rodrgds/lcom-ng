#ifndef __HIGHSCORE_H
#define __HIGHSCORE_H

/** @file highscore.h
 * @brief file containing the definitions of the functions in highscore.c
 */

/** @defgroup highscore highscore
 * @{
 *
 * @brief functions for handling game highscores
 */
#include "rtc.h"

/**@{
 * @brief High Score info
 */
typedef struct {
	unsigned short name[5];				/**< @brief name of the player that obtained the high score */
	DateTime *datetime;					/**< @brief date and time of the high score */
	unsigned int score;					/**< @brief actual score of the high score */
} HighScore;
/** @}*/

/**
 * @brief create a new highscore sctruct
 * @return the highscore created
 */
HighScore *create_highscore();

/**
 * @brief destroy a previously created highscore
 * @param highscore the highscore to destroy
 */
void destroy_highscore(HighScore *highscore);

/**
 * @brief load the game highscores from a text file
 * @param highscores array of the highscores where the file info will be loaded
 */
void load_highscores(HighScore *highscores[10]);

/**
 * @brief save the game highscores to a text file
 * @param highscores array of the highscores that will be saved
 */
void save_highscores(HighScore *highscores[10]);

/**
 * @brief update the game highscores using the current game score
 * @param highscores the array of current highscores
 * @param new_score the new score from this game
 * @param new_name the new player name with the new score
 * @param new_datetime the date and time of the new score
 */
void update_highscores(HighScore *highscores[10], unsigned int new_score, unsigned short new_name[5], DateTime *new_datetime);

/** @} end of highscore */
#endif /* __HIGHSCORE_H */
