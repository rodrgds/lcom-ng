#include <minix/sef.h>
#include "bow_and_arrow.h"
#include "utils.h"
#include "menu.h"
#include "video_gr.h"

int main()
{
	sef_startup();
	vg_init(VIDEO_GR_MODE_117);
	while (1)
	{
		int select = selectionHandler();
		if (select == 1)
			gameLoop();
		if (select == 2)
			break;
	}
	vg_exit();
	return 0;
}

