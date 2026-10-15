

#include <lcom/lcf.h>
#include "sprite.h"

extern vbe_mode_info_t info;
extern uint16_t mode;



Sprite *create_sprite(xpm_map_t pic) {
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;
    
    if( sp == NULL )
    return NULL;
    sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }
    sp->width = img.width; 
    sp->height=img.height;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
        return;
    if( sp ->map )
    free(sp->map);
    free(sp);
    sp = NULL; 
}



int draw_sprite(Sprite *sp, int x, int y) {
    uint16_t width = sp->width;
    uint16_t height = sp->height;
    for (int w = 0 ; w < width ; w++) {
        for (int h = 0 ; h < height ; h++) {
            if (vg_draw_pixel(x + w, y + h, sp->map[w + h*width])) return 1;
        }
    }
    return 0; 
}

int draw_reverse_sprite(Sprite *sp, int x, int y) {
    uint16_t width = sp->width;
    uint16_t height = sp->height;
    for (int w = 0 ; w < width ; w++) {
        for (int h = 0 ; h < height ; h++) {
            if (vg_draw_pixel(x + (width - w), y + h, sp->map[w + h*width])) return 1;
        }
    }
    return 0; 
}

void loadInitialSprites(){
    mouse_cursor= create_sprite((xpm_map_t) square_110);
    ResChooseBckgrd = create_sprite((xpm_map_t) ResChooseBckgrnd);

}
void freeInitialSprites(){
    destroy_sprite(mouse_cursor);
    destroy_sprite(ResChooseBckgrd);
}


