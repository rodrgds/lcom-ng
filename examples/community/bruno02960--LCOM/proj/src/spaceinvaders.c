/**
 * @file spaceinvaders.c
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief SPACE INVADERS
 */

#include "game.h"
#include "devices.h"
#include "print.h"
#include "variables.h"

/** @defgroup spaceinvaders spaceinvaders
 * @{
 *
 * Main function
 */

int main(int argc, char **argv) {
	int i;

	sef_startup();

	if (argc == 1) {
		printf("\t service run %s -args \"Player Name\"\n", argv[0]);
		return 0;
	} else {
		strcpy(user, argv[1]);

		for (i = 2; i < argc; ++i) {
			strcat(user, " ");
			strcat(user, argv[i]);
		}
	}

	if (devices_subscriptions())
		return 1;

	print_menu();

	if (devices_unsubscriptions())
		return 1;

	return 0;
}

/** @} end of spaceinvaders */
