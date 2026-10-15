/**
 * @file Room.h
 * @brief This file contains functions regarding the removal, addition and existence of entities 
 * @date 2024-06-02
 */
#ifndef ROOM_H
#define ROOM_H


#include "../models/Entity.h"
#include "../models/Cloud.h"

#define CLOUD_QNT 5

/**
 * @brief Room struct
 * 
 */
typedef struct {
    Entity* player;
    Entity* cats[10];
    Entity* walls[4];
    Position* center;
    Cloud clouds[CLOUD_QNT];
    uint8_t stage;
    uint8_t n_cats;
} Room;

/**
 * @brief Creates a new room
 * 
 * @return pointer to created room 
 */
Room* newRoom();
/**
 * @brief Deletes a previously created room
 * 
 * @param room 
 */
void deleteRoom(Room* room);
/**
 * @brief Adds a cat to chosen room
 * 
 * @param room 
 * @return int 
 */
int addCat(Room* room);   //called after a cooldown; cooldown is shorter in higher stages
/**
 * @brief Removes chosen cat in chosen room
 * 
 * @param room 
 * @param cat 
 * @return int 
 */
int removeCat(Room* room ,Entity* cat);   //called if the specified cat is pet correctly
/**
 * @brief Increases game difficulty
 * 
 * @param room 
 * @return int 
 */
int advanceStage(Room* room);
/**
 * @brief Returns the cat selected by the player
 * 
 * @param room 
 * @return Entity* 
 */
Entity* getSelectedCat(Room* room);

#endif /* ROOM_H */
