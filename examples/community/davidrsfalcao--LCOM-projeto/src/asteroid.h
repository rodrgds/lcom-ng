#ifndef __ASTEROID_H
#define __ASTEROID_H


typedef struct asteroid
{
	unsigned short x;
	unsigned short y;
	int height;
	int width;
	char **sprite;

}asteroid;
/**
 * @brief draw the asteroids
 * @param ast1 one of the asteroids to be draw
 * @param ast2 one of the asteroids to be draw
 *
 */
void draw_asteroids(asteroid ast1, asteroid ast2);
/**
 * @brief move the asteroid
 *
 * @param the asteroid that will be moved
 */
void move_asteroid(asteroid *ast);
/**
 * @brief generates the asteroids,so they appear on the screen
 * @param ast1 one of the asteroids that will be generated
 * @param ast2 one of the asteroids that will be generated
 */
void asteroid_generator(asteroid *ast1, asteroid *ast2);
/**
 * @brief draw the atual score
 *
 */
void pontuation_atual();


#endif
