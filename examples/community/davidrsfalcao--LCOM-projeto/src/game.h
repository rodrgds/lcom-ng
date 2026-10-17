#ifndef __GAME_H
#define __GAME_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/sysutil.h>
#include <minix/com.h>
#include <math.h>
#include <stdint.h>

#include "vbe.h"
#include "video_gr.h"
#include "lmlib.h"
#include "timer.h"
#include "i8254.h"
#include "imgs.h"
#include "space_ship.h"
#include "asteroid.h"
#include "collisions.h"
#include "game_over.h"
#include "numbers.h"

/**
 * @brief the game
 * @return 0 on success or non-zero otherwise
 */
int game();
/**
 * @brief draws the objects that will be in the game
 *
 * @param nv1 spaceship
 * @param ast1 one of the asteroids
 * @param ast2 one of the asteroids
 */
void draw(space_ship nv1, asteroid ast1, asteroid ast2);
/**
 * @brief shows the player's pontuation updated on every frame
 *
 */
void pontuation_atual();

#endif



