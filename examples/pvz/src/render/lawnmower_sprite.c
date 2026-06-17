#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"

#include "../../assets/ui/lawnmower.xpm"

static AnimSprite lawnmower_sprite;

AnimSprite* get_lawnmower_sprite() {
    return &lawnmower_sprite;
}

int load_lawnmower_sprite() {
    return sprite_load_single(&lawnmower_sprite, (xpm_map_t) lawnmower_xpm);
}

void free_lawnmower_sprite() {
    sprite_free_frames(&lawnmower_sprite);
}
