#ifndef PROJ_H
#define PROJ_H
/** @defgroup Proj proj
 * @{
 * Functions to initialize data structures and get irqs
 */
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "i8042.h"
#include "i8254.h"
#include "timer.h"
#include "video_gr.h"
#include "kb.h"
#include "mouse.h"
#include "Bitmap.h"
#include "sudoku.h"
/** @defgroup PROJ PROJ
 * @{
 * Initializations
 */
/**
 * @brief Initializes all data structures and calls menu1() function
 */
int main();
/**
 * @brief Initialize all data structures and calls menu1() function
 * @param game data structure
 */
int load_bitmaps(Game*game);
/**
 * @brief Sets mouse bitmap height and width to avoid bitmap to be drawn out of VM
 * @param game data structure
 */
void set_mouse(Game*game);
/**
 * @brief Sets coordinates of sudoku board squares
 * @param game data structure
 */
void set_coordinates(Game*game);

#endif
/**@}*/
