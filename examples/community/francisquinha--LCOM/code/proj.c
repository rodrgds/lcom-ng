
/** @file proj.c
 * @brief file containing main function for the game project
 */

#include <minix/drivers.h>

#include "game.h"

int main(int argc, char **argv) {
	srand(time(NULL));
	sef_startup();	/* Initialize service */
    sys_enable_iop(SELF);
	run_game();
	return 0;
}
