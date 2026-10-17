#include "minigotchi.h"

/**
 * @brief Creates a new minigotchi "object" and returns a pointer to it.
 */
Minigotchi *new_minigotchi(int x, int y, uint32_t hunger, uint32_t happiness, char* name) {
    Minigotchi *minigotchi = malloc(sizeof(*minigotchi));
    if (minigotchi == NULL) return NULL;

    minigotchi->sprite = guiDrawer_get_minigotchi_sprite();
    minigotchi->x = x;
    minigotchi->y = y; 
    minigotchi->hunger = hunger;
    minigotchi->happiness = happiness;
    minigotchi->name = name;
    minigotchi->isRxCuddles = false;
    return minigotchi;
}

/**
 * @brief Minigotchi destructor
 */
void kill_minigotchi(Minigotchi *minigotchi) {
    free(minigotchi);
}

/**
 * @brief Draws a minigotchi 
 */
void draw_minigotchi(Minigotchi *minigotchi) {
    drawSprite(minigotchi->sprite, minigotchi->x, minigotchi->y);
}

// Getters and setters
Sprite* minigotchi_get_sprite(Minigotchi *minigotchi) {
    return minigotchi->sprite;
}

void minigotchi_set_sprite(Minigotchi *minigotchi, Sprite* sprite) {
    minigotchi->sprite = sprite;
}

int minigotchi_get_x(Minigotchi *minigotchi) {
    return minigotchi->x;
}

int minigotchi_get_y(Minigotchi *minigotchi) {
    return minigotchi->y;
}


void minigotchi_set_x(Minigotchi *minigotchi, int x) {
    minigotchi->x = x;
}

void minigotchi_set_y(Minigotchi *minigotchi, int y) {
    minigotchi->y = y;
}

uint32_t minigotchi_get_happiness(Minigotchi *minigotchi) {
    return minigotchi->happiness;
}

uint32_t minigotchi_get_hunger(Minigotchi *minigotchi) {
    return minigotchi->hunger;
}

void minigotchi_set_happiness(Minigotchi *minigotchi, uint32_t value){
    minigotchi->happiness = value;
}

void minigotchi_set_hunger(Minigotchi *minigotchi, uint32_t value){
    minigotchi->hunger = value;
}

char* minigotchi_get_name(Minigotchi *minigotchi) {
    return minigotchi->name;
}

void minigotchi_set_name(Minigotchi *minigotchi, char* name) {
    minigotchi->name = name;
}

bool minigotchi_get_cuddles(Minigotchi *minigotchi) {
    return minigotchi->isRxCuddles;
}

void minigotchi_set_cuddles(Minigotchi *minigotchi, bool cuddles) {
    minigotchi->isRxCuddles = cuddles;
}
