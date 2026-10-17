#pragma once
#include <lcom/lcf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Score struct
typedef struct {
    unsigned long score;
    char name[20];
    char date[25];
} GameScore;
/**
* @brief Writes scores to file
*
* Reads scores from the array of size size and writes it to the scores' file
*
* @param scores - array of GameScore objects containing leaderboard scores
* @param size - score array's size
* @return Returns 0 on success, 1 otherwise
*/
int write_file(GameScore scores[], int size);
/**
* @brief Reads file to scores
*
* Reads scores from the file and writes it to the array of scores
*
* @param scores - array of GameScore objects to store read scores from file
* @param size - score array's size
* @return Returns 0 on success, 1 otherwise
*/
int read_file(GameScore scores[], int size);
/**
* @brief Inserts a score to leaderboard
*
* Inserts a given score to the leaderboard by drawing the needed characters and numbers
*
* @param scores - array of GameScore objects to store read scores from file
* @param size - score array's size
* @param gs - score of the new player to add
* @return Returns 0 on success, 1 otherwise
*/
int insert_on_leaderboard(GameScore scores[], int size, GameScore gs);
