#pragma once

/** @defgroup proj proj
 * @{
 * Functions for manipulating the project
 */

/**
 * @brief      Project main funtion
 *
 * @param[in]  argc  number of arguments
 * @param      argv  arguments
 *
 * @return     { return 0 on program success other number otherwise }
 */
int main(int argc, char *argv[]);

/**
 * @brief      Project main loop
 *
 * @param[in]  argc  number of arguments
 * @param      argv  arguments
 *
 * @return     { return 0 on program success other number otherwise }
 */
int(proj_main_loop)(int argc, char *argv[]);
