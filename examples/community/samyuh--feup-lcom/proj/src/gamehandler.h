#include <lcom/lcf.h>
#include "images.h"

/** @defgroup gamehandler gamehandler
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

/**
 * @brief Generates a random makecode from the alphabet
 * @param image ImageLibrary struct with all information about the race
 * @param dinamicImg Image to store the generated key
 * @return Makecode of the generated key
 */
uint8_t (generateKey)(ImageLibrary *image,  xpm_image_t *dinamicImg);

/**
 * @brief Main menu where interrupts are handled
 * @return 0 if no errors occured, 1 otherwise
 */
int (mainMenu)();
