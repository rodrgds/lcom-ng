#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"
#include "constants.h"

#include "../../assets/maps/day_map.xpm"

static AnimSprite map_sprite;

int load_map_sprite() {
    return sprite_load_single(&map_sprite, (xpm_map_t) day_map);
}

void free_map_sprite() {
    sprite_free_frames(&map_sprite);
}

AnimSprite* get_map_sprite() {
    return &map_sprite;
}
