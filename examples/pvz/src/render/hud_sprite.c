#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"

#include "../../assets/ui/hud.xpm"

static AnimSprite hud_sprite;

int load_hud_sprite() {
    return sprite_load_single(&hud_sprite, (xpm_map_t) hud_xpm);
}

void free_hud_sprite() {
    sprite_free_frames(&hud_sprite);
}

AnimSprite* get_hud_sprite() {
    return &hud_sprite;
}
