#include "Player.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>

#define MAKECODE_A 0x1e
#define MAKECODE_D 0x20
#define X_INIT      360
#define Y_INIT      450
#define VEL_PLAYER    3

Player* newPlayer() {
	Player* p = (Player*) malloc(sizeof(Player));
	p->x = X_INIT;
	p->y = Y_INIT;
	p->vel=0;
	p->vel_i=VEL_PLAYER;
	p->sec=0;
	p->change=0;
	p->counter=0;
	p->change=0;
	p->ash_l=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/ash.bmp");
	p->ash_r=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/ash2.bmp");
	p->hPlayer=p->ash_l->bitmapInfoHeader.height;
	p->wPlayer=p->ash_l->bitmapInfoHeader.width;
	return p;
}

void drawPlayer(Player* p) {
	if(p->change==0)
		drawBitmap(p->ash_l,p->x,p->y,ALIGN_LEFT);
	else if(p->change==1)
		drawBitmap(p->ash_r,p->x,p->y,ALIGN_LEFT);
}

void movePlayer(Player* p,unsigned long scancode){
	if (checkTrackCollision(p) == 0) {
		if (scancode == MAKECODE_A) {
			p->vel += VEL_PLAYER;
			p->x -= p->vel;
		} else if (scancode == MAKECODE_D) {
			p->vel += VEL_PLAYER;
			p->x += p->vel;
		}
	} else if (checkTrackCollision(p) == 1) {
		if (scancode == MAKECODE_D) {
			p->vel += VEL_PLAYER;
			p->x += p->vel;
		}
	} else if (checkTrackCollision(p) == 2) {
		if (scancode == MAKECODE_A) {
			p->vel += VEL_PLAYER;
			p->x -= p->vel;
		}
	}
	p->vel = p->vel_i;
}

void deletePlayer(Player* p){
	if(p==NULL)
		return;
	deleteBitmap(p->ash_l);
	deleteBitmap(p->ash_r);
	free(p);
}

int checkTrackCollision(Player* p){
	if (p->x <= 200)
		return 1;
	if (p->x+p->ash_l->bitmapInfoHeader.width >= 600)
		return 2;
	return 0;
}

void updateCounterPlayer(Player* p){
	p->counter++;
	if(p->counter==10 || p->counter==20 || p->counter==30 || p->counter==40 || p->counter==50 || p->counter==60)
	{
		if(p->change==0)
			p->change=1;
		else
			p->change=0;
	}
	if(p->counter==60)
	{
		p->counter=0;
		p->sec++;
	}
}

void updatePlayerSpeed(Player* p,int incSpeed){
	if(incSpeed==1)
		p->vel_i++;
}
