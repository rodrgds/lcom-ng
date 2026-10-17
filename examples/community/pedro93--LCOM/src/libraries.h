#ifndef __LIBRARIES_H
#define __LIBRARIES_H

#define H_RES          	1024
#define V_RES		 	 768

#include <time.h>
#include <stdlib.h>

extern unsigned int time_start; //!< variables to keep track of the time of the game
extern unsigned int time_end; //!< variables to keep track of the time of the game

///Minix Libraries
#include <minix/syslib.h> //!<
#include <minix/drivers.h> //!<
#include <minix/bitmap.h> //!<
#include <minix/sysutil.h> //!<
#include <machine/int86.h> //!<
#include <sys/mman.h> //!<
#include <sys/types.h> //!<
#include <string.h> //!<


///Graphical libraries
///lab's libraries
#include "i8254.h" //!<
#include "i8042.h" //!<
#include "lmlib.h" //!<
#include "RTC.h" //!<
#include "speaker.h" //!<
#include "timer.h" //!<
#include "vbe.h" //!<
#include "video_gr.h" //!<
#include "kbd.h" //!<
#include "interupts.h" //!<
#include "commodore.h" //!<

///Sprite libraries
#include "sprite.h" //!<
//#include "asprite.h" dont forget to include in the makefile
#include "pixmap.h" //!<
#include "read_xpm.h" //!<

#define BIT(n) (0x01<<(n)) //!< generic functions


#endif //__LIBRARIES_H
