#ifndef __IMAGES_H
#define __IMAGES_H

/** @file images.h
 * @brief file containing declarations of the functions that deal with xpm images
 */

/**
 * @brief function to obtain an array of **chars with the cards xpms
 * @return array with the cards xpms
 */
char ***get_cards();

/**
 * @brief function to obtain the xpm with the set button
 * @return xpm with the set button xpm
 */
char **get_set_button();

/**
 * @brief function to obtain the xpm with the no sets button
 * @return xpm with the no sets button
 */
char **get_nosets_button();

/**
 * @brief function to obtain the xpm with the hint button
 * @return xpm with the hint button
 */
char **get_hint_button();

/**
 * @brief function to obtain the xpm with the pause button
 * @return xpm with the pause button
 */
char **get_pause_button();

/**
 * @brief function to obtain the xpm with the play button
 * @return xpm with the play button
 */
char **get_play_button();

/**
 * @brief function to obtain the xpm with the save button
 * @return xpm with the save button
 */
char **get_save_button();

/**
 * @brief function to obtain the xpm with the exit button
 * @return xpm with the exit button
 */
char **get_exit_button();

/**
 * @brief function to obtain an array with the xpms of the game letters
 * @return array of xpms with the game letters
 */
char ***get_letters();

/**
 * @brief function to obtain an array with the xpms of the game numbers
 * @return array of xpms with the game numbers
 */
char ***get_numbers();

/**
 * @brief function to obtain the xpm with the game background
 * @return xpm with the game background
 */
char **get_background();

/**
 * @brief function to obtain the xpm with the set logo
 * @return xpm with the set game logo
 */
char **get_set_logo();

#endif /* __IMAGES_H */
