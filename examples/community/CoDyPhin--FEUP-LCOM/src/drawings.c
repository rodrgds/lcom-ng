#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "drawings.h"
#include "rtc.h"


Cursor* (create_sprite)(xpm_map_t pic, int x, int y, int xspeed, int yspeed) {//allocate space for the "object"
    Cursor*sp = (Cursor*) malloc (sizeof(Cursor));
    xpm_image_t xpm_info;
    if(sp == NULL)return NULL;
    sp->map = xpm_load(pic, XPM_DIRECT, &xpm_info);
    if(sp->map == NULL) {
        free(sp);
        return NULL;
    }
    sp->width = xpm_info.width;
    sp->height=xpm_info.height;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->x = x;
    sp->y = y;
    return sp;
}

int (animate_sprite)(Cursor*sp){
    int x = sp->x;
    int y = sp->y;
    if (sp->xspeed != 0){
        x += sp->xspeed;
        if (x < 280 || x > 835) return 0;
        sp->x += sp->xspeed;
    }
    if (sp->yspeed != 0){
        y += sp->yspeed;
        if (y < 140 || y > 690) return 0;
        sp->y += sp->yspeed;
    }
    return 1;
}

int (erase_sprite)(Cursor* spr, uint8_t* db, uint8_t* bckgrdSpr) {
    for(uint16_t i = 0; i < 4*40; i += 4) {
        if((spr->y + i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*40; j++) {
            if((spr->x + j)>= 4*getHres()) break;
            uint8_t *pixel = db + (4*spr->y + i)*getHres() +  4*spr->x + j;
            *pixel = bckgrdSpr[(4*spr->y + i)*getHres() + 4*spr->x + j];
        }
    }
    return 0;
}

int (clear_image)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* db, uint8_t* bckgrdSpr) {
    for(uint16_t i = 0; i < 4*width; i += 4) {
        if((y + i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*height; j++) {
            if((x + j)>= 4*getHres()) break;
            uint8_t *pixel = db + (y + i)*getHres() +  x + j;
            *pixel = bckgrdSpr[(y + i)*getHres() + x + j];
        }
    }
    return 0;
}

int (move_sprite)(Cursor* spr, uint8_t* db, uint8_t* bckgrdSpr) {
    erase_sprite(spr, db, bckgrdSpr);
    animate_sprite(spr);
    drawCursor(spr, db);
    return 0;
}

int (draw_background)(xpm_map_t bkgrd, uint8_t* adrs){
    xpm_image_t bkgrd_info;
    uint8_t* sprite = xpm_load(bkgrd, XPM_DIRECT, &bkgrd_info);
    if(sprite == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*bkgrd_info.height; i++){
        if((i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < bkgrd_info.width; j++){
            if((j)>= getHres()) break;
            uint8_t *pixel = adrs + ((i)*getHres()+j);
            *pixel = sprite[i*bkgrd_info.width+j];
        }
    }
    return 0;
}

int (draw_option)(xpm_map_t xpm, uint8_t* adrs, uint16_t x, uint16_t y){
    xpm_image_t xpm_info;
    uint8_t * sprite = xpm_load(xpm, XPM_DIRECT, &xpm_info);
    if(sprite == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*xpm_info.height; i+=4){
        if((y+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*xpm_info.width; j++){
            if((x+j)>= 4*getHres()) break;
            uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
            if (sprite[i*xpm_info.width+j] != TRANSPARENCY_BYTE) *pixel = sprite[i*xpm_info.width+j];
        }
    }
    return 0;
}

int (draw_menu_options)(xpm_map_t first, xpm_map_t second, xpm_map_t third, uint8_t* adrs){
        if(draw_option(first, adrs, 5*20+8, 2*4*100+60)) return 1;
        if(draw_option(second, adrs, 0, 3*4*100+60)) return 1;
        if(draw_option(third, adrs, 0, 4*4*100+60)) return 1;
        return 0;
}

int(draw_tabuleiro)(xpm_map_t square, uint8_t* adrs, uint16_t xi, uint16_t yi){
    xpm_image_t tab_info;
    uint16_t x = xi;
    uint16_t y = yi;
    bool reversex = false, reversey = false;
    uint8_t* spriteTab = xpm_load(square, XPM_DIRECT, &tab_info);
    if(spriteTab == NULL){
        return 1;
    }
    for(uint8_t size = 0; size < 28; size++){
        board[size].x = x;
        board[size].y = y;
        board[size].color = WHITE;
        board[size].sprite = spriteTab;
        board[size].sprite_info = tab_info;
        for(uint16_t i = 0; i < 4*tab_info.height; i+=4){
            if((y+i) >= 4*getVres()) break;
            for(uint16_t j = 0; j < 4*tab_info.width; j++){
                if((x+j)>= 4*getHres()) break;
                uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                *pixel = spriteTab[i*tab_info.width+j];
            }
        }
        if(x==(xi + 7*400) || reversex){
            if(y==(yi+7*400)){
                x-=400;
                reversex = x != xi;
            }
            else{
                y+=400;
            }
        }
        else {
            if(y==(yi+7*400) || reversey){
                y-=400;
                reversey = y != yi;
            }
            else{
                x+=400;
            }
        }
    }
    return 0;
}

int (paint_square)(xpm_map_t square, uint8_t* adrs, uint8_t position, uint32_t color){
    xpm_image_t tab_info;
    uint16_t x = board[position].x;
    uint16_t y = board[position].y;
    board[position].color = color;
    uint8_t* sprite = xpm_load(square, XPM_DIRECT, &tab_info);
    board[position].sprite = sprite;
    board[position].sprite_info = tab_info;
    if(sprite == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*tab_info.height; i+=4){
        if((y+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*tab_info.width; j++){
            if((x+j)>= 4*getHres()) break;
            uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
            *pixel = sprite[i*tab_info.width+j];
        }
    }
    return 0;
}

int (draw_player)(struct player* Plr, uint8_t* adrs, uint8_t position){
    uint8_t auxx = 0, auxy = 0;
    switch (Plr->playerNum){
        case 0:
            break;
        case 1:
            auxx = 1;
            break;
        case 2:
            auxy = 1;
            break;
        case 3:
            auxy = 1;
            auxx = 1;
            break;
    }
    if(Plr->position != position){
        if(position >= 28) position%=28;
        if(Plr->position > position){
            position+=28;
            if(Plr->hp < 100){
                Plr->hp+=5;
            }
        }
        if(Plr->sprite == NULL){
            return 1;
        }
        for(uint16_t k = Plr->position; k <= position; k++){
            uint16_t x = board[k%28].x+4*21+(auxx)*4*28;
            uint16_t y = board[k%28].y+4*21+(auxy)*4*28;
            for(uint16_t i = 0; i < 4*Plr->sprite_info.height; i+=4){
                if((y+i) >= 4*getVres()) break;
                for(uint16_t j = 0; j < 4*Plr->sprite_info.width; j++){
                    if((x+j)>= 4*getHres()) break;
                    uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                    if (Plr->sprite[i*Plr->sprite_info.width+j] != TRANSPARENCY_BYTE) *pixel = Plr->sprite[i*Plr->sprite_info.width+j];
                }
            }
            if(k!=position){
                if(resetBackground(board[k%28].sprite, board[k%28].sprite_info, Plr->sprite_info, adrs, x, y, 4*22, 4*22)){
                    return 1;
                }
            }
        }
    }
    else{
        uint16_t x = board[position].x+4*21+(auxx)*4*28;
        uint16_t y = board[position].y+4*21+(auxy)*4*28;
        for(uint16_t i = 0; i < 4*Plr->sprite_info.height; i+=4){
            if((y+i) >= 4*getVres()) break;
            for(uint16_t j = 0; j < 4*Plr->sprite_info.width; j++){
                if((x+j)>= 4*getHres()) break;
                uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                if (Plr->sprite[i*Plr->sprite_info.width+j] != TRANSPARENCY_BYTE) *pixel = Plr->sprite[i*Plr->sprite_info.width+j];
            }
        }

    }
    Plr->position = position % 28;
    return 0;
}

int draw_start_players(xpm_map_t redPlr, xpm_map_t bluePlr, xpm_map_t greenPlr, xpm_map_t yellowPlr, uint8_t* adrs, uint8_t colorchosen){
    switch (colorchosen) {
        case 0:
            players[0].sprite = xpm_load(redPlr, XPM_DIRECT, &players[0].sprite_info);
            players[0].hp = 100;
            players[0].playerNum = 0;
            players[0].color = RED;
            if(draw_player(&players[0], adrs, 0)){
                return 1;
            }
            players[1].sprite = xpm_load(greenPlr, XPM_DIRECT, &players[1].sprite_info);
            players[1].hp = 100;
            players[1].playerNum = 1;
            players[1].color = GREEN;
            if(draw_player(&players[1], adrs, 0)){
                return 1;
            }
            players[2].sprite = xpm_load(bluePlr, XPM_DIRECT, &players[2].sprite_info);
            players[2].hp = 100;
            players[2].playerNum = 2;
            players[2].color = BLUE;
            if(draw_player(&players[2], adrs, 0)){
                return 1;
            }
            players[3].sprite = xpm_load(yellowPlr, XPM_DIRECT, &players[3].sprite_info);
            players[3].hp = 100;
            players[3].playerNum = 3;
            players[3].color = YELLOW;
            if(draw_player(&players[3], adrs, 0)){
                return 1;
            }
            break;
        case 1:
            players[0].sprite = xpm_load(bluePlr, XPM_DIRECT, &players[0].sprite_info);
            players[0].hp = 100;
            players[0].playerNum = 0;
            players[0].color = BLUE;
            if(draw_player(&players[0], adrs, 0)){
                return 1;
            }
            players[1].sprite = xpm_load(greenPlr, XPM_DIRECT, &players[1].sprite_info);
            players[1].hp = 100;
            players[1].playerNum = 1;
            players[1].color = GREEN;
            if(draw_player(&players[1], adrs, 0)){
                return 1;
            }
            players[2].sprite = xpm_load(redPlr, XPM_DIRECT, &players[2].sprite_info);
            players[2].hp = 100;
            players[2].playerNum = 2;
            players[2].color = RED;
            if(draw_player(&players[2], adrs, 0)){
                return 1;
            }
            players[3].sprite = xpm_load(yellowPlr, XPM_DIRECT, &players[3].sprite_info);
            players[3].hp = 100;
            players[3].playerNum = 3;
            players[3].color = YELLOW;
            if(draw_player(&players[3], adrs, 0)){
                return 1;
            }
            break;
        case 2:
            players[0].sprite = xpm_load(greenPlr, XPM_DIRECT, &players[0].sprite_info);
            players[0].hp = 100;
            players[0].playerNum = 0;
            players[0].color = GREEN;
            if(draw_player(&players[0], adrs, 0)){
                return 1;
            }
            players[1].sprite = xpm_load(redPlr, XPM_DIRECT, &players[1].sprite_info);
            players[1].hp = 100;
            players[1].playerNum = 1;
            players[1].color = RED;
            if(draw_player(&players[1], adrs, 0)){
                return 1;
            }
            players[2].sprite = xpm_load(bluePlr, XPM_DIRECT, &players[2].sprite_info);
            players[2].hp = 100;
            players[2].playerNum = 2;
            players[2].color = BLUE;
            if(draw_player(&players[2], adrs, 0)){
                return 1;
            }
            players[3].sprite = xpm_load(yellowPlr, XPM_DIRECT, &players[3].sprite_info);
            players[3].hp = 100;
            players[3].playerNum = 3;
            players[3].color = YELLOW;
            if(draw_player(&players[3], adrs, 0)){
                return 1;
            }
            break;
        case 3:
            players[0].sprite = xpm_load(yellowPlr, XPM_DIRECT, &players[0].sprite_info);
            players[0].hp = 100;
            players[0].playerNum = 0;
            players[0].color = YELLOW;
            if(draw_player(&players[0], adrs, 0)){
                return 1;
            }
            players[1].sprite = xpm_load(greenPlr, XPM_DIRECT, &players[1].sprite_info);
            players[1].hp = 100;
            players[1].playerNum = 1;
            players[1].color = GREEN;
            if(draw_player(&players[1], adrs, 0)){
                return 1;
            }
            players[2].sprite = xpm_load(bluePlr, XPM_DIRECT, &players[2].sprite_info);
            players[2].hp = 100;
            players[2].playerNum = 2;
            players[2].color = BLUE;
            if(draw_player(&players[2], adrs, 0)){
                return 1;
            }
            players[3].sprite = xpm_load(redPlr, XPM_DIRECT, &players[3].sprite_info);
            players[3].hp = 100;
            players[3].playerNum = 3;
            players[3].color = RED;
            if(draw_player(&players[3], adrs, 0)){
                return 1;
            }
            break;
    }
    return 0;

}


int resetBackground(uint8_t* sprite, xpm_image_t bkgrd_info, xpm_image_t prevImgInfo, uint8_t* adrs, uint16_t xi, uint16_t yi, uint16_t x, uint16_t y){
    if(sprite == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*prevImgInfo.height; i+=4){
        if((yi+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*prevImgInfo.width; j++){
            if((xi+j)>= 4*getHres()) break;
            uint8_t *pixel = adrs + ((yi+i)*getHres()+xi+j);
            if (sprite[(y+i)*bkgrd_info.width+j+x] != TRANSPARENCY_BYTE) *pixel = sprite[(i+y)*bkgrd_info.width+j+x];
        }
    }
    return 0;
}

int drawTime(uint8_t* spriteBkgrd, xpm_image_t spriteBkgrdInfo, xpm_map_t numbers[], uint16_t chosenTime, uint16_t elapsedTime, uint8_t* adrs) {
    uint16_t currentTimer = chosenTime - elapsedTime;
    uint8_t digitsArray[5];
    uint8_t min = currentTimer / 60;
    uint8_t segs = currentTimer-min*60;
    uint8_t dezSeg = segs / 10;
    uint8_t uniSeg = segs - dezSeg*10;
    uint8_t dezMin = min / 10;
    uint8_t uniMin = min - dezMin*10;
    digitsArray[0] = dezMin;
    digitsArray[1] = uniMin;
    digitsArray[2] = 10;
    digitsArray[3] = dezSeg;
    digitsArray[4] = uniSeg;
    uint8_t * sprite;
    xpm_image_t number_info;
    for(uint8_t k = 0; k < 5; k++){
        sprite = xpm_load(numbers[digitsArray[k]], XPM_DIRECT, &number_info);
        uint16_t x = 4*500+k*4*35;
        uint16_t y = 4*150;
        if(resetBackground(spriteBkgrd, spriteBkgrdInfo, number_info, adrs, x, y, x, y)) return 1;
        if(sprite == NULL){
            return 1;
        }
        for(uint16_t i = 0; i < 4*number_info.height; i+=4){
            if((y+i) >= 4*getVres()) break;
            for(uint16_t j = 0; j < 4*number_info.width; j++){
                if((x+j)>= 4*getHres()) break;
                uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                if (sprite[i*number_info.width+j] != TRANSPARENCY_BYTE) *pixel = sprite[i*number_info.width+j];
            }
        }
    }
    return 0;
}

int drawSlots(xpm_map_t slotbg, xpm_map_t leverState, uint8_t* adrs){
    xpm_image_t slot_info;
    uint8_t *slotSpr = xpm_load(slotbg, XPM_DIRECT, &slot_info);
    if(slotSpr == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*slot_info.height; i+=4){
        if((4*500+4*35+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*slot_info.width; j++){
            if((4*500+j)>= 4*getHres()) break;
            uint8_t *pixel = adrs + ((4*500+i)*getHres()+4*450+4*35+j);
            if (slotSpr[i*slot_info.width+j] != TRANSPARENCY_BYTE) *pixel = slotSpr[i*slot_info.width+j];
        }
    }
    xpm_image_t lever_info;
    uint8_t * leverSpr = xpm_load(leverState, XPM_DIRECT, &lever_info);
    if(leverSpr == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*lever_info.height; i+=4){
        if((4*500+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*lever_info.width; j++){
            if((4*500+4*35+4*145+j)>= 4*getHres()) break;
            uint8_t *pixel = adrs + ((4*500+i)*getHres()+4*450+4*35+4*145+j);
            if (leverSpr[i*lever_info.width+j] != TRANSPARENCY_BYTE) *pixel = leverSpr[i*lever_info.width+j];
        }
    }
    return 0;
}

int throwDice(xpm_map_t lever, uint8_t *spriteBkgrd, xpm_image_t bkrd_info, uint8_t * adrs){
    uint8_t * leverSpr;
    xpm_image_t lever_info;
    leverSpr = xpm_load(lever, XPM_DIRECT, &lever_info);
    if (leverSpr == NULL) {
        return 1;
    }
    if(resetBackground(spriteBkgrd, bkrd_info, lever_info, adrs, 4*500+4*130, 4*500, 4*500+4*130, 4*500)) return 1;
    for (uint16_t i = 0; i < 4 * lever_info.height; i += 4) {
        if ((4 * 500 + i) >= 4 * getVres()) break;
        for (uint16_t j = 0; j < 4 * lever_info.width; j++) {
            if ((4 * 500 + 4 * 35 + 4 * 145 + j) >= 4 * getHres()) break;
            uint8_t *pixel = adrs + ((4 * 500 + i) * getHres() + 4 * 450 + 4 * 35 + 4 * 145 + j);
            if (leverSpr[i * lever_info.width + j] != TRANSPARENCY_BYTE)
                *pixel = leverSpr[i * lever_info.width + j];
        }
    }
    return 0;
}

int drawNum(xpm_map_t number, uint8_t* spriteBkrd, uint16_t x, uint16_t y, uint8_t* adrs){
    if(spriteBkrd == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*35; i+=4){
        if((y+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*35; j++){
            if((x+j)>= 4*getHres()) break;
            uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
            if (spriteBkrd[(y+i)*1152+j+x] != TRANSPARENCY_BYTE) *pixel = spriteBkrd[(i+y)*1152+j+x];
        }
    }
    xpm_image_t number_info;
    uint8_t* sprite = xpm_load(number, XPM_DIRECT, &number_info);
    if(sprite == NULL){
        return 1;
    }
    for(uint16_t i = 0; i < 4*number_info.height; i+=4){
        if((y+i) >= 4*getVres()) break;
        for(uint16_t j = 0; j < 4*number_info.width; j++){
            if((x+j)>= 4*getHres()) break;
            uint8_t *pixell = adrs + ((y+i)*getHres()+x+j);
            if(sprite[i*number_info.width+j] != TRANSPARENCY_BYTE)
                *pixell = sprite[i*number_info.width+j];
        }
    }
    return 0;
}

int drawCursor(Cursor* curs, uint8_t* adrs) {
    uint8_t* mapSpr = curs->map;
    for (uint16_t i = 0; i < 4*curs->height; i+=4) {
        if ((uint16_t)(i + curs->y) >= 4 * getVres()) break;
        for (uint16_t j = 0; j < 4 * curs->width; j++) {
            if ((uint16_t) (curs->x + j) >= 4 * getHres()) break;
            uint8_t *pixel = adrs + ((4 * curs->y + i) * getHres()) + 4 * curs->x + j;
            if (mapSpr[i * curs->width + j] != TRANSPARENCY_BYTE) *pixel = mapSpr[i * curs->width + j];
        }
    }
    return 0;
}

int drawHearts(xpm_map_t heart[], uint8_t *adrs){
    uint8_t* sprite;
    xpm_image_t info;
    uint16_t x, y;
    for(uint8_t k = 0; k < 4; k++){
        switch(players[k].color){
            case RED:
                 sprite = xpm_load(heart[0], XPM_DIRECT, &info);
                break;
            case BLUE:
                sprite = xpm_load(heart[1], XPM_DIRECT, &info);
                break;
            case GREEN:
                sprite = xpm_load(heart[2], XPM_DIRECT, &info);
                break;
            case YELLOW:
                sprite = xpm_load(heart[3], XPM_DIRECT, &info);
                break;
        }
        switch(k){
            case 0:
                x = 4*(35+35+35+5);
                y = 4*1;
                break;
            case 1:
                x = 4*(1152-40);
                y = 4*1;
                break;
            case 2:
                x = 4*(35+35+35+5);
                y = 4*(864-40);
                break;
            case 3:
                x = 4*(1152-40);
                y = 4*(864-40);
                break;
        }
        for(uint16_t i = 0; i < 4*info.height; i+=4){
            if((y+i) >= 4*getVres()) break;
            for(uint16_t j = 0; j < 4*info.width; j++){
                if((x+j)>= 4*getHres()) break;
                uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                if(sprite[i*info.width+j] != TRANSPARENCY_BYTE)
                    *pixel = sprite[i*info.width+j];
            }
        }
    }
    return 0;
}

int drawHP(uint8_t* spriteBkgrd, xpm_map_t numbers[], uint8_t* adrs){
    uint8_t * sprite;
    uint16_t x,y;
    xpm_image_t BkgrdInfo;
    BkgrdInfo.height = 1152;
    BkgrdInfo.width = 864;
    uint8_t digithp;
    uint8_t hp;
    xpm_image_t number_info;
    for(uint8_t k = 0; k < 4; k++) {
        hp = players[k].hp;
        for(uint16_t alg = 0; alg < 3; alg++){
            switch(alg){
                case 0:
                    digithp = (hp / 100);
                    break;
                case 1:
                    digithp = (hp % 100) / 10;
                    break;
                case 2:
                    digithp = (hp % 10) / 1;
                    break;
            }
            switch (k) {
                case 0:
                    x = 4 * alg*35;
                    y = 4 * 1;
                    break;
                case 1:
                    x = 4 * (1152 - 5 - (4-alg)*35);
                    y = 4 * 1;
                    break;
                case 2:
                    x = 4 * (alg*35);
                    y = 4 * (864 - 5 - 35);
                    break;
                case 3:
                    x = 4 * (1152 - 5 - (4-alg)*35);
                    y = 4 * (864 - 5 - 35);
                    break;
            }
            sprite = xpm_load(numbers[digithp], XPM_DIRECT, &number_info);
            clear_image(x, y, number_info.width, number_info.height, adrs, spriteBkgrd);
            if(sprite == NULL){
                return 1;
            }
            for(uint16_t i = 0; i < 4*number_info.height; i+=4){
                if((y+i) >= 4*getVres()) break;
                for(uint16_t j = 0; j < 4*number_info.width; j++){
                    if((x+j)>= 4*getHres()) break;
                    uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                    if (sprite[i*number_info.width+j] != TRANSPARENCY_BYTE) *pixel = sprite[i*number_info.width+j];
                }
            }
        }
    }
    return 0;
}

int killPlayer(struct player Plr, xpm_map_t whiteSquare, uint8_t* adrs){
    uint8_t auxx = 0, auxy = 0;
    switch (Plr.playerNum){
        case 0:
            break;
        case 1:
            auxx = 1;
            break;
        case 2:
            auxy = 1;
            break;
        case 3:
            auxy = 1;
            auxx = 1;
            break;
    }
    if(resetBackground(board[Plr.position].sprite, board[Plr.position].sprite_info, Plr.sprite_info, adrs, board[Plr.position].x+4*21+(auxx)*4*28, board[Plr.position].y+4*21+(auxy)*4*28, 4*22, 4*22)){
        return 1;
    }
    for(uint8_t i = 0; i < 28; i++){
        if(board[i].color == Plr.color){
            if(paint_square(whiteSquare,adrs, i, WHITE)){
                return 1;
            }
            board[i].color = WHITE;
        }
    }
    return 0;
}

int drawRTC(uint8_t* spriteBkgrd, xpm_image_t spriteBkgrdInfo, xpm_map_t numbers[], uint8_t* adrs){
    uint8_t digitsArray[8];
    time_date rtc = getData();
    digitsArray[0] = rtc.hour / 10;
    digitsArray[1] = rtc.hour % 10;
    digitsArray[2] = 10;
    digitsArray[3] = rtc.min / 10;
    digitsArray[4] = rtc.min % 10;
    digitsArray[5] = 10;
    digitsArray[6] = rtc.sec / 10;
    digitsArray[7] = rtc.sec % 10;
    uint8_t * sprite;
    xpm_image_t number_info;
    for(uint8_t k = 0; k < 8; k++){
        sprite = xpm_load(numbers[digitsArray[k]], XPM_DIRECT, &number_info);
        uint16_t x = 4*60+k*4*35;
        uint16_t y = 4*600;
        if(resetBackground(spriteBkgrd, spriteBkgrdInfo, number_info, adrs, x, y, x, y)) return 1;
        if(sprite == NULL){
            return 1;
        }
        for(uint16_t i = 0; i < 4*number_info.height; i+=4){
            if((y+i) >= 4*getVres()) break;
            for(uint16_t j = 0; j < 4*number_info.width; j++){
                if((x+j)>= 4*getHres()) break;
                uint8_t *pixel = adrs + ((y+i)*getHres()+x+j);
                if (sprite[i*number_info.width+j] != TRANSPARENCY_BYTE) *pixel = sprite[i*number_info.width+j];
            }
        }
    }
    return 0;
}
