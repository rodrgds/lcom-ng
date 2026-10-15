#include "Room.h"
#include "../utils/utils.h"
#include "../drivers/utils_drivers.h"
#include <math.h>
#include "CatInfo.h"

Room* newRoom(){
    Room *room = (Room*) malloc(sizeof(Room));
    for (int i = 0; i < 4; i++) room->walls[i] = NULL;
    room->player = newPlayer(100, 100);
    room->center = newPosition(512, 371);
    room->n_cats = 0;

    for (int i = 0; i < 10; i++){
        if (addCat(room) != 0) return NULL;
    }

    int cloud_x_offset = 0;
    for (int i = 0; i < CLOUD_QNT; i++) {
        Cloud cloud = {
          .x = cloud_x_offset,
          .y = 80 + getRandomInt(-50, 30),
          .velocity = getRandomFloat(1.3, 1.7),
        };
        room->clouds[i] = cloud;
        cloud_x_offset += getRandomInt(50, 300);
    }


    return room;
}




void deleteRoom(Room* room) {
    deleteEntity(room->player);

    
    for (int i = 0; i < 10; i++) {
        if (room->cats[i] == NULL) continue;
        deleteEntity(room->cats[i]);
    }

    
    for (int i = 0; i < 4; i++) {
        if (room->walls[i] == NULL) continue;
        deleteEntity(room->walls[i]);
    }


    free(room);
}

Entity* getSelectedCat(Room* room) {
    
    Entity* currCat;
    Entity* closestCat = NULL;
    double distToPlayer;
    double minDistToPlayer = SELECT_RADIUS;
    
    for (int i = 0; i < 10; i++) {
        currCat = room->cats[i];
        if (currCat == NULL) continue;

        ((CatInfo*)currCat->typeInfo)->isSelected = false;
        distToPlayer = sqrt(pow(room->player->position->x - currCat->position->x, 2)
                            + pow(room->player->position->y - currCat->position->y, 2));
        
        if (distToPlayer >= SELECT_RADIUS) continue;
        if (distToPlayer < minDistToPlayer) {
            minDistToPlayer = distToPlayer;
            closestCat = currCat;
        }
    }
    if (closestCat != NULL) ((CatInfo*)closestCat->typeInfo)->isSelected = true;
    return closestCat;
}


int addCat(Room* room){
    if (room == NULL) return 1;
    if (room->n_cats >= 10) return 0;

    while(true){
        int catX = randomNumer(30, 1000);
        int catY = randomNumer(300, 700);
        Position pos;
        pos.x = catX;
        pos.y = catY;
        Entity* cat = newCat(catX, catY);
        bool hasCollision = false;
        for (int i = 0; i < room->n_cats; i++){
            CollisionType col = checkCollision(pos, cat, room->cats[i]);
            if (col != NO_COLLISION){
                hasCollision = true;
                break;
            } 
        }
        if (!hasCollision){
            room->cats[room->n_cats] = newCat(catX, catY);
            room->n_cats++;
            break;
        }
    }
    return 0;
}
