#include <minix/drivers.h>
#include "video_gr.h"
#include "vbe.h"
#include "lmlib.h"
#include "game.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	char *video_mem;
	/* Initialize service */
	sef_startup();


	lm_init();
	/* Map memory and start video mode  */
	video_mem = vg_init(0x105);
	vg_exit();
	video_mem = vg_init(0x105);

	game();
	vg_exit();
	return 0;
}
