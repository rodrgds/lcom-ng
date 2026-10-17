#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "handler.h"

int main(int argc, char **argv) {

	/* Initialize service */

	sef_startup();
	sys_enable_iop(SELF);

	srand(time(NULL));

	mainhandler();

	return 0;

}
