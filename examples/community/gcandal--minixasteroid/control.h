#ifndef _CONTROL_H_
#define _CONTROL_H_

/** @defgroup control control
 * @{
 *
 * Functions for using and interpreting keyboard/mouse responses in order to move the spaceship
 */

#define STAT_REG 			0x64
#define KBC_CMD_REG 		0x64
#define OUT_BUF				0x60
#define IN_BUF 				0x60 //or 0x64
#define KBD_IRQ				1
#define MOUSE_IRQ			12
#define DELAY_US 			20000


//C@KBD responses
#define ESC_MAKE			0x01
#define ESC_BREAK			0x81
#define SPACE_BREAK			0x39
#define SPACE_MAKE			0xB9

#define BIT(n) 				(0x01<<(n))

//Status Register
#define OBF					BIT(0) // =1: full; Outbuffer
#define PAR_ERR				BIT(7)
#define TO_ERR				BIT(6)

//Mouse commands
#define ENABLE_MOUSE		0xA8
#define ENABLE_DATA			0xF4
#define DISABLE_DATA		0xF5
#define WRITE_BYTE			0xD4 //commands the KBC to forward its argument to the mouse

static int mousehook=3, kbdhook=5;
static unsigned char packet[3];
static unsigned short count=0;
static unsigned long stat;

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @return Return 0 upon success
 */
int kbd_subscribe_int();
/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success
 */
int kbd_unsubscribe_int();
/**
 * @brief Reads the scancode from the CKBD
 *
 * @return Returns the scancode
 */
static unsigned char kbd_readscancode();
/**
 * @brief Interprets keyboard interrupt
 *
 * Reads information from keyboard and interprets it.
 *
 * @param shots Number of shots the spaceship can still do
 * @return Returns -1 if no relevant response, 1 if a shot has been fired and 0 if ESC was pressed.
 */
int kbd_int_handler(const unsigned int shots);
/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @return Return 0 upon success
 */
int mouse_subscribe_int();
/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success
 */
int mouse_unsubscribe_int();
/**
 * @brief Interprets mouse interrupt
 *
 * Reads information from the mouse and interprets it.
 *
 * @param x Ship's coordinates
 * @param y Ship's coordinates
 * @param posx_inicial Ship's coordinates
 * @param posy_inicial Ship's coordinates
 * @param H_RES Horizontal screen resolution
 * @param V_RES Vertical screen resolution
 * @return Returns -1 if no relevant response, 1 if a shot has been fired and 0 if ESC was pressed.
 */
int mouse_int_handler(int* x, int* y, int* posx_inicial, int* posy_inicial, int H_RES, int V_RES);

#endif
