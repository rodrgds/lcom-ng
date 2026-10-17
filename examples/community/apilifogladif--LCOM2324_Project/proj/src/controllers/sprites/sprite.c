#include <lcom/lcf.h>
#include "sprite.h"

extern unsigned h_res;
extern unsigned v_res;
extern unsigned bytes_per_pixel;

enum xpm_image_type type = XPM_8_8_8_8;

Sprite *darkmenu;
Sprite *lightmenu;
Sprite *sunsetmenu;
Sprite *helpmenu;
Sprite *button_local;
Sprite *button_help;
Sprite *button_quit;
Sprite *button_next;
Sprite *button_remote;
Sprite *title;
Sprite *board;
Sprite *cursor;
Sprite *hand_cursor;
Sprite *crosshair;
Sprite *s_boat_2p;
Sprite *s_boat_3p1;
Sprite *s_boat_3p2;
Sprite *s_boat_4p;
Sprite *s_boat_5p;
Sprite *s_boat_2p_selected;
Sprite *s_boat_3p1_selected;
Sprite *s_boat_3p2_selected;
Sprite *s_boat_4p_selected;
Sprite *s_boat_5p_selected;
Sprite *s_boat_2p_V;
Sprite *s_boat_3p1_V;
Sprite *s_boat_3p2_V;
Sprite *s_boat_4p_V;
Sprite *s_boat_5p_V;
Sprite *s_boat_2p_selected_V;
Sprite *s_boat_3p1_selected_V;
Sprite *s_boat_3p2_selected_V;
Sprite *s_boat_4p_selected_V;
Sprite *s_boat_5p_selected_V;
Sprite *player1;
Sprite *player2;
Sprite *waiting1;
Sprite *myTurn;
Sprite *sure_go_game;
Sprite *sure_next_player;
Sprite *sure_quit_text;
Sprite *player1_won;
Sprite *player2_won;
Sprite *choosing;

Sprite *game_board;
Sprite *small_s_boat_2p;
Sprite *small_s_boat_3p1;
Sprite *small_s_boat_3p2;
Sprite *small_s_boat_4p;
Sprite *small_s_boat_5p;
Sprite *small_s_boat_2p_V;
Sprite *small_s_boat_3p1_V;
Sprite *small_s_boat_3p2_V;
Sprite *small_s_boat_4p_V;
Sprite *small_s_boat_5p_V;

Sprite *zero;
Sprite *one;
Sprite *two;
Sprite *three;
Sprite *four;
Sprite *five;
Sprite *six;
Sprite *seven;
Sprite *eight;
Sprite *nine;
Sprite *ten;

Sprite *create_sprite(xpm_map_t sprite, int x, int y){

  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if( sp == NULL ) return NULL;

  xpm_image_t img;
  sp->colors = (uint32_t *) xpm_load(sprite, type, &img);
  sp->height = img.height;
  sp->width = img.width;
  sp->x = x;
  sp->y = y;
  sp->type = img.type;

  if( sp->colors == NULL ) {
    free(sp);
    return NULL;
  }

  return sp;
}

void destroy_sprite(Sprite *sp) {
    if(sp == NULL) return;
    if(sp ->colors)
    free(sp->colors);
    free(sp);
    sp = NULL;
}

