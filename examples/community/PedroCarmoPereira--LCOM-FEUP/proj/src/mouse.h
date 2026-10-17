#ifndef _MOUSE_H
#define _MOUSE_H

/** @defgroup Mouse Mouse
 * @{
 */

/*
struct packet{
  uint8_t bytes[3]; //mouse packet raw bytes
  bool rb;          //right mouse button pressed
  bool mb;          //middle mouse button pressed
  bool lb;          //left mouse button pressed
  uint16_t delta_x; //mouse x-displacement: rightwards is positive
  uint16_t delta_y; //mouse y-displacement: upwards is positive
  bool x_ov;        //mouse x-displacement overflow
  bool y_ov;        //mouse y-displacement overflow
};
*/

typedef enum {INIT, DRAW_UP, VERT,DRAW_DOWN, COMP} state_t;

/*
@brief subscribes to mouse interrupts

@param bit_no returns the bit of the mouse irq

@return 0 if everything is ok
*/

int mouse_subscribe_int(uint8_t *bit_no);

/*
@brief unsubscribes the mouse interrupts

@return 0 if everything is ok
*/

int mouse_unsubscribe_int();

int mouse_cmd_aux(uint8_t cmd);

int mouse_cmd(uint8_t cmd);

/*
@brief sets stream mode

@return r if there is an error
*/

int set_stream();

/*
@brief sets remote mode

@return r if there is an error
*/

int set_remote();

/*
@brief Checks if a bytes Bit 4 is set

@param byte   the byte to be checked

@return   True if so
*/
bool isNegX(uint8_t byte);

/*
@brief Checks if a bytes Bit 5 is set

@param byte   the byte to be checked

@return True if so
*/
bool isNegY(uint8_t byte);

/*
@brief Parses a given byte into a packet struct

@param byte to be parsed

@return True byte was parsed correctly
*/
bool parseByte(uint8_t byte);

/*
@brief Polls the mouse

@return 0
*/
int mouse_poll();

/*
@brief processes a mouse event

@param event    mouse event to be processed

@param len      distance covered by the mouse in a given state

@param tol      tolerance to deviation from pattern to be drawn
*/
void mouse_process_event(struct mouse_ev * event, uint8_t len, uint8_t tol);

/*
@brief Debug funtion prints mouse state

@param s    state to be printed
*/
void printState(state_t s);

#define MOUSE_IRQ		12

#define MOUSE_CMD 		0xD4
#define DIS_DATA_R		0xF5
#define EN_DATA_R		0xF4
#define SET_STREAM		0xEA
#define SET_REMOTE		0xF0
#define READ_DATA    	0xEB
#define ACK				0xFA
#define NACK			0xFE
#define ERROR			0xFC

#endif /*_MOUSE_H*/
