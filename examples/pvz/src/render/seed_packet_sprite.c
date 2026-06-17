#include "pvz_platform.h"
#include "sprite_loader.h"
#include "sprites.h"

#include "../../assets/ui/seed_packet_card.xpm"

static AnimSprite seed_packet_sprite;

AnimSprite* get_seed_packet_sprite() {
    return &seed_packet_sprite;
}

int load_seed_packet_sprite() {
    return sprite_load_single(&seed_packet_sprite, (xpm_map_t) seed_packet_card_xpm);
}

void free_seed_packet_sprite() {
    sprite_free_frames(&seed_packet_sprite);
}
