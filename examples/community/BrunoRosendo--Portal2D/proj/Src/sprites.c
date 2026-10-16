#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sprites.h"
#include "xpm_files.h"

Sprite *create_sprite(xpm_map_t pic, int x, int y, int xspeed, int yspeed){
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;
    if( sp == NULL ){
        printf("malloc failed\n");
        return NULL;
    }
    // read the sprite pixmap
    sp->map = xpm_load(pic, get_type(), &img);
    if( sp->map == NULL ) {
        free(sp);
        printf("xpm_load failed\n");
        return NULL;
    }
    sp->width = img.width;
    sp->height = img.height;
    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->map_reversed = NULL;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
        return;
    if(sp->map_reversed != sp->map)
        free(sp->map_reversed);
    free(sp->map);
    free(sp);
    sp = NULL;
}

void draw_sprite(Sprite* sp){
    size_t i = 0;
    for (uint16_t y = sp->y; y < sp->y + sp->height; ++y){
        for (uint16_t x = sp->x; x < sp->x + sp->width; ++x){
            uint16_t color = ((uint16_t*) (sp->map))[i];
            i++;
            if (color != xpm_transparency_color(get_type()))
                vg_draw_pixel(x, y, color);
        }
    }
}

static Sprite cursor_render_copy(Sprite *cursor){
    // Drawing and restoring must use the same inset without changing portal aim.
    Sprite render = *cursor;
    int max_x = (int)get_h_res() > render.width ? get_h_res() - render.width : 0;
    int max_y = (int)get_v_res() > render.height ? get_v_res() - render.height : 0;
    render.x = render.x < 0 ? 0 : render.x > max_x ? max_x : render.x;
    render.y = render.y < 0 ? 0 : render.y > max_y ? max_y : render.y;
    return render;
}

void draw_cursor(Sprite *cursor){
    Sprite render = cursor_render_copy(cursor);
    draw_sprite(&render);
}

void erase_cursor(Sprite *cursor){
    Sprite render = cursor_render_copy(cursor);
    erase_sprite(&render);
}

void erase_sprite(Sprite* sp){
    size_t i = 0;
    for (uint16_t y = sp->y; y < sp->y + sp->height; ++y){
        for (uint16_t x = sp->x; x < sp->x + sp->width; ++x){
            uint16_t color = ((uint16_t*) (sp->map))[i];
            i++;
            if (color != xpm_transparency_color(get_type())){
                vg_draw_from_background(x, y);
            }
        }
    }
}

void animate_sprite(Sprite* sp){
    sp->x += sp->xspeed;
    sp->y += sp->yspeed;
    draw_sprite(sp);
}

int flip_sprite(Sprite* sp){
    if (sp->map_reversed == NULL){
        printf("Sprite is not reversable!\n");
        return 1;
    }
    uint8_t* temp;
    temp = sp->map;
    sp->map = sp->map_reversed;
    sp->map_reversed = temp;
    return 0;
}
