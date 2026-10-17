#ifndef _TABLE_H_
#define _TABLE_H_

#include "../devices/timer/rtc.h"
#include "../devices/kbc/keyboard.h"
#include <string.h>
#include "player.h"

#define MAX_PERSONS 50
#define MAX_PERSONS_DRAW 10

extern Player p1;
extern time_info data;

typedef struct {
    char name[MAX_LENGTH];
    time_info time;
    int money;
} person_info;


/**
 @brief Reads data from a file.
This function opens a file located at "/home/lcom/labs/g5/proj/src/view/data.txt" in read mode.
It reads the data from the file and stores it in the persons array, up to the maximum number of persons (MAX_PERSONS).
The data in the file is expected to be in a specific format, with each line containing name, date and time, and money information.
 @return 0 if the file is successfully read, 1 otherwise.
*/
int read_file();


/**
 @brief Deletes a game record at a given index.
This function deletes a game record at the specified index in the persons array.
It checks if the name of the person at the given index matches the name of the current player (p1).
If the names do not match, it prints an error message and returns 1.
Otherwise, it shifts the remaining game records in the array to fill the gap created by the deletion,
decrements the person_count, and prints a success message before returning 0.
 @param index The index of the game record to delete. 
 @return 0 if the game record is successfully deleted, 1 otherwise.
*/
int delete_game(uint8_t index);


/**
 @brief Saves data to a file.
This function opens a file located at "/home/lcom/labs/g5/proj/src/view/data.txt" in write mode.
It writes the game data stored in the persons array to the file.
The data is written in a specific format, with each line containing name, date and time, and money information.
After writing the data, it closes the file, resets the person_count to 0, and returns 0 to indicate success.
 @return 0 if the file is successfully saved, 1 otherwise.
*/
int save_file();


/**
 @brief Adds a game to the game records.
This function adds a new game to the game records based on the data provided in the to_save string.
It extracts the name, date and time, and money information from the string and stores it in a person_info structure.
If the person_count is less than MAX_PERSONS (maximum number of game records) and the game time is not in the past,
the new game record is added to the persons array and person_count is incremented.
 @return 0 if the game is successfully added, 1 otherwise.
*/
int add_game();

/**
 @brief Prints person information to a file.
This function writes the person's name, date and time, and money to the specified file in a specific format.
 @param file Pointer to the file where the information will be printed.
 @param p Pointer to the person_info structure containing the person's information.
*/
void print_person_to_file(FILE *file, const person_info *p);

#endif
