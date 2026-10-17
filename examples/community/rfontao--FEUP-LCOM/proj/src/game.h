#ifndef _GAME_H_
#define _GAME_H_

#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "kbc.h"
#include "vbe.h"
#include "vbe_macros.h"
#include "sprite.h"
#include "bullet.h"
#include "collisions.h"
#include "enemy.h"
#include "rtc.h"
#include "serial_port.h"
#include "queue.h"

#include "resources/carro1.xpm"
#include "resources/carro2.xpm"
#include "resources/carro3.xpm"
#include "resources/carro4.xpm"
#include "resources/carro5.xpm"
#include "resources/carro6.xpm"
#include "resources/carro7.xpm"
#include "resources/carro8.xpm"
#include "resources/carro12.xpm"
#include "resources/carro23.xpm"
#include "resources/carro34.xpm"
#include "resources/carro45.xpm"
#include "resources/carro56.xpm"
#include "resources/carro67.xpm"
#include "resources/carro78.xpm"
#include "resources/carro81.xpm"

#include "resources/arma1.xpm"
#include "resources/arma2.xpm"
#include "resources/arma3.xpm"
#include "resources/arma4.xpm"
#include "resources/arma5.xpm"
#include "resources/arma6.xpm"
#include "resources/arma7.xpm"
#include "resources/arma8.xpm"
#include "resources/arma12.xpm"
#include "resources/arma23.xpm"
#include "resources/arma34.xpm"
#include "resources/arma45.xpm"
#include "resources/arma56.xpm"
#include "resources/arma67.xpm"
#include "resources/arma78.xpm"
#include "resources/arma81.xpm"

#include "resources/enemy1.xpm"
#include "resources/enemy2.xpm"
#include "resources/enemy3.xpm"
#include "resources/enemy4.xpm"
#include "resources/enemy5.xpm"
#include "resources/enemy6.xpm"
#include "resources/enemy7.xpm"
#include "resources/enemy8.xpm"
#include "resources/enemy12.xpm"
#include "resources/enemy23.xpm"
#include "resources/enemy34.xpm"
#include "resources/enemy45.xpm"
#include "resources/enemy56.xpm"
#include "resources/enemy67.xpm"
#include "resources/enemy78.xpm"
#include "resources/enemy81.xpm"

#include "resources/digit0.xpm"
#include "resources/digit1.xpm"
#include "resources/digit2.xpm"
#include "resources/digit3.xpm"
#include "resources/digit4.xpm"
#include "resources/digit5.xpm"
#include "resources/digit6.xpm"
#include "resources/digit7.xpm"
#include "resources/digit8.xpm"
#include "resources/digit9.xpm"
#include "resources/dots.xpm"

#include "resources/background.xpm"
#include "resources/backMenu.xpm"
#include "resources/TestBack.xpm"
#include "resources/backgroundFinal.xpm"
#include "resources/crosshair.xpm"
#include "resources/start1.xpm"
#include "resources/start2.xpm"
#include "resources/title.xpm"
#include "resources/obj.xpm"
#include "resources/carBar.xpm"
#include "resources/carHpFill.xpm"
#include "resources/objectiveBar.xpm"
#include "resources/objectiveHpFill.xpm"
#include "resources/gameOverScreen.xpm"
#include "resources/roundSp.xpm"
#include "resources/enemyBar.xpm"
#include "resources/scoreSp.xpm"
#include "resources/highSp.xpm"
#include "resources/startMenu.xpm"
#include "resources/multButton.xpm"
#include "resources/multButton2.xpm"

/** @defgroup Game Game
 * @{
 *
 * Constants and the main game loop
 */

#define FIXED_ROUND_TIME        90      /**< @brief Time for each round in seconds*/
#define user_interface_x        50
#define user_interface_y        50
#define objective_x             900
#define objective_y             80

#define CAR_HP                  100     /**< @brief Health points of the car*/
#define OBJECTIVE_HP            100     /**< @brief Health points of the objective*/
#define CAR_GUN_COOLDOWN        20      /**< @brief Cooldown between car shots*/
#define ENEMY_SPAWN_COOLDOWN    120     /**< @brief Cooldown between enemy spawns*/
#define CAR_MAX_SPEED           5

/**
 * @brief Main game loop (singleplayer)
 * 
 * @param mode          Video mode
 * @return int          1 on failure
 */
int game_loop(uint16_t mode);
/**
 * @brief Main game loop (multiplayer)
 * 
 * @param mode          Video mode
 * @param player        Player number (1 or 2)
 * @return int          1 on failure
 */
int game_loop_multi(uint16_t mode, unsigned int player);

/**@}*/

#endif
