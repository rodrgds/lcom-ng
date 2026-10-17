#ifndef PROJ_H_
#define PROJ_H_
#include <minix/drivers.h>

#include "control.h"
#include "rtc.h"
#include "timer.h"
#include "sprite.h"
#include "video_gr.h"
#include "vbe.h"
#include "xpm.h"

#define BIT(n) 				(0x01<<(n))
#define BACKGROUND 			0
#define BATTERYXPOS			H_RES-110
#define BATTERYYPOS			0
#define NUM_ASTEROIDS		35
#define ASTEROID_PERIOD		1			// controls asteroid's movement rate
#define SHOT_PERIOD			150			//Time between shot's recharge
#define DELTA				10			//Time between velocity increment

static char** digits[]={digits0,digits1,digits2,digits3,digits4,digits5,digits6,digits7,digits8, digits9};

//Represents each element in the game
Sprite* batteries[5];
Sprite* battleship;
Sprite* asteroids[NUM_ASTEROIDS];
Sprite* shotsprt[4];
Sprite* timesprt[3];
Sprite* game_over;
int posx_inicial=(H_RES/2)-20;
int posy_inicial=V_RES-100;
unsigned short times[]={0,0,0}, timesalarm[]={0,0,0}; // stores time since the game started
													  // and the time read from the RTC

//Relevant to know when to stop reading time, playing, and when to update framebuffer
unsigned short readtime=0, finished=0, changed=0;

//Sprite's velocities and remaining shots
unsigned int asteroidvel=1, shotvel=5, shipvel=10, shots=4, intcounter=1;

void timer_int_handler();

#endif
