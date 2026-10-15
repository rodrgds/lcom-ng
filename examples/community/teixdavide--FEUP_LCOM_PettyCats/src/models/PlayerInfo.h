/**
 * @file PlayerInfo.h
 * @brief This file contains a small struct with player info
 * @date 2024-06-02
 */
#ifndef _PLAYERINFO_H_
#define _PLAYERINFO_H_

#include <stdbool.h>


/**
 * @brief PlayerInfo struct
 * 
 */
typedef struct {
    bool isPetting;
} PlayerInfo;

PlayerInfo* newPlayerInfo();
void deletePlayerInfo(PlayerInfo* playerInfo);

#endif