void load_game_sprites() {
    darkmenu = create_sprite((xpm_map_t) mainmenu_dark_xpm, 0, 0);
    lightmenu = create_sprite((xpm_map_t) mainmenu_light_xpm, 0, 0);
    sunsetmenu = create_sprite((xpm_map_t) mainmenu_sunset_xpm, 0, 0);

    helpmenu = create_sprite((xpm_map_t) help_xpm, 0, 0);

    title = create_sprite((xpm_map_t) title_xpm, 0, 50);
    board = create_sprite((xpm_map_t) board_xpm, 0, 0);
    game_board = create_sprite((xpm_map_t) game_board_xpm, 0, 0);

    button_local = create_sprite((xpm_map_t) button_local_xpm, 456, 360);
    button_remote = create_sprite((xpm_map_t) button_remote_xpm, 427, 475);
    button_help = create_sprite((xpm_map_t) button_help_xpm, 481, 590);
    button_quit = create_sprite((xpm_map_t) button_quit_xpm, 483, 705);

    button_next = create_sprite((xpm_map_t) button_next_xpm, 1107, 43);

    cursor = create_sprite((xpm_map_t) cursor_xpm, 200, 200);
    hand_cursor = create_sprite((xpm_map_t) hand_cursor_xpm, 200, 200);
    crosshair = create_sprite((xpm_map_t) crosshair_xpm, 200, 200);

    load_boat_sprites(BIG_BOARD_X, BIG_BOARD_Y);

    load_small_boat_sprites(SMALL_BOARD_X, SMALL_BOARD_Y);

    player1 = create_sprite((xpm_map_t) player1_xpm, 907, 43);
    player2 = create_sprite((xpm_map_t) player2_xpm, 907, 43);
    sure_go_game = create_sprite((xpm_map_t) sure_go_game_xpm, 0, 288);
    sure_next_player = create_sprite((xpm_map_t) sure_next_player_xpm, 0, 288);
    sure_quit_text = create_sprite((xpm_map_t) sure_quit_text_xpm, 0, 288);
    player1_won = create_sprite((xpm_map_t) player1_won_xpm, 0, 288);
    player2_won = create_sprite((xpm_map_t) player2_won_xpm, 0, 288);

    zero = create_sprite((xpm_map_t) zero_xpm, 930, 160);
    one = create_sprite((xpm_map_t) one_xpm, 930, 160);
    two = create_sprite((xpm_map_t) two_xpm, 930, 160);
    three = create_sprite((xpm_map_t) three_xpm, 930, 160);
    four = create_sprite((xpm_map_t) four_xpm, 930, 160);
    five = create_sprite((xpm_map_t) five_xpm, 930, 160);
    six = create_sprite((xpm_map_t) six_xpm, 930, 160);
    seven = create_sprite((xpm_map_t) seven_xpm, 930, 160);
    eight = create_sprite((xpm_map_t) eight_xpm, 930, 160);
    nine = create_sprite((xpm_map_t) nine_xpm, 930, 160);
    ten = create_sprite((xpm_map_t) ten_xpm, 930, 160);

    waiting1 = create_sprite((xpm_map_t) waiting1_xpm, 895, 43);
    myTurn = create_sprite((xpm_map_t) myTurn_xpm, 895, 43);
    choosing = create_sprite((xpm_map_t) choosing_xpm, 0, 0);
}

void load_boat_sprites(int x, int y) {
    s_boat_2p_selected = create_sprite((xpm_map_t) boat_2p_w_xpm, x, y);
    s_boat_3p1_selected = create_sprite((xpm_map_t) boat_3p_w_xpm, x, y);
    s_boat_3p2_selected = create_sprite((xpm_map_t) boat_3p_w_xpm, x, y);
    s_boat_4p_selected = create_sprite((xpm_map_t) boat_4p_w_xpm, x, y);
    s_boat_5p_selected = create_sprite((xpm_map_t) boat_5p_w_xpm, x, y);
    s_boat_2p = create_sprite((xpm_map_t) boat_2p_b_xpm, x, y);
    s_boat_3p1 = create_sprite((xpm_map_t) boat_3p_b_xpm, x, y);
    s_boat_3p2 = create_sprite((xpm_map_t) boat_3p_b_xpm, x, y);
    s_boat_4p = create_sprite((xpm_map_t) boat_4p_b_xpm, x, y);
    s_boat_5p = create_sprite((xpm_map_t) boat_5p_b_xpm, x, y);

    s_boat_2p_selected_V = create_sprite((xpm_map_t) boat_2p_w_V_xpm, x, y);
    s_boat_3p1_selected_V = create_sprite((xpm_map_t) boat_3p_w_V_xpm, x, y);
    s_boat_3p2_selected_V = create_sprite((xpm_map_t) boat_3p_w_V_xpm, x, y);
    s_boat_4p_selected_V = create_sprite((xpm_map_t) boat_4p_w_V_xpm, x, y);
    s_boat_5p_selected_V = create_sprite((xpm_map_t) boat_5p_w_V_xpm, x, y);
    s_boat_2p_V = create_sprite((xpm_map_t) boat_2p_b_V_xpm,  x, y);
    s_boat_3p1_V = create_sprite((xpm_map_t) boat_3p_b_V_xpm, x, y);
    s_boat_3p2_V = create_sprite((xpm_map_t) boat_3p_b_V_xpm, x, y);
    s_boat_4p_V = create_sprite((xpm_map_t) boat_4p_b_V_xpm,  x, y);
    s_boat_5p_V = create_sprite((xpm_map_t) boat_5p_b_V_xpm,  x, y);
}

