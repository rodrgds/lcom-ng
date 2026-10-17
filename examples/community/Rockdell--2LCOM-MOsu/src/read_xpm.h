
/** @file read_xpm.h */

/** @defgroup read_xpm Read_xpm
 *  @{
 *
 *  Function used to draw sprites.
 */

/**
 * @brief Reads a xpm-like sprite defined in "map".
 * @param map Pixmap of the sprite.
 * @param width Updates with the sprite width.
 * @param height Updates with the sprite height.
 * @return Returns the address of the allocated memory where the sprite was read or NULL on error.
*/
char *read_xpm(char *map[], int *width, int *height);


/** @} end of read_xpm */
