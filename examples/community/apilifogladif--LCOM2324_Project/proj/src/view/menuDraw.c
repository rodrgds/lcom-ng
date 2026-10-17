#include <lcom/lcf.h>
#include "menuDraw.h"

extern Sprite *darkmenu;
extern Sprite *lightmenu;
extern Sprite *sunsetmenu;
extern Sprite *helpmenu;
extern Sprite *title;
extern Sprite *button_local;
extern Sprite *button_help;
extern Sprite *button_quit;
extern Sprite *button_remote;
extern Sprite *cursor;
extern Sprite *hand_cursor;

extern enum MODE menu;

extern int x;
extern int y;

void drawMainMenu() {
    int time = timeRTC();
    if(time == 0 || time == 3){
        draw_sprite(lightmenu);
    } else if (time == 1) {
        draw_sprite(darkmenu);
    } else if (time == 2) {
        draw_sprite(sunsetmenu);
    }

    draw_sprite(title);
    draw_sprite(button_local);
    draw_sprite(button_quit);
    draw_sprite(button_remote);
    draw_sprite(button_help);
}

void drawMainCursor() {
    if (y > button_local->y && y < button_local->y + button_local->height) {
        if (x > button_local->x && x < button_local->x + button_local->width) {
            draw_sprite(hand_cursor);
        }
        else draw_sprite(cursor);
    }
    else if (y > button_remote->y && y < button_remote->y + button_remote->height) {
        if (x > button_remote->x && x < button_remote->x + button_remote->width) {
            draw_sprite(hand_cursor);
        }
        else draw_sprite(cursor);
    }
    else if (y > button_help->y && y < button_help->y + button_help->height) {
        if (x > button_help->x && x < button_help->x + button_help->width) {
            draw_sprite(hand_cursor);
        }
        else draw_sprite(cursor);
    }
    else if (y > button_quit->y && y < button_quit->y + button_quit->height) {
        if (x > button_quit->x && x < button_quit->x + button_quit->width) {
            draw_sprite(hand_cursor);
        }
        else draw_sprite(cursor);
    }
    else draw_sprite(cursor);
}
