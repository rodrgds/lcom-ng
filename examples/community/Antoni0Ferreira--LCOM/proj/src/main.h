#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

/**
 * @defgroup main main
 * @{
 * 
 * Main loop of the project
 */

/**
 * @brief Project loop
 * 
 * @param argc number of arguments
 * @param argv array of arguments
 * @return Return 0 upon success and non-zero otherwise 
 */
int (proj_main_loop)(int argc, char *argv[]);

/**
 * @}
 */

#endif //MAIN_H_INCLUDED
