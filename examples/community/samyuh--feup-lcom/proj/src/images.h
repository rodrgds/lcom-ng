#ifndef IMAGE_H
#define IMAGE_H

#include <lcom/lcf.h>
/** @defgroup images images
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Struct with needed images for the Game
 */
typedef struct {
  //Menu
  xpm_image_t menu; /*!< Image of main Menu  */
  xpm_image_t menu_highscores; /*!< Image of Highscores Menu */
  xpm_image_t menu_LevelSelect; /*!< Image of LEvel Selection Menu */
  xpm_image_t endGame; /*!< Image of End Game Menu */
  xpm_image_t locked;  /*!< Image of Locked level Menu */

  //Arrow
  xpm_image_t arrow; /*!<  Image of Arrow */

  //Cars
  xpm_image_t blueCar; /*!< Image of Blue Car */

  //Tracks
  xpm_image_t track1; /*!< Image of Level 1 Track */
  xpm_image_t track2; /*!< Image of Level 2 Track */
  xpm_image_t track3; /*!< Image of Level 3 Track */
  xpm_image_t track4; /*!< Image of Level 4 Track */
  xpm_image_t track5; /*!< Image of Level 5 Track */
  xpm_image_t track6; /*!< Image of Level 6 Track */
  xpm_image_t track7; /*!< Image of Level 7 Track */
  xpm_image_t track8; /*!< Image of Level 8 Track */

  //Nitro
  xpm_image_t nitroAvailable; /*!< Image of Nitro Bar availability */
  xpm_image_t nitroActive; /*!< Image indicating Nitro Bar is active */
  xpm_image_t nitroFullBar; /*!< Image indicating Nitro Bar is full */
  xpm_image_t nitroBar; /*!< Image of Nitro Bar */
  xpm_image_t fuel; /*!< Image of Nitro Bar Fuel */

  //Letters
  xpm_image_t letterA; /*!< Image of letter A */
  xpm_image_t letterB; /*!< Image of letter B */
  xpm_image_t letterC; /*!< Image of letter C */
  xpm_image_t letterD; /*!< Image of letter D */
  xpm_image_t letterE; /*!< Image of letter E */
  xpm_image_t letterF; /*!< Image of letter F */
  xpm_image_t letterG; /*!< Image of letter G */
  xpm_image_t letterH; /*!< Image of letter H */
  xpm_image_t letterI; /*!< Image of letter I */
  xpm_image_t letterJ; /*!< Image of letter J */
  xpm_image_t letterK; /*!< Image of letter K */
  xpm_image_t letterL; /*!< Image of letter L */
  xpm_image_t letterM; /*!< Image of letter M */
  xpm_image_t letterN; /*!< Image of letter N */
  xpm_image_t letterO; /*!< Image of letter O */
  xpm_image_t letterP; /*!< Image of letter P */
  xpm_image_t letterQ; /*!< Image of letter Q */
  xpm_image_t letterR; /*!< Image of letter R */
  xpm_image_t letterS; /*!< Image of letter S */
  xpm_image_t letterT; /*!< Image of letter T */
  xpm_image_t letterU; /*!< Image of letter U */
  xpm_image_t letterV; /*!< Image of letter V */
  xpm_image_t letterW; /*!< Image of letter W */
  xpm_image_t letterX; /*!< Image of letter X */
  xpm_image_t letterY; /*!< Image of letter Y */
  xpm_image_t letterZ; /*!< Image of letter Z */

  //Numbers
  xpm_image_t number0; /*!< Image of number 0 */
  xpm_image_t number1; /*!< Image of number 1 */
  xpm_image_t number2; /*!< Image of number 2 */
  xpm_image_t number3; /*!< Image of number 3 */
  xpm_image_t number4; /*!< Image of number 4 */
  xpm_image_t number5; /*!< Image of number 5 */
  xpm_image_t number6; /*!< Image of number 6 */
  xpm_image_t number7; /*!< Image of number 7 */
  xpm_image_t number8; /*!< Image of number 8 */
  xpm_image_t number9; /*!< Image of number 9 */

  // Punctuation
  xpm_image_t twoDots; /*!< Image of two dots */
  xpm_image_t slash; /*!< Image of a forward slash */
} ImageLibrary;

/**
 * @brief Fills all elements of the struct Image Library
 * @param tmp ImageLibrary that will be filled
 * @return returns changed tmp ImageLibrary
 */
ImageLibrary* newImageLibrary(ImageLibrary* tmp);

/**
 * @brief Creates an image of a number to be drawn
 * @param self ImageLibrary where the number image will be searched
 * @param n Indicates the digit that will be returned
 * @return Image of the chosen number
 */
xpm_image_t numberToDraw(ImageLibrary* self, int n);

/**
 * @brief Creates an image of a character to be drawn
 * @param self ImageLibrary where the character image will be searched
 * @param c Indicates the character that will be returned
 * @return Image of the chosen character
 */
xpm_image_t charToDraw(ImageLibrary* self, char c);

/**
 * @brief Draws a Number on the Screen
 * @param image ImageLibrary where the number xpm is stored
 * @param x Position in the x axis where the number will be drawn
 * @param y Position in the y axis where the number will be drawn
 * @param value Number that willbe drawn
 * @param size Size of xpm. 1 for the same size, 0.5 for half.
 */
void drawNumber(ImageLibrary *image, int x, int y, int value, double size);

#endif /* IMAGE_H */
