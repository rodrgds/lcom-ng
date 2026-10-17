#pragma once
#include <lcom/lcf.h>
#include "xpm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**@brief Information about a letter - scancode, char, lower or upper case, sprite and xpm image*/
typedef struct letter{
    char letter;        /**< @brief The char associated with the letter*/
    bool capital;       /**< @brief True if the letter is uppercase and false if it is lowercase*/
    uint8_t breakCode;  /**< @brief The keyboard break code associated with the letter*/
    uint8_t makeCode;   /**< @brief The keyboard make code associated with the letter*/
    xpm_image_t img;    /**< @brief The xpm image of the letter*/
    uint8_t * sprite;   /**< @brief Letter's sprite*/
}letter;

/**@brief Information about a number - char, sprite and xpm image*/
typedef struct number{
    char number;           /**< @brief The char associated with the number*/
    xpm_image_t img;       /**< @brief The xpm image of the number*/
    uint8_t * sprite;      /**< @brief Number's sprite*/
}number;

#define nLetters  59       /**< @brief The number of letters and symbols in the game's alphabet*/
#define nSentences  30     /**< @brief The number of sentences that may be generated in the game*/
#define nChars  163        /**< @brief The max number of chars of a game's sentence*/

letter * abc;              /**<@brief The game's alphabet */
// 0-25 -> a,b,c,d,..,z
// 26-51 -> A,B,C,D,..,Z
// 52, 53, 54, 55, 56  ->  . , : ; '

number *numbers;           /**<@brief The game's numeric alphabet */

/**<@brief An array of char sentences that may be generated when playing the game */
static char sentencesChar[nSentences][nChars]={
    {"You control the content, so if you misspell a name or get a number wrong, it is your own fault."},
    {"The European Union wants a complete ban on drift net fishing until the end of the next year, otherwise it will take some strict measures."},
    {"According to a press release issued by district officials, the student drove a vehicle to school belonging to a parent and the weapon was in that vehicle."},
    {"Additionally, I doubt many people run for office simply because they need a job and love politics."},
    {"Add kids, cupcakes, juice boxes, a few backyard games and inexpensive party favors."},
    {"A few hours later in the evening, after my errands in the city, I went back to do my shopping, and I did want to eat dates, so I went to the fridge."},
    {"After a report last year by the University of Indiana concluded providing information alone changed student borrowing habits, MU conducted further research."},
    {"After boot camp I was stationed in San Francisco for a short time and then transferred to the Farallon Islands off the San Francisco Bay."},
    {"In addition to his service with Interact, he was also active in the Marching Band, ROTC, and National Honor Society."},
    {"Last week, the story reached a new low."},
    {"Now, your life is a fruitful testimony to the power of God's saving grace."},
    {"She also had five assists and two steals."},
    {"The Complete Stone Roses will be coming to the Blue Toon on March."},
    {"Cars Ink has earned the respect of thousands of loyal customers throughout the Southeast."},
    {"Emulate the moderate behaviour that you respect in others."},
    {"During numerous hospital visits and stays it's given me an opportunity to tell those I love just how much I value and respect them."},
    {"Don't know many servers that make less than that."},
    {"He meant that, now that I was allowed to bloody other people's copy, the goal was getting people to use their own voice."},
    {"I did in fact let my Church attendance lapse because of that."},
    {"Long nights in the computer lab is what it took for senior computer science majors David Farrow and Brett Jones to complete their video game programming projects."},
    {"Neither of them falls into the category of mediocre candidates."},
    {"On a first date, you should always have a getaway car."},
    {"She felt the campus should have one available."},
    {"We, unfortunately, experienced a recent burglary and were advised by the officer who responded that they are occurring at an almost epidemic pace."},
    {"But it seems to be paying off."},
    {"What you value you will fight for."},
    {"You've got to look forward."},
    {"But obviously, you've trained better one would assume."},
    {"Read an article about a distant place."},
    {"Reducing gas requirements by this amount would benefit oil producers not only from a cost perspective."}
};

/**<@brief Too many errors message*/
static char errorMessageChar[50] = {"Too many errors, please press BackSpace."};

/**<@brief An array of xpm's - sentences that may be generated when playing the game */
letter ** sentences; 

/**<@brief Array of xpm's representing an error message that may be showed due to too many errors */
letter * errorMessage;

/**
 * @brief Destroys sentences, letters other related variables   
*/
void destroySentencesAndLetters();

/**
 * @brief Allocate space for a two dimensional array of letters
 * 
 * @param row the number of rows of the array
 * @param col the number of columns of the array
*/
letter ** allocateTwoDimenArrayOnHeapUsingMalloc(int row, int col);

/**
 * @brief Free the allocated space for a two dimensional array of letters
 * 
 * @param ptr pointer to the array to destroy
 * @param row the number of rows of the array
 * @param col the number of columns of the array
*/
void destroyTwoDimenArrayOnHeapUsingFree(letter ** ptr, int row, int col);

/**
 * @brief Fill abc array with the game's alphabet
 */
void loadLetters(); 

/**
 * @brief Fill sentences array with the game's sentences
 */
void loadSentences();

/**
 * @brief Swap two chars
 * @param x char to swap
 * @param y char to swap
*/
inline void swap(char *x, char *y);

/**
 * @brief Reverse buffer[i..j] - auxiliar function for itoa()
 * 
 * @ buffer array to reverse
 * @ i start index
 * @ j end index
 * @return reversed array
*/
char* reverse(char *buffer, int i, int j);

/**
 * @brief Implement itoa() from C - convert a value in a specific base to an array of chars
 * 
 * @param value the number to convert to char
 * @param base the base of the number
 * @return the result of the conversion from int to array of chars
*/
char* itoa(int value, int base);
