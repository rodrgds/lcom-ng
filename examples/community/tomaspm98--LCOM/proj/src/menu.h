#ifndef __MENU_H
#define __MENU_H

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "entity.h"
#include "graphics/video_gr.h"

#include "images/game_name.xpm"
#include "images/start.xpm"
#include "images/quit.xpm"




int display_menu();
int handle_mouse();

#endif 
