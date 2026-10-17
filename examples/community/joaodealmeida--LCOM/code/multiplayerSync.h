#ifndef __multiplayerSync_H
#define __multiplayerSync_H

#include "GameController.h"

/** @defgroup multiplayerSync multiplayerSync
 * @{*/

#define USER_TYPE_NONE 0
#define USER_TYPE_HOST 1
#define USER_TYPE_CLIENT 2
#define MULTIPLAYER_SYNC_RECEIVE 0
#define MULTIPLAYER_SYNC_SEND 1

/**
 * param user_type define user as host or client
 * param game_p (be CAREFULF here) pointer to gamePointer wich will point to game when it is created
 */
void multiplayer_defineUserType(unsigned char user_type, GameController** game_p);

char multiplayer_getUserType();///<returns  usertype (host or client)

/**
 * @brief sync the seed given to be used by ran() func
 * host sends seed and client receives it
 * @return 0 if succeeds, something else value if failed
 */
char multiplayer_syncSeed(unsigned int *seed);

char multiplayer_update();///<receive and send game info via uart. process received info.

void multiplayer_reset_receivedInfo();

/**
 * help sync games start. could be very different depending on how much time the gameInit takes for each user
 * \nclient send start signal to host
 * return 0 when sync is over
 */
char multiplayer_syncGameInit();

/** @} end of multiplayerSync */

#endif //__multiplayerSync_H
