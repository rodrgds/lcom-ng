#include <lcom/lcf.h>
#include "assetDraw.h"


extern Sprite *board;
extern Sprite *game_board;
extern Sprite *cursor;
extern Sprite *hand_cursor;
extern Sprite *crosshair;
extern Sprite *player1;
extern Sprite *player2;
extern Sprite *sure_go_game;
extern Sprite *sure_next_player;
extern Sprite *sure_quit_text;
extern Sprite *player1_won;
extern Sprite *player2_won;

extern Sprite *zero;
extern Sprite *one;
extern Sprite *two;
extern Sprite *three;
extern Sprite *four;
extern Sprite *five;
extern Sprite *six;
extern Sprite *seven;
extern Sprite *eight;
extern Sprite *nine;
extern Sprite *ten;

extern int global_counter;

extern Sprite *waiting1;
extern Sprite *myTurn;
extern Sprite *choosing;

extern Player p1;
extern Player p2;

extern bool cursorActive;

void drawBoard() {
    draw_sprite(board);
}

void drawGameBoard() {
    draw_sprite(game_board);
}

void drawNamePlayer1() {
    draw_sprite(player1);
}

void drawQuitText() {
    draw_sprite(sure_quit_text);
}

void drawGoGameText() {
    draw_sprite(sure_go_game);
}

void drawNextPlayerText() {
    draw_sprite(sure_next_player);
}

void drawNamePlayer2() {
    draw_sprite(player2);
}

void drawCursor() {
    draw_sprite(cursor);
}

void drawHandCursor() {
    draw_sprite(hand_cursor);
}

void drawCrosshair() {
    draw_sprite(crosshair);
}

void drawEndGame(int p) {
    if(p == 1){
        draw_sprite(player1_won);
    }
    else if(p == 2){
        draw_sprite(player2_won);
    }
}

void drawTime() {
    switch(global_counter / 60) {
        case 0 :
            draw_sprite(ten);
            break;
        case 1:
            draw_sprite(nine);
            break;
        case 2:
            draw_sprite(eight);
            break;
        case 3:
            draw_sprite(seven);
            break;
        case 4:
            draw_sprite(six);
            break;
        case 5:
            draw_sprite(five);
            break;
        case 6:
            draw_sprite(four);
            break;
        case 7:
            draw_sprite(three);
            break;
        case 8:
            draw_sprite(two);
            break;
        case 9:
            draw_sprite(one);
            break;  
        default:
            draw_sprite(zero);
            break;    
    }
}

void drawWaiting() {
    draw_sprite(waiting1);
}

void drawMyTurn() {
    draw_sprite(myTurn);
}
