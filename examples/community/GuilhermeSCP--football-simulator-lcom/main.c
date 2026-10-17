#include <minix/drivers.h>
#include <stdlib.h>
#include "TUI.h"
#include "video_txt.h"
#include "vt_info.h"


int main()
{
	char *video_mem;
	Rato rato;
	vt_info_t vt_info;

	sef_startup();
	rato.irq=mouse_subscribe_int();
	/* Get VDU info */
	vt_info_get(&vt_info);

	/* Print display info */
	vt_info_display(&vt_info);

	video_mem = vt_init(&vt_info);
	MenuPrincipal(&rato);
	kbc_clean_buf();
	mouse_unsubscribe_int();
}
