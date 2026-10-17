#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse-maze.h"
#include "video_gr.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv) {
	sef_startup();
	vg_init(GRAPHIC_MODE);
	MouseMaze* maze = (MouseMaze*)startMouseMaze();
	while(!maze->validation){
		updateMouseMaze(maze);
	}
	stopMouseMaze(maze);
	vg_exit();
	return 0;
}