void loadAllSprites(uint16_t mode){

    
    
    if(mode==0x14C) {
        cuphead1 = create_sprite((xpm_map_t) cuphead_1);
        cuphead2 = create_sprite((xpm_map_t) cuphead_2);
        cuphead3 = create_sprite((xpm_map_t) cuphead_3);
        cuphead4 = create_sprite((xpm_map_t) cuphead_4);
        cuphead5 = create_sprite((xpm_map_t) cuphead_5);
        cuphead6 = create_sprite((xpm_map_t) cuphead_6);
        cuphead7 = create_sprite((xpm_map_t) cuphead_7);
        cuphead8 = create_sprite((xpm_map_t) cuphead_8);
        cuphead9 = create_sprite((xpm_map_t) cuphead_9);
        cuphead10 = create_sprite((xpm_map_t) cuphead_10);
        cuphead11 = create_sprite((xpm_map_t) cuphead_11);
        cuphead12 = create_sprite((xpm_map_t) cuphead_12);
        cuphead13 = create_sprite((xpm_map_t) cuphead_13);
        cuphead14 = create_sprite((xpm_map_t) cuphead_14);
        cuphead15 = create_sprite((xpm_map_t) cuphead_15);
        cuphead16 = create_sprite((xpm_map_t) cuphead_16);
        cupheadstand = create_sprite((xpm_map_t) cuphead_stand);
        hearthLife = create_sprite((xpm_map_t) hearth2);
        font = create_sprite((xpm_map_t) startFont);
        ScoreBackGrnd = create_sprite((xpm_map_t) ScoreBrdBackgrnd);
        LeaderBackGrnd = create_sprite((xpm_map_t) LeaderbrdBckgrnd);
        mouse_cursor= create_sprite((xpm_map_t) square);
        menu_full = create_sprite((xpm_map_t) full_Menu);
        background = create_sprite((xpm_map_t) back_ground);
        monster1 = create_sprite((xpm_map_t) monster_1);
        monster2 = create_sprite((xpm_map_t) monster_2);
        bala = create_sprite((xpm_map_t) bala1);
        bala_inimigo = create_sprite((xpm_map_t) bala2);
        instructions = create_sprite((xpm_map_t) instructions_14c);
    }
    else if (mode==0x11A) {
        cuphead1 = create_sprite((xpm_map_t) cuphead1_11A);
        cuphead2 = create_sprite((xpm_map_t) cuphead2_11A);
        cuphead3 = create_sprite((xpm_map_t) cuphead3_11A);
        cuphead4 = create_sprite((xpm_map_t) cuphead4_11A);
        cuphead5 = create_sprite((xpm_map_t) cuphead5_11A);
        cuphead6 = create_sprite((xpm_map_t) cuphead6_11A);
        cuphead7 = create_sprite((xpm_map_t) cuphead7_11A);
        cuphead8 = create_sprite((xpm_map_t) cuphead8_11A);
        cuphead9 = create_sprite((xpm_map_t) cuphead9_11A);
        cuphead10 = create_sprite((xpm_map_t) cuphead10_11A);
        cuphead11 = create_sprite((xpm_map_t) cuphead11_11A);
        cuphead12 = create_sprite((xpm_map_t) cuphead12_11A);
        cuphead13 = create_sprite((xpm_map_t) cuphead13_11A);
        cuphead14 = create_sprite((xpm_map_t) cuphead14_11A);
        cuphead15 = create_sprite((xpm_map_t) cuphead15_11A);
        cuphead16 = create_sprite((xpm_map_t) cuphead16_11A);
        cupheadstand = create_sprite((xpm_map_t) cupheadstand_11A);
        
        hearthLife = create_sprite((xpm_map_t) hearth2_11A);
        font = create_sprite((xpm_map_t) PressStart2P_vaV7_11A);
        ScoreBackGrnd = create_sprite((xpm_map_t) ScoreBoardBackgrd11A);
        LeaderBackGrnd = create_sprite((xpm_map_t) LeaderboardMenu11A);
        mouse_cursor= create_sprite((xpm_map_t) square11A);
        menu_full = create_sprite((xpm_map_t) full_Menu11A);
        background = create_sprite((xpm_map_t) background11A);
        monster1 = create_sprite((xpm_map_t) monster1_11A);
        monster2 = create_sprite((xpm_map_t) monster2_11A);
        bala = create_sprite((xpm_map_t) bala11A);
        bala_inimigo = create_sprite((xpm_map_t) bala_inimigo_11A);
        instructions = create_sprite((xpm_map_t) instructions_11a);
    }
    else if (mode==0x115) {
        cuphead1 = create_sprite((xpm_map_t) cuphead1_115);
        cuphead2 = create_sprite((xpm_map_t) cuphead2_115);
        cuphead3 = create_sprite((xpm_map_t) cuphead3_115);
        cuphead4 = create_sprite((xpm_map_t) cuphead4_115);
        cuphead5 = create_sprite((xpm_map_t) cuphead5_115);
        cuphead6 = create_sprite((xpm_map_t) cuphead6_115);
        cuphead7 = create_sprite((xpm_map_t) cuphead7_115);
        cuphead8 = create_sprite((xpm_map_t) cuphead8_115);
        cuphead9 = create_sprite((xpm_map_t) cuphead9_115);
        cuphead10 = create_sprite((xpm_map_t) cuphead10_115);
        cuphead11 = create_sprite((xpm_map_t) cuphead11_115);
        cuphead12 = create_sprite((xpm_map_t) cuphead12_115);
        cuphead13 = create_sprite((xpm_map_t) cuphead13_115);
        cuphead14 = create_sprite((xpm_map_t) cuphead14_115);
        cuphead15 = create_sprite((xpm_map_t) cuphead15_115);
        cuphead16 = create_sprite((xpm_map_t) cuphead16_115);
        cupheadstand = create_sprite((xpm_map_t) cupheadstand_115);

        hearthLife = create_sprite((xpm_map_t) hearth2_115);
        font = create_sprite((xpm_map_t) PressStart2P_vaV7_115);
        ScoreBackGrnd = create_sprite((xpm_map_t) ScoreBoardBackgrd_115);
        LeaderBackGrnd = create_sprite((xpm_map_t) LeaderboardMenu_115);
        mouse_cursor= create_sprite((xpm_map_t) square_115);
        menu_full = create_sprite((xpm_map_t) full_Menu_115);
        background = create_sprite((xpm_map_t) background_115);
        monster1 = create_sprite((xpm_map_t) monster1_115);
        monster2 = create_sprite((xpm_map_t) monster2_115);
        bala = create_sprite((xpm_map_t) bala_115);
        bala_inimigo = create_sprite((xpm_map_t) bala_inimigo_115);
        instructions = create_sprite((xpm_map_t) instructions_115);
    }
    else if (mode==0x110) {
        cuphead1 = create_sprite((xpm_map_t) cuphead1_110);
        cuphead2 = create_sprite((xpm_map_t) cuphead2_110);
        cuphead3 = create_sprite((xpm_map_t) cuphead3_110);
        cuphead4 = create_sprite((xpm_map_t) cuphead4_110);
        cuphead5 = create_sprite((xpm_map_t) cuphead5_110);
        cuphead6 = create_sprite((xpm_map_t) cuphead6_110);
        cuphead7 = create_sprite((xpm_map_t) cuphead7_110);
        cuphead8 = create_sprite((xpm_map_t) cuphead8_110);
        cuphead9 = create_sprite((xpm_map_t) cuphead9_110);
        cuphead10 = create_sprite((xpm_map_t) cuphead10_110);
        cuphead11 = create_sprite((xpm_map_t) cuphead11_110);
        cuphead12 = create_sprite((xpm_map_t) cuphead12_110);
        cuphead13 = create_sprite((xpm_map_t) cuphead13_110);
        cuphead14 = create_sprite((xpm_map_t) cuphead14_110);
        cuphead15 = create_sprite((xpm_map_t) cuphead15_110);
        cuphead16 = create_sprite((xpm_map_t) cuphead16_110);
        cupheadstand = create_sprite((xpm_map_t) cupheadstand_110);

        hearthLife = create_sprite((xpm_map_t) hearth2_110);
        font = create_sprite((xpm_map_t) PressStart2P_vaV7_110);
        ScoreBackGrnd = create_sprite((xpm_map_t) ScoreBoardBackgrd_110);
        LeaderBackGrnd = create_sprite((xpm_map_t) LeaderboardMenu_110);
        mouse_cursor= create_sprite((xpm_map_t) square_110);
        menu_full = create_sprite((xpm_map_t) full_Menu_110);
        background = create_sprite((xpm_map_t) background_110);
        monster1 = create_sprite((xpm_map_t) monster1_110);
        monster2 = create_sprite((xpm_map_t) monster2_110);
        bala = create_sprite((xpm_map_t) bala_110);
        bala_inimigo = create_sprite((xpm_map_t) bala_inimigo_110);
        instructions = create_sprite((xpm_map_t) instructions_110);
    }
    
}

