#include "video_gr.h"
#include "Utilities.h"
#include <minix/drivers.h>
#include <stdlib.h>

unsigned char* buffer;


void initBuffer()
{
	unsigned int size= H_RES*V_RES;
	ALLOC(unsigned char,size,buffer);
}

void buffer_drawPixel(unsigned int index, unsigned char color)
{
	buffer[index] = color;
}

void printBuffer(unsigned char* destiny_adress)
{
	/*int x = H_RES-1;
	int y = V_RES-1;
	for (;x>=0;x--)
		for (y = V_RES-1;y>=0;y--)
			destiny_adress[y*H_RES+x]=buffer[y*H_RES+x];
	*/
	memcpy(destiny_adress,buffer,H_RES*V_RES);
}

void clearBuffer()
{
	int x = H_RES-1;
	int y = V_RES-1;
	for (;x>=0;x--)
		for (y = V_RES-1;y>=0;y--)
			buffer_drawPixel(y*H_RES+x,0);
}

void fillBuffer(unsigned char color){
	int x = H_RES-1;
	int y = V_RES-1;
	//unsigned char r=0;
	for (;x>=0;x--){
		/*i think the random problem solved!
		 *the seed appears in both logs with the same value
		 *but the sharks appear on different locations sometimes
		 *i think it's cause when using 2 vmwares sometimes one runs
		 *i faster than the other.
		 *consequentially sometimes one of the apps skips one draw
		 *frame because it took too long and didn't match with timer.
		 *Since rand is used im the draw the rand values of the apps
		 *stop being in sync
		 *this may not happen on a faster laptop though
		*/
		//if (x%4==0) r=(rand()%3*rand()%7%2)*8;
		for (y = V_RES-1;y>=0;y--)
			buffer_drawPixel(y*H_RES+x,color);//+r);
	}
}


void freeBuffer()
{
	free(buffer);
}
