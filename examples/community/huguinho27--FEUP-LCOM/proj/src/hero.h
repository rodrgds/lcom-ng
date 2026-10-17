#ifndef	HERO_H
#define HERO_H

/** @defgroup Hero Hero
 * @{
 * Functions for manipulating the hero
 */

typedef enum {
	WITHOUT_ANY_ARROW,
	REST_NO_ARROW,
	REST_WITH_ARROW,
	POWERING_UP,
	FULL_POWER
} hero_state;


struct Hero;
typedef struct Hero hero_t;	//it is initialized with hero_t

/**
 * @brief Gets the current number of arrows of the hero
 * @param h The hero we are referring to
 *
 * @return The number of arrows available
 */
int getNumberOfArrows(hero_t *h);

/**
 * @brief Gets the hero 'x' position
 * @param h The hero we are referring to
 *
 * @return The hero 'x' position
 */
int getHeroXPos(hero_t *h);

/**
 * @brief Gets the hero 'y' position
 * @param h The hero we are referring to
 *
 * @return The hero 'y' position
 */
int getHeroYPos(hero_t *h);

/**
 * @brief Decrements the number of arrows of the hero in 1
 * @param h The hero we are referring to
 */
void releaseArrow(hero_t *h);

/**
 * @brief Checks if hero movement is possible (inside the screen)
 * @param h The hero we are referring to
 *
 * @return 1 on true, 0 otherwise
 */
int heroMovementInsideScreen(hero_t *h);

/**
 * @brief Create a new 'instance' of hero, allocating needed memory
 *
 * @return Pointer to the new allocated hero structure
 */
hero_t* createHero();

/**
 * @brief Increments the number of balloons destroyed by the hero
 * @param h The hero
 */
void heroDestroyedBalloon(hero_t *h);

/**
 * @return The number of balloons destroyed by the hero
 * @param h The hero
 */
int getNumberOfBalloonsDestroyed(hero_t *h);

/**
 * @brief Frees the allocated memory for an hero structure
 * @param h The hero we are referring to
 */
void deleteHero(hero_t *h);

/**
 * @brief Draws on screen the hero bitmap
 * @param h The hero we are referring to
 */
void drawHero(hero_t *h);

/**
 * @brief Updates the hero position according to the mouse movemtn
 * @param h The hero we are referring to
 */
void updateHeroPosition(hero_t *h);

/**
 * @brief Makes hero power up, changing the bitmap
 * @param h The hero we are referring to
 */
void setHeroPoweringUp(hero_t *h);

/**
 * @brief Makes the hero be in 'full power'
 * @param h The hero we are referring to
 */
void setHeroFullPower(hero_t *h);

/**
 * @brief Makes the hero be in 'rest position' with no arrow assigned
 * @param h The hero we are referring to
 */
void setHeroRestingWithoutArrow(hero_t * h);

/**
 * @brief Makes the hero in 'rest position' with an arrow assigned
 * @param h The hero we are referring to
 */
void setHeroRestingWithArrow(hero_t * h);

/**
 * @brief Because hero has no arrows, we change
 * bitmap where his quiver has no arrows
 * @param h The hero we are referring to
 */
void setHeroWithoutAnyArrow(hero_t *h);

/**
 * @brief Checks if hero is at full power
 * @param h The hero we are referring to
 *
 * @return 1 on true,0 otherwise
 */
int isHeroFullPower(hero_t *h);

/**
 * @brief Checks if hero is resting without arrow
 * @param h The hero we are referring to
 *
 * @return 1 on true,0 otherwise
 */
int isHeroRestingWithoutArrow(hero_t *h);

/**
 * @brief Checks if hero is resting with arrow
 * @param h The hero we are referring to
 *
 * @return 1 on true,0 otherwise
 */
int isHeroRestingWithArrow(hero_t *h);

/**
 * @brief Changes the arrows the hero has
 * to a certain number
 * @param h The hero we are referring to
 */
void setHeroArrowNumber(hero_t *h, int number);

#endif /**@}*/