int clean_img(uint16_t x, uint16_t y, uint16_t width) {
    for (int w = 0 ; w < width ; w++) {
        for (int h = 0 ; h < width ; h++) {
            if (vg_draw_pixel(x + w, y + h, 0)) return 1;
        }
    }
    return 0; 
}

void freeAllSprites(){
    destroy_sprite(cuphead1);
    destroy_sprite(cuphead2);
    destroy_sprite(cuphead3);
    destroy_sprite(cuphead4);
    destroy_sprite(cuphead5);
    destroy_sprite(cuphead6);
    destroy_sprite(cuphead7);
    destroy_sprite(cuphead8);
    destroy_sprite(cuphead9);
    destroy_sprite(cuphead10);
    destroy_sprite(cuphead11);
    destroy_sprite(cuphead12);
    destroy_sprite(cuphead13);
    destroy_sprite(cuphead14);
    destroy_sprite(cuphead15);
    destroy_sprite(cuphead16);
    destroy_sprite(cupheadstand);
    destroy_sprite(hearthLife);
    destroy_sprite(font);
    destroy_sprite(ScoreBackGrnd);
    destroy_sprite(LeaderBackGrnd);
    destroy_sprite(mouse_cursor);
    destroy_sprite(menu_full);
    destroy_sprite(background);
    destroy_sprite(monster1);
    destroy_sprite(monster2);
    destroy_sprite(bala);
    destroy_sprite(bala_inimigo);
    destroy_sprite(instructions);
}


