/** @file read_xpm.h
 * @brief file containing declarations of the functions that deal with xpm files
 */

/** @defgroup read_xpm read_xpm
 * @{
 *
 * @brief functions for handling the xpm files
 */

/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

/** Reads a xpm-like sprite defined in "map" (look at pixmap.h for
 * examples). Returns the address of the allocated memory where the
 * sprite was read. Updates "width" and "heigh" with the sprite
 * dimension.  Return NULL on error.
 * Assumes that VRES and HRES (the screen vertical and horizontal resolution)
 * are externaly defined.
 *
 * Usage example, using the defined sprite in pixmap.h:
 * <pre>
 *   #include "pixmap.h" // defines  pic1, pic2, etc
 *   int wd, hg;
 *   char *sprite = read_xpm(pic1, &wd, &hg);
 * </pre>
 */

/**
 * @brief reads a xpm-like sprite defined in "map"
 * @param map the map with the xpm definition
 * @param width the width of the pixmap
 * @param height the height of the pixmap
 * @return NULL on error, the address of the allocated memory where the sprite was read
 */
char *read_xpm(char *map[], int *width, int *height);
