
#ifndef _CIRCLE_H_
#define _CIRCLE_H_

/** @defgroup circle Circle
 *  @{
 *
 *  Circle related functions
 */

/** A Circle is an "object" that contains all needed information to create, tick,
 *  draw, erase and destroy circles on the screen.
 */
typedef struct {

	unsigned int x;		/**< X position of circle. */
	unsigned int y;		/**< Y position of circle. */

	unsigned int t;		/**< Time left on the circle. */

	unsigned int r;		/**< Radius of the main circle. */
	unsigned int out_r;	/**< Radius of the outer circle. */

	unsigned long c;	/**< Color of the circle. */

	unsigned long l;	/**< Letter assigned to the circle (keyboard code). */

} Circle;

/**
 *  @brief Creates a game circle.
 *
 *  These circles have a main and an outer radius.
 *
 *  @param mode Mode of the game (0 for easy mode and 1 for hard mode)
 *  @return Returns a pointer to a circle.
 *  @see Circle
 */
Circle* create_circle(int mode);

/**
 *  @brief Creates a menu circle.
 *
 *  These circles only have a main circle.
 *
 *  @return Returns a pointer to a circle.
 *  @see Circle
 */
Circle * menu_create_circle();

/**
 *  @brief Draws a game circle in the screen.
 *  @param c Game Circle to draw.
 */
void draw_circle(Circle* c);

/**
 *  @brief Draws a menu circle in the screen.
 *  @param c Menu Circle to draw.
 */
void draw_menu_circle(Circle * c);

/**
 *  @brief Erases a circle from the screen.
 *  @param c Circle to erase.
 */
void erase_circle(Circle* c);

/**
 *  @brief Ticks a game circle.
 *
 *  Decreases a circle's time by one. When it reaches 0, it is destroyed.
 *
 *  @param c Game Circle to tick.
 *  @return Returns 1 after destroying the Circle and 0 otherwise.
 */
int tick_mainCircle(Circle* c);

/**
 *  @brief Ticks a menu circle.
 *
 *  Decreases a circle's radius by one. When it reaches 0, it is destroyed.
 *
 *  @param c Game Circle to tick.
 *  @return Returns 1 after destroying the Circle and 0 otherwise.
 */
int tick_menu_circle(Circle * c);

/**
 *  @brief Ticks a game circle's outer radius.
 *
 *  Decreases a circle's outer radius by one and re-applies the circle's letter. When the circle's time reaches 0, it does nothing.
 *
 *  @param c Game Circle to tick..
 */
void tick_outerCircle(Circle* c);

/**
 *  @brief Ticks a end circle.
 *
 *  Decreases an end circle's time by one. When it reaches 0, it is destroyed.
 *
 *  @param c End Circle to tick.
 *  @return Returns 1 after destroying the Circle and 0 otherwise.
 *
 */
int tick_endCircle(Circle* c);

/**
 *  @brief Destroy and free the memory allocated to the circle.
 *  @param c Circle to destroy.
 */
void destroy_circle(Circle* c);

/** @} end of circle */

#endif
