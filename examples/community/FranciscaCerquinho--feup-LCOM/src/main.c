#include <stdio.h>
#include <limits.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <string.h>
#include <errno.h>
#include "i8254.h"
#include "Bitmap.h"
#include "kbd.h"
#include "timer.h"
#include "vbe.h"
#include "video_gr.h"
#include "mouse.h"
#include "Background.h"
#include "Pipe.h"
#include "Mario.h"
#include "StateMachineMenu.h"
#include "MarioEvent.h"
#include "Colision.h"
#include "rtc.h"
#include "IRQ.h"


int main()
{
	char *map;
	int score;
	sef_startup();

	IRQ* irqs = Subscribe();

	map = vg_init(0x117);

	int test=Menu(map, irqs);



	switch(test){
	case 1: {
		vg_exit();
		break;
	}
	case 2: {
		score=UpdateMarioGame(map, irqs);
		vg_exit();
		WriteScore(score);
		break;
	}
	}

	void Unsubscribe();

}


