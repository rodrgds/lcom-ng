/**
	@file files.h
	@author Daniel Marques
	@author João Almeida

	Prototypes of the functions to be used for I/O operations on files

*/

/** @defgroup files files
 * @{
 *
 * Functions used for I/O operations on text files
 */

/**
  @brief Read the highscores from a file
  @param array Place to put the highscores read
  @param size How many highscores to read
  @param fileName Name of the file to read the highscores from
*/
void readHighScores(unsigned int array[], unsigned int size, char * file_name);

/**
  @brief Writes the highscores to a file
  @param array Place with the highscores
  @param size How many highscores are in the array
  @param fileName Name of the file to write the highscores to
*/
void writeHighScores(unsigned int array[], unsigned int size, char * file_name);

/**
  @brief Updates the array of highscores
  @param array Place with the highscores
  @param size How many highscores are in the array
  @param score Score to add to the highscores array
*/
void updateHighScores(unsigned int array[], unsigned int size, unsigned int score);
