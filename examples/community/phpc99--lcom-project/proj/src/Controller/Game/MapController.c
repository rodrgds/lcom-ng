#include "MapController.h"
#include <math.h>

/**
 * @file MapController.c
 * @brief Implements all logic for controlling the game map, including elements, player, background, and animation.
 *
 * Handles element addition/removal, tree growth, background transitions sunrise/day/sunset/night,
 * cloud movement, and solar/lunar animation using polar coordinates.
 */

#define PI 3.14159265
#define RADIUS ((mode_info.XResolution / 2)+(mode_info.XResolution/12))
#define CENTER_X ((mode_info.XResolution / 2)- 65)
#define CENTER_Y (mode_info.YResolution -( mode_info.YResolution / 10))
#define CLOUD_WIDTH 220
#define CLOUD2_WIDTH 104

BG bg = BG_RISE;

static bool CollisionTop(Position p, int16_t Pw, int16_t Ph, Position o, int16_t Ow, int16_t Oh){
    if(p.y == o.y + Oh) {
        if (p.x < o.x + Ow && p.x + Pw > o.x) {
            return true;
        }
    }

    return false;
}

void MapInit(){
    mc = new_MapController();
    check_up(mc);
}

Player* get_player(){
    return mc->map->player;
}

MapController *get_MapController(){
    return mc;
}

MapController *new_MapController(void){
    MapController *mc = malloc(sizeof(*mc));
    if( mc == NULL){
        return NULL;
    }

    mc->map = new_Map();
    return mc;
}
void delete_MapController(){
    delete_Map(mc->map);
    free(mc);
}

void add(MapController *mc, ElementType type , Position position, int16_t width, int16_t height, bool block_up){
    size_t last = mc->map->count_elements;
    mc->map->count_elements++;
    mc->map->elements = realloc(mc->map->elements, mc->map->count_elements * sizeof(*mc->map->elements));
    mc->map->elements[last] = new_Element(type, position, width, height, block_up);
}

bool delete(MapController *mc, size_t idx){
    if (idx >= mc->map->count_elements) {
        return false;
    }

    if(mc->map->elements[idx]->block_up == true || mc->map->elements[idx]->type == ELEMENT_BEDROCK) {
        return false;
    }

    delete_Element(mc->map->elements[idx]);

    for (size_t i = idx; i < mc->map->count_elements - 1; i++) {
        mc->map->elements[i] = mc->map->elements[i + 1];
    }

    mc->map->count_elements--;
    mc->map->elements = realloc(mc->map->elements, mc->map->count_elements * sizeof(*mc->map->elements));

    return true;
}

void check_up(MapController *mc){
    size_t count = mc->map->count_elements;

    for(size_t i = 0; i< count; i++){
        mc->map->elements[i]->block_up = false;
    }


    for(size_t i = 0; i< count -1; i++){
        Element *e1 = mc->map->elements[i];

        for(size_t j = 0; j< count ; j++ ){
            if(j!= i){
                Element *e2 = mc->map->elements[j];
                if(CollisionTop(e1->position, e1->width, e1->height, e2->position, e2->width, e2->height)){
                    e1->block_up = true;
                }
            }
        }
    }
}

void update_Trees(MapController *mc) {

    for(size_t i = 0; i < mc->map->count_elements; i++) {
        Element *e = mc->map->elements[i];
        if (e->type == ELEMENT_TREE) {
            e->data.tree.tree_age++;
            if (e->data.tree.growth_stage == TREE_SAPPLING && e->data.tree.tree_age >= 5) {
                e->data.tree.growth_stage = TREE_MEDIUM;
                e->height = 120;
                e->position.y -= 88;
            } else if (e->data.tree.growth_stage == TREE_MEDIUM && e->data.tree.tree_age >= 10) {
                e->data.tree.growth_stage = TREE_FINAL_STAGE;
                e->height = 179;
                e->position.y -= 59;
            }
        }
    }

}

void update_map(){
    static int Hours = 0;
    static int cloud_direction = 1; // 1 for right, -1 for left
    static int cloud2_direction = -1; // 1 for right, -1 for left
    static int cloud_speed = 20;
    static int cloud2_speed = 20;
    float angle = (2 * PI * Hours) / 480.0; // 480 steps para um ciclo completo

    mc->map->sun.position.x = CENTER_X - RADIUS * cos(angle);
    mc->map->sun.position.y = CENTER_Y - RADIUS * sin(angle);

    mc->map->moon.position.x = CENTER_X - RADIUS * cos(angle + PI);
    mc->map->moon.position.y = CENTER_Y - RADIUS * sin(angle + PI);

    mc->map->cloud.position.x += cloud_speed * cloud_direction;
    mc->map->cloud2.position.x += cloud2_speed * cloud2_direction;
    // Inverter direção ao bater nas bordas
    if (mc->map->cloud.position.x <= 0 || mc->map->cloud.position.x + CLOUD_WIDTH >= mode_info.XResolution) {
        cloud_direction *= -1;
    }
    if (mc->map->cloud2.position.x <= 0 || mc->map->cloud2.position.x + CLOUD2_WIDTH >= mode_info.XResolution) {
        cloud2_direction *= -1;
    }

    Hours++;
    if (Hours < 40){
        bg = BG_RISE;
    }
    else if (Hours<220){
        bg=BG_DAY;
    }
    else if (Hours<260){
        bg=BG_SET;
    }
    else if (Hours<480){
        bg=BG_NIGHT;
    }
    if(Hours >= 480){ 
        Hours = 0;
    }
}
