#ifndef __FILE_H
#define __FILE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @defgroup file file
 * @{
 * 
 * Functions for reading and updating the highscore of the game 
 */

/**
 * @brief Reads the file that contains that highest score of all time
 * 
 * @param highscore The highest score someone has gotten playing the game
 * @return Return 0 upon success and non-zero otherwise
 */
int (read_highscore)(int *highscore);

/**
 * @brief Writes to the file a new high score
 * 
 * @param highscore The highest score someone has gotten playing the game
 * @return Return 0 upon success and non-zero otherwise 
 */
int (write_highscore)(int highscore);

/**
 * @}
 */

#endif /* __FILE_H */