bool check_collision(Sprite *sp1, int x1, int y1, Sprite *sp2, int x2,  int y2) {
    if (sp1 == NULL || sp2 == NULL) return false;

    int sp1_x_end = x1 + sp1->width;
    int sp1_y_end = y1 + sp1->height;
    int sp2_x_end = x2 + sp2->width;
    int sp2_y_end = y2 + sp2->height;
    
    if ((x1 < sp2_x_end && sp1_x_end > x2) && (y1 < sp2_y_end && sp1_y_end > y2)) {
        return true;
    }
    return false;
    
}

int drawChar(char c, uint16_t x, uint16_t y) { //height = 17, width = 16 - 0x14C
    uint16_t xMap=0, yMap=0;
    
    if (c >= 'a' && c <= 'z') {
        xMap = 9 + (c - 'a') * 18;
        yMap = 1;
    } else if (c >= '0' && c <= '9') {
        xMap = 9 + (c - '0') * 18;
        yMap = 36;
    } else if (c == ':') {
        xMap = 9 + 11 * 18;
        yMap = 36;
    } else if (c == '/') {
        xMap = 9 + 30 * 18;
        yMap = 36;
    } else if (c == '-') {
        xMap = 9 + 27 * 18;
        yMap = 36;
    } else {
        return 1;
    }
    
    if (mode == 0x110) {
        xMap = xMap*info.XResolution/DEFAULT_X_RESOLUTION_14C;
        yMap = yMap*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
        for (int h = 0 ; h < 18*info.YResolution/DEFAULT_Y_RESOLUTION_14C; h++) {
            for (int w = 0 ; w < 18*info.XResolution/DEFAULT_X_RESOLUTION_14C; w++) {
                if (vg_draw_pixel(x + w,y + h, font->map[xMap + w + (yMap+h)*443])) return 1;
            }
        }
    } else if (mode == 0x14C) {
        for (int h = 0 ; h < 18; h++) {
            for (int w = 0 ; w < 18; w++) {
                if (vg_draw_pixel(x + w,y + h, font->map[xMap + w + (yMap+h)*800])) return 1;
            }
        }
    } else if (mode == 0x115) {
        xMap = xMap*info.XResolution/DEFAULT_X_RESOLUTION_14C;
        yMap = yMap*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
        for (int h = 0 ; h < 13; h++) {
            for (int w = 0 ; w < 13; w++) {
                if (vg_draw_pixel(x + w,y + h, font->map[xMap + w + (yMap+h)*555])) return 1;
            }
        }
    } else if (mode == 0x11A) {
        xMap = xMap*info.XResolution/DEFAULT_X_RESOLUTION_14C;
        yMap = yMap*info.YResolution/DEFAULT_Y_RESOLUTION_14C;
        for (int h = 0 ; h < 20; h++) {
            for (int w = 0 ; w < 20; w++) {
                if (vg_draw_pixel(x + w,y + h, font->map[xMap + w + (yMap+h)*888])) return 1;
            }
        }
    }
    
    return 0;
}

int drawNum(int c, uint16_t x, uint16_t y) { 
    return drawChar('0' + c, x, y);
}

int drawTxt(char txt[], uint16_t x, uint16_t y) {
    uint8_t i=0;
    int widthLetters=0;
    if(mode == 0x11A) {
        widthLetters=20;
    } else if (mode == 0x110) {
        widthLetters=10;
    } else if (mode == 0x14C){
        widthLetters = 18;
    } else if (mode == 0x115){
        widthLetters=12;
    }
    while(txt[i]) {
        if(txt[i]==' ') {
            i++;
        }
        else {
            if(drawChar(txt[i],x+(widthLetters)*i,y)) {return 1;}
            i++;
        }   
    }
    return 0;
}

