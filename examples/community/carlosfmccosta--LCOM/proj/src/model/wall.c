/**
 * @file wall.c
 * @brief Implementation of wall entity management.
 *
 * This file contains the implementation of the functions declared in wall.h.
 */
#include "wall.h"
#include "../controller/video_gr/video_gr.h"

/**
 * @struct wall
 * @brief Structure representing a wall entity.
 *
 * Contains a pointer to the sprite used for the wall's visualization.
 */
struct wall {
    sprite_t *sprite; /**< Pointer to the sprite representing the wall */
};






wall_t* wall_init(int16_t start_x, int16_t start_y) {
    xpm_map_t my_xpm[1] = {wall_xpm};
    sprite_t* sp = create_sprite(my_xpm, 1, start_x, start_y);
    if (sp == NULL) { /* Error Creating Sprite */ 
        return NULL;
    }

    wall_t *wall = (wall_t*) malloc(sizeof(wall_t));
    if (wall == NULL) { /* Error Creating wall */ 
        destroy_sprite(sp);
        return NULL;
    }

    wall->sprite = sp;
    return wall;
}

void destroy_wall(wall_t *wall) {
    destroy_sprite(wall->sprite);
    free(wall);
    wall = NULL;
}


int wall_draw(const wall_t *wall) {
    if (draw_sprite(wall->sprite)){
        return 1;
    }
    return 0; 
}

//Getters
uint16_t get_wall_width(wall_t *wall) {
    return get_sprite_width(wall->sprite);
}

uint16_t get_wall_height(wall_t *wall) {
    return get_sprite_height(wall->sprite);
}

position_t * get_wall_pos(wall_t *wall) {
    return get_sprite_pos(wall->sprite);
}

sprite_t* get_wall_sprite(wall_t *wall) {
    return wall->sprite;
}

bool collides_with_any_wall(sprite_t* sp, wall_array_t* wall_arr, int *dx, int *dy) {
    for (int i = 0; i < wall_arr->count; ++i) {
        sprite_t* wall_sprite = get_wall_sprite(wall_arr->walls[i]);
        
        if (!sp || !wall_sprite) continue;

        position_t* pos1 = get_sprite_pos(sp);
        uint16_t w1 = get_sprite_width(sp);
        uint16_t h1 = get_sprite_height(sp);

        position_t* pos2 = get_sprite_pos(wall_sprite);
        uint16_t w2 = get_sprite_width(wall_sprite);
        uint16_t h2 = get_sprite_height(wall_sprite);

        if (pos1->x + w1 <= pos2->x || pos2->x + w2 <= pos1->x ||
            pos1->y + h1 <= pos2->y || pos2->y + h2 <= pos1->y) {
            continue;
        }

        if (dx) {
            int overlap_left = (pos1->x + w1) - pos2->x;      // sp overlaps wall from left
            int overlap_right = (pos2->x + w2) - pos1->x;     // sp overlaps wall from right
            *dx = (overlap_left < overlap_right) ? overlap_left : -overlap_right;
        }
        if (dy) {
            int overlap_top = (pos1->y + h1) - pos2->y;       // sp overlaps wall from top
            int overlap_bottom = (pos2->y + h2) - pos1->y;    // sp overlaps wall from bottom
            *dy = (overlap_top < overlap_bottom) ? overlap_top : -overlap_bottom;
        }
        if (dx && dy) {
            if (*dx < *dy) {
                *dx = 0;
            } else {
                *dy = 0;
            }
        }
        return true;
    }
    if (dx) *dx = 0;
    if (dy) *dy = 0;
    return false;
}


/**
 * @brief Array of strings representing the wall placement on the level.
 */
const char *level1[] = {
"######################",
"#                    #",
"#                    #",
"#    ####    ####    #",
"#    #          #    #",
"#    #          #    #",
"#                    #",
"#                    #",
"#                    #",
"#                    #",
"#    #          #    #",
"#    #          #    #",
"#    ####    ####    #",
"#                    #",
"#                    #",
"######################"
};
const int LEVEL1_ROWS = sizeof(level1) / sizeof(level1[0]);

wall_array_t load_walls_from_array(const char **level, int rows, int tile_width, int tile_height) {
    int max_walls = rows * strlen(level[0]);
    wall_t **walls = malloc(max_walls * sizeof(wall_t*));
    int wall_count = 0;
    int x_offset = tile_width / 2.66;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; level[y][x] != '\0'; ++x) {
            if (level[y][x] == '#') {
                wall_t *wall = wall_init((x * tile_width) - x_offset, y * tile_height);
                if (wall) walls[wall_count++] = wall;
            }
        }
    }
    return (wall_array_t){walls, wall_count};
}
