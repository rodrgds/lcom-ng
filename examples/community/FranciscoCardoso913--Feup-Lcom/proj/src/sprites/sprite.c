#include "sprite.h"

Sprite* create_sprite(xpm_map_t pic, uint16_t x, uint16_t y, uint16_t xspeed, uint16_t yspeed) {
    Sprite* sp = (Sprite*)malloc(sizeof(Sprite));

    if (sp == NULL) return NULL;

    xpm_image_t img;
    uint32_t* colormap = (uint32_t*)xpm_load(pic, XPM_8_8_8_8, &img);

    if (colormap == NULL) {
        free(sp);
        return NULL;
    }

    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->width = img.width;
    sp->height = img.height;
    sp->map = colormap;

    return sp;
}

int load_sprites() {
    background = create_sprite((xpm_map_t)background_xpm, 0, 0, 0, 0);
    if(!background){
        printf("Failed to load background.xpm\n");
        return 1;
    }
    cursor = create_sprite((xpm_map_t)cursor_xpm, 0, 0, 0, 0);
     if(!cursor){
        printf("Failed to load cursor.xpm\n");
        return 1;
    }
    logo = create_sprite((xpm_map_t)logo_xpm, 0, 100, 0, 0);
     if(!logo){
        printf("Failed to load logo.xpm\n");
        return 1;
    }
    single_player_text = create_sprite((xpm_map_t)single_player_xpm, 420, 400, 0, 0);
     if(!single_player_text){
        printf("Failed to load single_player.xpm\n");
        return 1;
    }
    single_player_texth = create_sprite((xpm_map_t)single_playerh_xpm, 430, 400, 0, 0);
     if(!single_player_texth){
        printf("Failed to load single_playerh.xpm\n");
        return 1;
    }
    coop_text = create_sprite((xpm_map_t)coop_xpm, 490, 470, 0, 0);
     if(!coop_text){
        printf("Failed to load coop.xpm\n");
        return 1;
    }
    coop_texth = create_sprite((xpm_map_t)cooph_xpm, 490, 470, 0, 0);
     if(!coop_texth){
        printf("Failed to load cooph.xpm\n");
        return 1;
    }
    exit_text = create_sprite((xpm_map_t)exit_xpm, 500, 560, 0, 0);
     if(!exit_text){
        printf("Failed to load exit.xpm\n");
        return 1;
    }
    exit_texth = create_sprite((xpm_map_t)exith_xpm, 500, 560, 0, 0);
     if(!exit_texth){
        printf("Failed to load exith.xpm\n");
        return 1;
    }
    game_over = create_sprite((xpm_map_t)game_over_xpm, 0, 100, 0, 0);
     if(!game_over){
        printf("Failed to load game_over.xpm\n");
        return 1;
    }
    pause_menu = create_sprite((xpm_map_t)pause_xpm, 0, 100, 0, 0);
     if(!pause_menu){
        printf("Failed to load pause.xpm\n");
        return 1;
    }
     waiting = create_sprite((xpm_map_t)waiting_xpm, 0, 100, 0, 0);
     if(!waiting){
        printf("Failed to load waiting.xpm\n");
        return 1;
    }
    return 0;
    
}

int destroy_sprite(Sprite* sp) {
    if (!sp) return 1;

    if (sp->map) free(sp->map);

    free(sp);
    sp = NULL;

    return 0;
}

int draw_sprite(Sprite* sp) {
    if (!sp) return 1;

    for (int h = 0; h < sp->height; h++) {
        for (int w = 0; w < sp->width; w++) {
            uint32_t pixel = sp->map[h * sp->width + w];
            if (pixel == xpm_transparency_color(XPM_8_8_8_8)) continue;
            video_draw_pixel(sp->x + w, sp->y + h, pixel);
        }
    }

    return 0;
}

int erase_sprite(Sprite* sp) {
    if (!sp) return 1;

    for (int h = 0; h < sp->height; h++) {
        for (int w = 0; w < sp->width; w++) {
            uint32_t pixel = sp->map[h * sp->width + w];
            if (pixel == xpm_transparency_color(XPM_8_8_8_8)) continue;
            replace_with_background(sp->x + w, sp->y + h);
        }
    }

    return 0;
}

int move_sprite(Sprite* sp) {
    if (!sp) return 1;

    sp->x += sp->xspeed;
    sp->y += sp->yspeed;

    return 0;
}

int animate_sprite(Sprite* sp) {
    if (!sp) return 1;

    if (move_sprite(sp)) return 1;

    if (draw_sprite(sp)) return 1;

    return 0;
}
