#include <lcom/lcf.h>
#include <stdint.h>
#include "game_structs.h"


/**
 * @brief Reads the highscores from a .txt file.
 * 
 * @param highscores - Array of Highscore objects
 * @return int 0 if successful
 */
int read_highscores(Highscore highscores[5]);

/**
 * @brief Writes the highscores in a .txt file.
 * 
 * @param highscores - Array of Highscore objects
 * @return int 0 if successful
 */
int write_highscores(Highscore highscores[5]);

