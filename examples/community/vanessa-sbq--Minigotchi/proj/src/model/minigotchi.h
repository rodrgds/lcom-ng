#ifndef _MINIGOTCHI_H_
#define _MINIGOTCHI_H_

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int x; 
    int y;
    uint32_t happiness;
    uint32_t hunger;
    bool isRxCuddles;
    char* name;
} Minigotchi;

Minigotchi *new_minigotchi(int x, int y, uint32_t hunger, uint32_t happiness, char* name);

void kill_minigotchi(Minigotchi *minigotchi);

void draw_minigotchi(Minigotchi *minigotchi);

// Getters and setters
Sprite* minigotchi_get_sprite(Minigotchi *minigotchi);
void minigotchi_set_sprite(Minigotchi *minigotchi, Sprite* sprite);

int minigotchi_get_x(Minigotchi *minigotchi);
int minigotchi_get_y(Minigotchi *minigotchi);

void minigotchi_set_x(Minigotchi *minigotchi, int x);
void minigotchi_set_y(Minigotchi *minigotchi, int y);

uint32_t minigotchi_get_happiness(Minigotchi *minigotchi);
uint32_t minigotchi_get_hunger(Minigotchi *minigotchi);

void minigotchi_set_happiness(Minigotchi *minigotchi, uint32_t value);
void minigotchi_set_hunger(Minigotchi *minigotchi, uint32_t value);

char* minigotchi_get_name(Minigotchi *minigotchi);
void minigotchi_set_name(Minigotchi *minigotchi, char* name);

bool minigotchi_get_cuddles(Minigotchi *minigotchi);
void minigotchi_set_cuddles(Minigotchi *minigotchi, bool cuddles);

#endif /* _MINIGOTCHI_H_ */
