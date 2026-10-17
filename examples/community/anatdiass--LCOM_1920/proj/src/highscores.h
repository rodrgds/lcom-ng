#include <lcom/lcf.h>

/**
 * @brief Struct of a record
 * 
 */
typedef struct {
    char name[10];
    char seconds[12];
} Record;

/**
 * @brief Get the maximum object of the seconds array
 * 
 * @param seconds Array with all the times
 * @param size Number of objects that array contains
 * @return int Maximum object of array seconds
 */
int getMaxTemp(int seconds[], int size);

/**
 * @brief Stores all the record times on array res
 * 
 * @param res Stores all the times
 */
void getSecondsArray(int res[]);

/**
 * @brief Add new highscore to records array
 * 
 * @param nome Name of the player who achieves the new highscore
 * @param seg Duration of the game 
 */
void addRecord(char nome[], int seg);

/**
 * @brief Verify if the time passed as argument can be a new highscore
 * 
 * @param seconds Duration of the game
 * @return true Shorter duration than the current maximum duration on highscores table
 * @return false Higher duration than the current maximum duration on highscores table
 */
bool verifyIsRecord(int seconds);

/**
 * @brief Prints all the highscores stored in records on highscores table
 * 
 */
void printRecords();

/**
 * @brief Save all the highscores in a text file
 * 
 */
void saveRecords();

/**
 * @brief Stores information of a file on records array
 * 
 */
void loadRecords();

/**
 * @brief Builds player's name
 * 
 * @param name Name of the player
 * @param nameSize Number of characters of name
 * @param scancode Keyboard input
 */
void addRecordName(char name[], int nameSize, uint8_t scancode);

