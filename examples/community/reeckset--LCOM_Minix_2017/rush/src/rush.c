#include <minix/drivers.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "gameEngine.h"

int main(int argc, char **argv)
{
	/* Initializing service */
	sef_startup();
	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(SELF);

	startEngine();

	return 0;
}
