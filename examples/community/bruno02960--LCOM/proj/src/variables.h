/**
 * @file variables.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains list of macros, variables, macros and structs used in the game
 */

#ifndef __VARIABLES_H
#define __VARIABLES_H

#include <minix/syslib.h>

int total_score;				/**< @brief Player total score */
unsigned short time_counter;	/**< @brief Game time counter */
char user[100];					/**< @brief User name*/

#define ENEMY_ROWS		5		/**< @brief Number of enemy rows */
#define ENEMY_COLLUMNS  10		/**< @brief Number of enemy columns */
#define SHIP_X_INITIAL  512 	/**< @brief Initial coordinate y */
#define SHIP_Y_INITIAL  600 	/**< @brief Initial coordinate x */
#define SHIP_SPEED      15		/**< @brief Ship speed on keyboard */
#define PONG_SPEED      20		/**< @brief Pong speed */
#define BETWEEN_ENEMIES 100		/**< @brief Horizontal distance between enemies */
#define FST_ENEMY_COL   50		/**< @brief Coordinate y of the first enemy column */
#define ENEMY_DOWN_TIME 5		/**< @brief Time between enemy movements */
#define ENEMY_DOWN_Y	50		/**< @brief Distance per movement */
#define	BLINKS			3		/**< @brief Number of blinks of the title */

#define TIME_COUNT_X    300		/**< @brief Time count title x coordinate */
#define TIME_COUNT_Y    700		/**< @brief Time count title y coordinate */
#define TOTAL_SCORE_X   1000	/**< @brief Total score title x coordinate */
#define TOTAL_SCORE_Y   700		/**< @brief Total score title y coordinate */
#define CONTROLS_X   	100		/**< @brief Controls title x coordinate */
#define CONTROLS_Y		250		/**< @brief Controls title y coordinate */
#define SPACEBAR_X   	400		/**< @brief Spacebar title x coordinate */
#define SPACEBAR_Y		650		/**< @brief Spacebar title y coordinate */
#define NEW_GAME_X   	300		/**< @brief New game title x coordinate */
#define NEW_GAME_Y		200		/**< @brief New game title y coordinate */
#define INSTRUCTIONS_X  300		/**< @brief Instructions title x coordinate */
#define INSTRUCTIONS_Y	380		/**< @brief Instructions title y coordinate */
#define EXIT_X			700		/**< @brief Exit title x coordinate */
#define EXIT_Y			650		/**< @brief Exit title y coordinate */
#define GAME_MODES_X	150		/**< @brief Game modes title x coordinate */
#define GAME_MODES_Y	150		/**< @brief Game modes title y coordinate */
#define FINAL_SCORE_X   340		/**< @brief Final score title x coordinate */
#define FINAL_SCORE_Y   300		/**< @brief Final score title y coordinate */
#define WIN_LOSE_X		270		/**< @brief Win or lose title x coordinate */
#define WIN_LOSE_Y		346		/**< @brief Win or lose title y coordinate */
#define TIME_XPM_X		50		/**< @brief Time display x coordinate */
#define SCORE_XPM_X     780		/**< @brief Score display x coordinate */
#define ENTER_XPM_X		400		/**< @brief Press enter display x coordinate */
#define GAME_TITLES_Y	700		/**< @brief Game titles y coordinate */

unsigned short time_counter;	/**< @brief Time counter */

unsigned long kbd_set;
unsigned long timer_set;
unsigned long mouse_set;
unsigned long rtc_set;
int kbd_subs;
int mouse_subs;
int time_subs;
int rtc_subs;

unsigned long kbd_code;
int ipc_status;
message msg;

/** @name Enemy info block */
/**@{
 * @brief Enemy info block
 */
typedef struct
{
	int x;				/**< @brief Upper left corner coordinate X */
	int y;				/**< @brief Upper left corner coordinate Y */
	int width;			/**< @brief Width of the XPM */
	int height;			/**< @brief Height of the XPM */
	int score;			/**< @brief Score obtained by the destruction of the enemy */
	int destroyed;		/**< @brief Flag indicating if the enemy is destroyed */
	char** map;			/**< @brief Map of the XPM */
} Enemy_t;

/** @} end of Enemy_t*/

/*
 * @brief Enemy object declaration
 */
Enemy_t enemy;

/*
 * @brief Enemy matrix declaration
 */
Enemy_t enemies[ENEMY_ROWS][ENEMY_COLLUMNS];

/** @name Ship info block */
/**@{
 * @brief Ship info block
 */
typedef struct
{
	int x;				/**< @brief Upper left corner coordinate X */
	int y;				/**< @brief Upper left corner coordinate Y */
	int width;			/**< @brief Width of the XPM */
	int height;			/**< @brief Height of the XPM */
	char** map;			/**< @brief Map of the XPM */
} Ship_t;

/** @} end of Ship_t*/

/*
 * @brief Ship declaration
 */
Ship_t ship;

/** @name Pong info block */
/**@{
 * @brief Pong info block
 */
typedef struct
{
	int x;				/**< @brief Upper left corner coordinate X */
	int y;				/**< @brief Upper left corner coordinate Y */
	int width;			/**< @brief Width of the XPM */
	int height;			/**< @brief Height of the XPM */
	char** map;			/**< @brief Map of the XPM */
} Pong_t;

/** @} end of Pong_t*/

/*
 * @brief Pong declaration
 */
Pong_t pong;

/*
 * @brief State machine
 */
typedef enum { START, PONG_ENABLED, PONG_DISABLED, NO_ENEMIES, GAME_OVER, ENTER_PRESSED } State_t;

/*
 * @brief Game state
 */
State_t estado;

/*
 * @brief Main Menu State machine
 */
typedef enum { NEW_GAME, INSTRUCTIONS, LEAVE_GAME} Menu_t;

/*
 * @brief Menu state
 */
Menu_t menu;

/*
 * @brief Menu State machine to choose keyboard or mouse
 */
typedef enum { MOUSE_MOUSE, KEYBOARD_KEYBOARD, MOUSE_KEYBOARD, KEYBOARD_MOUSE, PLAYED} KBD_mouse_t;

/*
 * @brief KBD_Mouse state
 */
KBD_mouse_t kbd_mouse_menu;

#endif  /* __VARIABLES_H*/


