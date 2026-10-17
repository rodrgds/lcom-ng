#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#include <lcom/lcf.h>

#define UNASSIGNED      0        /* Constant used to identify empty cells of a grid */
#define N               9      /* Defines the grid dimensions (N x N) */
#define PRACTICE        0        /* Constant used to select stopwatch mode */
#define TIME_ATTACK     1        /* Constant used to select time attack mode */
#define EXIT_GAME       2        /* Constant used to select time attack mode */
#define ZEN             600      /* Defines the time cap for ZEN difficulty*/
#define CASUAL          450      /* Defines the time cap for CASUAL difficulty */
#define SKILLED         300      /* Defines the time cap for SKILLED difficulty */
#define MANIAC          150      /* Defines the time cap for MANIAC difficulty */
#define T_ATK_TEST      10       /* Auxiliary constant for testing */

int timer_mode;             /* Variable used to set game mode */

/**
 * @brief Generates a random number between min and max
 * 
 * @param min Minimum value established
 * @param max Maximum value established
 * 
 * @return Return a integer between min and max
 */
int (randint)(int min, int max);

/**
 * @brief Searches the grid to find an entry that is still unassigned. If 
 *    found, the reference parameters row, col will be set the location 
 *    that is unassigned, and true is returned. If no unassigned entries 
 *    remain, false is returned.
 * 
 * @param grid NxN array
 * @param row Pointer to the selected row of the grid
 * @param col Pointer to the selected column of the grid
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */
bool (FindUnassignedLocation)(int grid[N][N], int *row, int *col); 

/**
 * @brief Verifies if it is possible to store a value in a certain position
 * 
 * @param grid NxN array
 * @param row Refers to a row of the grid
 * @param col Refers to a column of the grid
 * @param num Number that is going to be used for comparison
 * @param ie auxiliary row
 * @param je auxiliary col
 * @param excep
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */  
bool (isSafe)(int grid[N][N], int row, int col, int num, int ie, int je, int excep); 

/**
 * @brief Takes a partially filled-in grid and attempts to assign values to 
 *    all unassigned locations in such a way to meet the requirements 
 *    for Sudoku solution (non-duplication across rows, columns, and boxes)
 * 
 * @param grid NxN array
 * @param ie auxiliary row
 * @param je auxiliary col
 * @param excep
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */  
bool (SolveSudoku)(int grid[N][N], int ie, int je, int excep);

/**
 * @brief Returns a boolean which indicates whether an assigned entry 
 * in the specified row matches the given number.
 * 
 * @param grid NxN array
 * @param row Refers to a row of the grid
 * @param num Number that is going to be used for comparison
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */  
bool (UsedInRow)(int grid[N][N], int row, int num);

/**
 * @brief Returns a boolean which indicates whether an assigned entry 
 * in the specified column matches the given number.
 * 
 * @param grid NxN array
 * @param col Refers to a col of the grid
 * @param num Number that is going to be used for comparison
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */  
bool (UsedInCol)(int grid[N][N], int col, int num);

/**
 * @brief Returns a boolean which indicates whether an assigned entry 
 * in the specified diagonal matches the given number.
 * 
 * @param grid NxN array
 * @param row Refers to a row of the grid
 * @param col Refers to a col of the grid
 * @param num Number that is going to be used for comparison
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */  
bool (UsedinMDiag)(int grid[N][N], int row, int col, int num);

/**
 * @brief Returns a boolean which indicates whether an assigned entry 
 * in the specified diagonal matches the given number.
 * 
 * @param grid NxN array
 * @param row Refers to a row of the grid
 * @param col Refers to a col of the grid
 * @param num Number that is going to be used for comparison
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */ 
bool (UsedinSDiag)(int grid[N][N], int row, int col, int num);

/**
 * @brief Returns a boolean which indicates whether an assigned entry 
 *    within the specified 3x3 box matches the given number.
 * 
 * @param grid NxN array
 * @param boxStartRow Refers to the row of the first cell of the box
 * @param boxStartCol Refers to the col of the first cell of the box
 * @param num Number that is going to be used for comparison
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */ 
bool (UsedInBox)(int grid[N][N], int boxStartRow, int boxStartCol, int num);

/**
 * @brief Verifies if it is possible to store a value in a certain position
 * 
 * @param grid NxN array
 * @param row Refers to a row of the grid
 * @param col Refers to a column of the grid
 * @param num Number that is going to be used for comparison
 * @param ie auxiliary row
 * @param je auxiliary col
 * @param excep
 * 
 * @return Return TRUE if found and FALSE if otherwise
 */ 
bool (isSafe)(int grid[N][N], int row, int col, int num, int ie, int je, int excep);

/**
 * @brief A utility function to print grid
 * 
 * @param grid NxN array
 * 
 * @return Void function
 */ 
void (printGrid)(int grid[N][N]);

/**
 * @brief Puzzle generator function
 * 
 * @return Void function
 */ 
void sudoku_generator(int ncasas);

/**
 * @brief Mainmenu function
 * 
 * @return Return game mode
 */ 
int mainmenu(void);

/**
 * @brief Mainmenu function
 * 
 * @return Return difficulty value
 */ 
int diffmenu(void);

/**
 * @brief Main game function
 * 
 * @param gamemode Game mode selected
 * @param gamediff Game difficulty selected
 * 
 * @return Return 0 upon success and non-zero otherwise
 */ 
int game(int gamemode, int gamediff);

/**
 * @brief Parses game mode and difficulty selected to set inicial timer
 * 
 * @param gamemode Game mode selected
 * @param difficulty Game difficulty selected
 * @param counter pointer to initial time value
 * 
 * @return Void function
 */ 
void set_game(int gamemode, int difficulty, int *counter);

#endif /* _SUDOKU_H_ */
