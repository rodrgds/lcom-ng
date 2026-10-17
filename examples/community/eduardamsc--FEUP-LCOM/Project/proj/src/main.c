#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/syslib.h>

#include "video_gr.h"
#include "keyboard.h"
#include "obstacleCourse.h"

int main(int argc, char **argv) {
	srand(time(NULL));
	sef_startup();

	vg_init(0x117);

	ObstacleCourse* course = (ObstacleCourse*) startObstacleCourse();
	while(!course->done) {
		updateObstacleCourse(course);

		if(!course->done)
			if(course->draw)
				drawObstacleCourse(course);
	}
	stopObstacleCourse(course);

	vg_exit();

	return 0;
}
