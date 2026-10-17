#ifndef LETTERS_H
#define LETTERS_H
#include "sprite.h"
#include "letters.h"
#include "images.h"

/** @defgroup letters 
 * @{
 *
 * Functions related to letters 
 */

#define LETTER_PIXEL_WIDTH 10
#define LETTER_PIXEL_HEIGHT 14
#define NUMBER_PIXEL_HEIGHT 16
#define NUMBER_PIXEL_WIDTH 14
#define LETTERS_START 0
typedef struct xpm_char xpm_char;

typedef struct {
    xpm_image_t *letters;
    int startX;
    int width, height;
} letter_xpm;


struct xpm_char{
    letter_xpm xpm;
    char c;
    xpm_char *next;
};


typedef struct{
    xpm_char *first;
    
} xpm_string;


/**
 * @brief Get the Letter Xpm for the char c
 * 
 * @param c 
 * @return letter_xpm 
 */
letter_xpm getLetterXpm(char c);
//xpm_char get_char_xpm(char c); 
//xpm_string get_string_xpm(char * str);

#endif
