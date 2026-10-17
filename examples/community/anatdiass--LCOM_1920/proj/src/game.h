#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Window with the initial page of this game
 * 
 * @return int 0 upon success
 */
int start();
/**
 * @brief Page with the menu of the game, where you can choose if you want to play in singlePlayer mode or in multiplayer, if you want to see the highScores or if you want to exit the game
 * 
 * @return int 0 upon success
 */
int mainMenu();
/**
 * @brief Verifies if the game has already ended
 * 
 * @param arr 
 * @param x 
 * @return true 
 * @return false 
 */
bool endGame(int arr[], int x);
/**
 * @brief Makes the pc play in the singleplayer mode
 * 
 * @param arr 
 * @return int position of the tiger in the board for singleplayer mode
 */
int pc_play(int arr[]);
/**
 * @brief Gets the x coordinate to draw an image
 * 
 * @param arr 
 * @param i 
 * @return int x position do draw an image
 */
int draw_images_getX(int arr[],int i);
/**
 * @brief Gets the y coordinate to draw an image
 * 
 * @param arr 
 * @param i 
 * @return int y position do draw an image
 */
int draw_images_getY(int arr[],int i);
/**
 * @brief Checks if a position in the board is already occupied
 * 
 * @param board_positions 
 * @param i 
 * @return true 
 * @return false 
 */
bool isOccupied(int board_positions[], int i);
/**
 * @brief When the game ends it is added a new record with the time the play lasted
 * 
 * @param positions 
 * @param time 
 * @return int 0 upon success 
 */
int gameOver(int positions[], int time);
/**
 * @brief Starts and develops the game in singleplayer mode. The player plays against the pc
 * 
 * @return int 0 upon success 
 */
int singlePlayer();
/**
 * @brief Starts and develops the game in multiplayer mode. Player 1 and player 2 play against each other in the same pc
 * 
 * @return int 
 */
int multiplayer();
/**
 * @brief Page with the records of players that played in singlePlayer mode
 * 
 */
void highscores();
/**
 * @brief Ends the whole program
 * 
 * @return int 0 upon success 
 */
int finish();