void load_small_boat_sprites(int x, int y) {
    small_s_boat_2p = create_sprite((xpm_map_t) small_boat_2p_b_xpm, x, y);
    small_s_boat_3p1 = create_sprite((xpm_map_t) small_boat_3p_b_xpm, x, y);
    small_s_boat_3p2 = create_sprite((xpm_map_t) small_boat_3p_b_xpm, x, y);
    small_s_boat_4p = create_sprite((xpm_map_t) small_boat_4p_b_xpm, x, y);
    small_s_boat_5p = create_sprite((xpm_map_t) small_boat_5p_b_xpm, x, y);

    small_s_boat_2p_V = create_sprite((xpm_map_t) small_boat_2p_b_V_xpm,  x, y);
    small_s_boat_3p1_V = create_sprite((xpm_map_t) small_boat_3p_b_V_xpm, x, y);
    small_s_boat_3p2_V = create_sprite((xpm_map_t) small_boat_3p_b_V_xpm, x, y);
    small_s_boat_4p_V = create_sprite((xpm_map_t) small_boat_4p_b_V_xpm,  x, y);
    small_s_boat_5p_V = create_sprite((xpm_map_t) small_boat_5p_b_V_xpm,  x, y);
}

void destroy_game_sprites() {
    destroy_sprite(lightmenu);
    destroy_sprite(darkmenu);
    destroy_sprite(sunsetmenu);
    destroy_sprite(helpmenu);
    destroy_sprite(title);
    destroy_sprite(button_local);
    destroy_sprite(button_help);
    destroy_sprite(button_quit);
    destroy_sprite(button_next);
    destroy_sprite(board);
    destroy_sprite(cursor);
    destroy_sprite(hand_cursor);
    destroy_sprite(player1);
    destroy_sprite(player2);
    destroy_sprite(sure_next_player);
    destroy_sprite(sure_quit_text);
    destroy_sprite(sure_go_game);
    destroy_sprite(s_boat_2p);
    destroy_sprite(s_boat_3p1);
    destroy_sprite(s_boat_3p2);
    destroy_sprite(s_boat_4p);
    destroy_sprite(s_boat_5p);
    destroy_sprite(s_boat_2p_selected);
    destroy_sprite(s_boat_3p1_selected);
    destroy_sprite(s_boat_3p2_selected);
    destroy_sprite(s_boat_4p_selected);
    destroy_sprite(s_boat_5p_selected);
    destroy_sprite(s_boat_2p_V);
    destroy_sprite(s_boat_3p1_V);
    destroy_sprite(s_boat_3p2_V);
    destroy_sprite(s_boat_4p_V);
    destroy_sprite(s_boat_5p_V);
    destroy_sprite(s_boat_2p_selected_V);
    destroy_sprite(s_boat_3p1_selected_V);
    destroy_sprite(s_boat_3p2_selected_V);
    destroy_sprite(s_boat_4p_selected_V);
    destroy_sprite(s_boat_5p_selected_V);
    destroy_sprite(small_s_boat_2p);
    destroy_sprite(small_s_boat_3p1);
    destroy_sprite(small_s_boat_3p2);
    destroy_sprite(small_s_boat_4p);
    destroy_sprite(small_s_boat_5p);
    destroy_sprite(small_s_boat_2p_V);
    destroy_sprite(small_s_boat_3p1_V);
    destroy_sprite(small_s_boat_3p2_V);
    destroy_sprite(small_s_boat_4p_V);
    destroy_sprite(small_s_boat_5p_V);
    destroy_sprite(zero);
    destroy_sprite(one);
    destroy_sprite(two);
    destroy_sprite(three);
    destroy_sprite(four);
    destroy_sprite(five);
    destroy_sprite(six);
    destroy_sprite(seven);
    destroy_sprite(eight);
    destroy_sprite(nine);
    destroy_sprite(ten);
}

void draw_sprite(Sprite *sp) {
    for(int i = 0; i < sp->height; i++) {
        for(int j = 0; j < sp->width; j++) {
            if (sp->colors[j + i* sp->width] != TRANSPARENT) {
                vg_draw_pixel(sp->x + j, sp->y + i,  sp->colors[j + i* sp->width]);
            }
        }
    }
}

