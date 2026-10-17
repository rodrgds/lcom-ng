#include "src/Model/Map/Map.h"

/**
 * @brief Calculates the number of blocks that can be placed on the map.
 *
 * Based on the screen resolution and the width of a block (assumed to be 32px).
 * The result is scaled to fill three vertical layers.
 *
 * @return Total number of block elements.
 */

int Number_block(){

    int n = 3*(mode_info.XResolution/32);

    return n;
}

/**
 * @brief Creates and initializes a new Map structure.
 *
 * Sets up the game map by allocating memory for elements and initializing
 * objects like the player, sun, moon, and clouds. It also populates the map with different
 * types of terrain and decorative elements (e.g., trees).
 *
 * @return Pointer to the newly created Map, or NULL if allocation fails.
 */

Map *new_Map(void){
    int count = Number_block();
    Map *m = malloc(sizeof(*m));

    if( m == NULL){
        return NULL;
    }
    m->count_elements = count + 3;
    m->elements = malloc(m->count_elements * sizeof(*m->elements));

    Position p;
    p.x = mode_info.XResolution / 2 + 64;
    p.y= 600;
    bool on_ground = false;

    m->player = new_Player(p, on_ground);
    if (!m->elements){
        free(m);
        return NULL;
    }
    m->sun = *new_Sun((Position){.x = mode_info.XResolution+120, .y = mode_info.XResolution+121}, 120, 121);
    m->moon = *new_Moon((Position){.x = mode_info.XResolution+130, .y = mode_info.XResolution+130}, 130, 130);
    m->cloud = *new_Cloud((Position){.x = 15, .y = 15}, 220, 105);
    m->cloud2 = *new_Cloud((Position){.x =mode_info.XResolution - 240 , .y = 30}, 104, 104);

    int idx = 0;
    for(int y = mode_info.YResolution - 32; y > mode_info.YResolution - 32*2; y -= 32){
        for(int x = 0; x < mode_info.XResolution ; x+= 32 ){
            Position p = {.x = x, .y = y};
            m->elements[idx++] = new_Element(ELEMENT_BEDROCK, p, 32, 32, false);
        }
    }

    for(int y = mode_info.YResolution - 32*2; y >= mode_info.YResolution - 32*3;y -= 32){
        for(int x = 0; x < mode_info.XResolution ; x+= 32 ){
            Position p = {.x = x, .y = y};
            m->elements[idx++] = new_Element(ELEMENT_EARTH, p, 32, 32, false);
        }
    }

    m->elements[idx++] = new_Element(ELEMENT_TREE, (Position){.x = 0, .y = mode_info.YResolution - 32*3 - 32}, 64, 32, false);
    m->elements[idx++] = new_Element(ELEMENT_TREE , (Position){.x = mode_info.XResolution - 64, .y = mode_info.YResolution - 32*3 - 32}, 64, 32, false);  
    m->elements[idx++] = new_Element(ELEMENT_TREE ,(Position){.x = mode_info.XResolution / 2 , .y = mode_info.YResolution - 32*3 - 32}, 64, 32, false);
    return m;
}

/**
 * @brief Destroys a Map instance and frees its associated memory.
 *
 * Frees the player, each element in the map, the elements array,
 * and finally the map itself.
 *
 * @param m Pointer to the Map to be deleted.
 */

void delete_Map(Map *m){
    if (!m) return;
    delete_Player(m->player);
    for(size_t i = 0; i < m->count_elements; i++) {
        delete_Element(m->elements[i]);
    }
    free(m->elements);
    free(m);
}
