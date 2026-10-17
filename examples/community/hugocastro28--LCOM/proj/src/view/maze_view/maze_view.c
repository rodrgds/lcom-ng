#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
//#include "../../ui-elements/maze.h"
#include "../initialize_pixmaps.h"
#include "maze_view.h"

//extern maze
extern uint8_t* maze_pixmap;
extern xpm_image_t maze_info;

void (draw_maze)() {
    draw_xpm(maze_pixmap, maze_info, MAZE_X, MAZE_Y);
}

void (erase_maze)() {
    draw_rectangle(MAZE_X, MAZE_Y, maze_info.width, maze_info.height, BACKGROUND);
}
