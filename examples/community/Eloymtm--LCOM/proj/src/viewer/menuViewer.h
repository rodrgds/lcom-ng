#ifndef MENUVIEWER_H
#define MENUVIEWER_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "../labs/video.h"
#include "../xpm/batalhaNaval.h"
#include "../xpm/play.h"
#include "../xpm/quit.h"
#include "../xpm/instr.h"
#include "boardViewer.h"

int drawMenu(); 

int drawTitle();

int drawPlayButton();

int drawInstrButton();

int drawQuitButton();

int drawSelected(int selectedMenu, uint32_t color);
#endif
