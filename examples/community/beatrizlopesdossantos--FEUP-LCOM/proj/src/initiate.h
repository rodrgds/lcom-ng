/**
 * @defgroup initiate
 * @{
 * @brief Functions to initiate and terminate the program
 */

/**
 * @brief This function has all the funtions necessary to initialize minix right
 * 
 * @return 0 if no error as occured, 1 otherwise.
 */
int inicialize_minix();

/**
 * @brief This function has all the funtions necessary to terminate minix right
 * 
 * @return 0 if no error as occured, 1 otherwise.
 */
int exit_minix();

/**
 * @brief Loads all the xpms to its corresponding array.
 * 
 */
void load_xpms();

/**
 * @brief Create all sprites objects.
 * 
 */
void create_sprites();

/**
 * @brief Destroys all sprites objects.
 * 
 */
void destroy_all_sprites();

/** @} end of initiate */
