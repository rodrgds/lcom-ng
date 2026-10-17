/**
 * @file xpm.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains functions related to xpm
 */

#ifndef _XPM_H_
#define _XPM_H_

/** @defgroup xpm xpm
 * @{
 *
 * Functions related to xpm
 */

/*
 * @ brief reads an XPM description of a pixmap, and returns
 * the pixmap as a two-dimensional char array
 *
 * @param map Pixmap whose XPM description is read
 * @param wd Width of the array
 * @param ht Height of the array
 */
char *read_xpm(char *map[], int *wd, int *ht);

/*
 * @brief Creates a XPM starting at given coordinates
 *
 * @param width Width of the XPM
 * @param height Height of the XPM
 * @param xi Starting coordinate x
 * @param yi Starting coordinate y
 * @param xpm Two-Dimensional char array of the pixmap
 */
void object_creator(int *width, int *height, unsigned short xi, unsigned short yi, char *xpm[]);

/*
 * @brief Destructs a XPM starting at given coordinates
 *
 * @param width Width of the XPM
 * @param height Height of the XPM
 * @param xi Starting coordinate x
 * @param yi Starting coordinate y
 */
void buffer_destructor(int width, int height, unsigned short xi, unsigned short yi);

/*
 * @brief Creates a title XPM starting at given coordinates
 *
 * @param xi Starting coordinate x
 * @param yi Starting coordinate y
 * @param xpm Two-Dimensional char array of the pixmap
 */
void title_creator(unsigned short xi, unsigned short yi, char *xpm[]);

/** @} end of xpm */

#endif
