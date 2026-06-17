#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"

#include "../../assets/ui/shovel.xpm"
#include "../../assets/ui/only_shovel.xpm"

static AnimSprite shovel_sprite;
static AnimSprite only_shovel_sprite;

AnimSprite* get_shovel_sprite() {
    return &shovel_sprite;
}

AnimSprite* get_only_shovel_sprite() {
    return &only_shovel_sprite;
}

int load_shovel_sprite() {
    if (sprite_load_single(&shovel_sprite, (xpm_map_t) shovel_xpm) != 0) {
        return 1;
    }

    if (sprite_load_single(&only_shovel_sprite, (xpm_map_t) only_shovel_xpm) != 0) {
        free_shovel_sprite();
        return 1;
    }

    return 0;
}

void free_shovel_sprite() {
    sprite_free_frames(&shovel_sprite);
    sprite_free_frames(&only_shovel_sprite);
}
