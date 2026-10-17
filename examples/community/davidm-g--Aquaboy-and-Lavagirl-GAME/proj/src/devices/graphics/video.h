#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Sets the graphic mode to the specified mode.
 *
 * This function sets the graphic mode of the video device to the specified mode.
 *
 * @param mode The mode to set the graphic mode to.
 * @return 0 on success, non-zero otherwise.
 */
int set_graphic_mode(uint16_t mode);

/**
 * @brief Sets the text mode.
 *
 * This function sets the video device to the text mode.
 *
 * @return 0 on success, non-zero otherwise.
 */
int set_text_mode();

/**
 * @brief Changes the screen vertical retrace.
 *
 * This function changes the screen vertical retrace to the specified value.
 *
 * @param h The value to set the screen vertical retrace to.
 */
void change_screen_vertical_retrace(int h);

/**
 * @brief Changes the screen artifacts.
 *
 * This function changes the screen artifacts to the specified value.
 *
 * @param h The value to set the screen artifacts to.
 */
void change_screen_artifacts(int h);
