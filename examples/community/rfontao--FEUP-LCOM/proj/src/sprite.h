#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <lcom/lcf.h>

#include "vbe.h"

/** @defgroup Sprite Sprite
 * @{
 *
 * Functions and constant of the sprites
 */

#define PI 3.14159265		/**< @brief The value of pi*/
				//TODO
#define tg_72 3.07768354	/**< @brief  */
#define tg_54 1.37638192	/**< @brief */
#define tg_36 0.72654253	/**< @brief */
#define tg_18 0.3249197		/**< @brief */

/**
 * @brief The Sprite Struct
 * 
 */
typedef struct{
	int x; 			/**< @brief X pos of the Sprite*/
	int y;			/**< @brief Y pos of the Sprite*/
	int width;		/**< @brief Width of the Sprite*/
	int height;		/**< @brief Height of the Sprite*/
	int xspeed;		/**< @brief X velocity of the Sprite*/
	int yspeed;		/**< @brief Y velocity of the Sprite*/
	uint8_t * map;		/**< @brief The array that contains the Sprite*/
} Sprite;

/**
 * @brief The Car Struct
 * 
 */
typedef struct {
	Sprite* sprite;			/**< @brief The Car Sprite */
	unsigned short int id;		/**< @brief Id of the Car object*/
	unsigned int hp;		/**< @brief Remaining health points of the Car*/
} Car;

/**
 * @brief The Objective Struct
 * 
 */
typedef struct {
	Sprite *sprite;		/**< @brief The Obective Sprite*/
	unsigned int hp;	/**< @brief Remaining health points of the Objective*/
} Objective;

/**
 * @brief Loads a new Sprite and returns it
 * 
 * @param xpm 		Xpm of the new Sprite
 * @return Sprite* 	A pointer to the new Sprite object
 */
Sprite * loadSprite(xpm_map_t xpm);

/**
 * @brief Draws a sprite on the screen at (x,y)
 * 
 * @param sprite 	Sprite Object to be drawn
 * @param x 		X of the coordinates to draw the sprite  
 * @param y		Y of the coordinates to draw the sprite
 */
void drawSprite(Sprite * sprite, int x, int y);

/**
 * @brief Destructor of Sprite
 * 
 * @param sprite 	The Sprite to be destroyed
 */
void destroy_sprite(Sprite *sprite);

/**
 * @brief Moves the car and keeps it inside the game
 * 
 * @param sprite 	The Car object
 * @param arma 		The Gun object
 */
void move_Car(Car *sprite, Sprite *arma);

/**
 * @brief Moves a sprite
 * 
 * @param sprite 	Sprite object to be moved
 */
void move_sprite(Sprite *sprite);

/**
 * @brief Draws the entire background to the game with memcpy
 * 
 * @param background 	Background to be drawn
 */
void drawBackground(Sprite * background);

/**
 * @brief Animates the Car object based on the direction and the array of Car sprites
 * 
 * @param car 		The Car object
 * @param carAnim 	The array of Car Sprites
 */
void anim_Car(Car * car, Sprite * carAnim[]);

/**
 * @brief Animates the Gun Sprite based on the direction and the array of Gun sprites
 * 
 * @param arma 		The Gun Sprite
 * @param armaAnim 	The array of Gun Sprites
 */
void anim_Arma(Sprite * arma, Sprite * armaAnim[]);

/**
 * @brief Updates the game camera coordinates based on the position of sprite(keeps it in the      *  	  middle)
 * 
 * @param sprite	The reference Sprite 
 * @param x 		The camera X pos
 * @param y 		The camera Y pos
 */
void updateCameraCoords(Sprite * sprite,int * x,int * y);

/**
 * @brief Displays a time counter and current score
 * 
 * @param digits 	The array of digits Sprites
 */
void displayTimeAndScore(Sprite* digits[]);

/**@}*/

#endif
