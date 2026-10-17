/**
	@file mouse.c
	@author Daniel Marques
	@author João Almeida

	Declarations of the functions used to communicate with the mouse and KBC
	Contains some functions imported from lab4 as well as some other functions useful on this Project
*/


#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "graphics.h"
#include "game.h"

/************* BEGIN IMPORTED FROM LAB3 *************/
void mouseSubscribe(int mouse_id) {
	sys_irqsetpolicy( MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_id);
	sys_irqenable(&mouse_id);
}

void mouseUnsubscribe(int mouse_id) {
	unsigned long int stat;
	sys_inb(IN_BUF, &stat);
	sys_inb(OUT_BUF, &stat);
	sys_irqrmpolicy(&mouse_id);
}

int sendCmd(unsigned long int port, unsigned long int cmd) {

	unsigned long int stat;
	while (1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) { //CHECKS IF BUFFER IS FULL
			sys_outb(port, cmd);
			return 0;
		} else
			return -1;
		tickdelay(micros_to_ticks(20000));
	}
}

unsigned long int receiveData(unsigned long int * data) {
	unsigned long int stat, cont = 0;
	while (cont < 5) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) { //CHECK IF BUFFER IS FULL
			if (sys_inb(OUT_BUF, data) == OK)
				return *data;

		}
		tickdelay(micros_to_ticks(20000));
		cont++;
	}
}

int readMouseByte(unsigned char * packet, unsigned short int byte_counter) {
	long unsigned int byte;
	byte = receiveData(&byte);
	packet[byte_counter] = byte;
	if (byte_counter == 0) {
		if ((byte & BIT(3)) != 0) { //BIT 3 IS SET
			return 0;
		} else
			return 1;
	} else
		return 0;

}
/************* END IMPORTED FROM LAB3 *************/

void updateMouseCoordinates(game_t *game, unsigned char packet[]){
  int incx = packet[1];
	int incy = packet[2];

	if ((Y_SIGN & packet[0]) != 0)  //Y<0
		incy = abs(incy - 256);
  else
    incy = -incy;

	if ((X_SIGN & packet[0]) != 0)  //X<0
		incx = -abs(incx - 256);

	if (incx + game->mouse.x <= 0)
		game->mouse.x = 0;
	else if (incx + game->mouse.x > getHRes())
		game->mouse.x = getHRes()-1;
	else
		game->mouse.x += incx;

	if (incy + game->mouse.y <= 0)
		game->mouse.y = 1;
	else if (incy + game->mouse.y > getVRes())
		game->mouse.y = getVRes()-1;
	else
		game->mouse.y += incy;
}

unsigned int mouseAboveButton(game_t *game , unsigned int relevant){
  if (relevant){
    unsigned int x = game->mouse.x , y = game->mouse.y;
    unsigned int px = game->menus.playx , py = game->menus.playy,
                 ex = game->menus.exitx , ey = game->menus.exity;
    unsigned int pmx = px + game->menus.play_w , pmy = py + game->menus.play_h,
                 emx = ex + game->menus.exit_w , emy = ey + game->menus.exit_h;

	  if ( x >= ex && x <= emx && y >= ey && y <= emy ) //PLAY BUTTON
      return 1;
    else if (x >= px && x <= pmx && y >= py && y<=pmy)
      return 0;
  }
  return 2;
}
