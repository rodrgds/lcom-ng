#include <lcom/lcf.h>
#include "sprite.h"

#include "./images/menu.xpm"
#include "./images/choose_player.xpm"
#include "./images/create_player.xpm"
#include "./images/table.xpm"
#include "./images/close_button.xpm"
#include "./images/mouse_cursor.xpm"
#include "./images/characters.xpm"
#include "./images/deck.xpm"
#include "./images/coin.xpm"

SpriteLookup sprite_table[] = {
    { "menu_background", &menu_background },
    { "create_player_background", &create_player_background },
    { "choose_player_background", &choose_player_background },
    { "game_background", &game_background },
    { "deal_deck", &deal_deck },
    { "close_button", &close_button },
    { "coin", &coin_sprite },
    { "coin_shadow", &coin_shadow_sprite },

    { "mouse_cursor", &mouse_cursor },
    { "mouse_cursor_click", &mouse_cursor_click },

    { "card_back", &card_back },

    { "clubs2", &clubs_2 },
    { "clubs3", &clubs_3 },
    { "clubs4", &clubs_4 },
    { "clubs5", &clubs_5 },
    { "clubs6", &clubs_6 },
    { "clubs7", &clubs_7 },
    { "clubs8", &clubs_8 },
    { "clubs9", &clubs_9 },
    { "clubs10", &clubs_10 },
    { "clubsJ", &clubs_J },
    { "clubsQ", &clubs_Q },
    { "clubsK", &clubs_K },
    { "clubsA", &clubs_A },

    { "diamonds2", &diamonds_2 },
    { "diamonds3", &diamonds_3 },
    { "diamonds4", &diamonds_4 },
    { "diamonds5", &diamonds_5 },
    { "diamonds6", &diamonds_6 },
    { "diamonds7", &diamonds_7 },
    { "diamonds8", &diamonds_8 },
    { "diamonds9", &diamonds_9 },
    { "diamonds10", &diamonds_10 },
    { "diamondsJ", &diamonds_J },
    { "diamondsQ", &diamonds_Q },
    { "diamondsK", &diamonds_K },
    { "diamondsA", &diamonds_A },

    { "hearts2", &hearts_2 },
    { "hearts3", &hearts_3 },
    { "hearts4", &hearts_4 },
    { "hearts5", &hearts_5 },
    { "hearts6", &hearts_6 },
    { "hearts7", &hearts_7 },
    { "hearts8", &hearts_8 },
    { "hearts9", &hearts_9 },
    { "hearts10", &hearts_10 },
    { "heartsJ", &hearts_J },
    { "heartsQ", &hearts_Q },
    { "heartsK", &hearts_K },
    { "heartsA", &hearts_A },

    { "spades2", &spades_2 },
    { "spades3", &spades_3 },
    { "spades4", &spades_4 },
    { "spades5", &spades_5 },
    { "spades6", &spades_6 },
    { "spades7", &spades_7 },
    { "spades8", &spades_8 },
    { "spades9", &spades_9 },
    { "spades10", &spades_10 },
    { "spadesJ", &spades_J },
    { "spadesQ", &spades_Q },
    { "spadesK", &spades_K },
    { "spadesA", &spades_A },

    { "a", &a },
    { "b", &b },
    { "c", &c },
    { "d", &d },
    { "e", &e },
    { "f", &f },
    { "g", &g },
    { "h", &h },
    { "i", &i },
    { "j", &j },
    { "k", &k },
    { "l", &l },
    { "m", &m },
    { "n", &n },
    { "o", &o },
    { "p", &p },
    { "q", &q },
    { "r", &r },
    { "s", &s },
    { "t", &t },
    { "u", &u },
    { "v", &v },
    { "w", &w },
    { "x", &x },
    { "y", &y },
    { "z", &z },
    { "0", &zero },
    { "1", &one },
    { "2", &two },
    { "3", &three },
    { "4", &four },
    { "5", &five },
    { "6", &six },
    { "7", &seven },
    { "8", &eight },
    { "9", &nine },
    { " ", &space },
    { "-", &hyphen },
    { ":", &colon },
    {NULL, NULL}
};

Sprite *(create_sprite)(const char *pic[], int x, int y, int xspeed, int yspeed) {
  //allocate space for the "object"
  Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
  
  xpm_image_t img;

  if (sp == NULL)
    return NULL;

  // read the sprite pixmap
  sp->map = (uint32_t*) xpm_load(pic, XPM_8_8_8_8, &img);

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->width = img.width; 
  sp->height=img.height;
  sp->xspeed = xspeed;
  sp->yspeed = yspeed;
  sp->x = x;
  sp->y = y;
  
  return sp;
}

void (destroy_sprite)(Sprite *sp) {
  if (sp == NULL)
    return;

  if (sp->map)
    free(sp->map);

  free(sp);

  sp = NULL;
}

void (move_sprite)(Sprite *sp, uint32_t destx, uint32_t desty) {
  if (sp == NULL)
    return;

  sp->x += sp->xspeed;
  sp->y += sp->yspeed;

  if (sp->xspeed < 0) sp->x = sp->x >= destx ? sp->x : destx;
  else sp->x = sp->x <= destx ? sp->x : destx;

  if (sp->yspeed < 0) sp->y = sp->y >= desty ? sp->y : desty;
  else sp->y = sp->y <= desty ? sp->y : desty;

  return;
}

void (set_sprite_position)(Sprite *sp, uint32_t x, uint32_t y) {
  sp->x = x;
  sp->y = y;
}

void (draw_sprite)(Sprite *sp) {
  uint16_t height = sp->height, width = sp->width;
  for (uint32_t h = 0; h < height; h++)
  {
    for (uint32_t w = 0; w < width; w++)
    {
      vg_draw_pixel(sp->x + w, sp->y + h, sp->map[w + h*width]);
    }
  }
}

void (draw_sprite_to_buffer)(Sprite *sp, uint8_t *buffer) {
    uint16_t height = sp->height, width = sp->width;
    for (uint32_t h = 0; h < height; h++) {
        for (uint32_t w = 0; w < width; w++) {
            uint32_t x = sp->x + w;
            uint32_t y = sp->y + h;

            unsigned int index = (XResolution * y + x) * BytesPerPixel;
            uint32_t color = sp->map[w + h*width];
            
            if(color == 0xFF00FF)
                continue;

            if (x >= XResolution || y >= YResolution)
                continue;

            
            memcpy(&buffer[index], &color, BytesPerPixel);
        }
    }
}


void (draw_scaled_sprite)(Sprite *sp, float xScale, float yScale) {
    uint16_t height = sp->height * yScale; // Scale the height
    uint16_t width = sp->width * xScale; // Scale the width
    uint16_t y_offset = (sp->height - height) / 2; // Center the scaled sprite
    uint16_t x_offset = (sp->width - width) / 2; // Center the scaled sprite

    for (uint32_t h = 0; h < height; h++) {
        for (uint32_t w = 0; w < width; w++) {
            uint32_t color = sp->map[(int)(w / xScale) + (int) (h / yScale) * sp->width];
            vg_draw_pixel(sp->x + x_offset + w, sp->y + y_offset + h, color);
        }
    }
}

void (create_sprites)() {
    menu_background = create_sprite(menu_xpm, 0, 0, 0, 0);
    create_player_background = create_sprite(create_player_xpm, 0, 0, 0, 0);
    choose_player_background = create_sprite(choose_player_xpm, 0, 0, 0, 0);
    game_background = create_sprite(table_xpm, 0, 0, 0, 0);
    deal_deck = create_sprite(deck_xpm, 780, 160, 0, 0);
    close_button = create_sprite(close_button_xpm, 20, 20, 0, 0);
    coin_sprite = create_sprite(coin_xpm, 0, 0, 0, 0);
    coin_shadow_sprite = create_sprite(coin_shadow_xpm, 0, 0, 0, 0);
    
    mouse_cursor = create_sprite(mouse_cursor_xpm, 0, 0, 0, 0);
    mouse_cursor_click = create_sprite(mouse_cursor_click_xpm, 0, 0, 0, 0);

    card_back = create_sprite(back_xpm, 0, 0, -4, 8);

    clubs_2 = create_sprite(clubs_2_xpm, 0, 0, 0, 0);
    clubs_3 = create_sprite(clubs_3_xpm, 0, 0, 0, 0);
    clubs_4 = create_sprite(clubs_4_xpm, 0, 0, 0, 0);
    clubs_5 = create_sprite(clubs_5_xpm, 0, 0, 0, 0);
    clubs_6 = create_sprite(clubs_6_xpm, 0, 0, 0, 0);
    clubs_7 = create_sprite(clubs_7_xpm, 0, 0, 0, 0);
    clubs_8 = create_sprite(clubs_8_xpm, 0, 0, 0, 0);
    clubs_9 = create_sprite(clubs_9_xpm, 0, 0, 0, 0);
    clubs_10 = create_sprite(clubs_10_xpm, 0, 0, 0, 0);
    clubs_J = create_sprite(clubs_J_xpm, 0, 0, 0, 0);
    clubs_Q = create_sprite(clubs_Q_xpm, 0, 0, 0, 0);
    clubs_K = create_sprite(clubs_K_xpm, 0, 0, 0, 0);
    clubs_A = create_sprite(clubs_A_xpm, 0, 0, 0, 0);

    diamonds_2 = create_sprite(diamonds_2_xpm, 0, 0, 0, 0);
    diamonds_3 = create_sprite(diamonds_3_xpm, 0, 0, 0, 0);
    diamonds_4 = create_sprite(diamonds_4_xpm, 0, 0, 0, 0);
    diamonds_5 = create_sprite(diamonds_5_xpm, 0, 0, 0, 0);
    diamonds_6 = create_sprite(diamonds_6_xpm, 0, 0, 0, 0);
    diamonds_7 = create_sprite(diamonds_7_xpm, 0, 0, 0, 0);
    diamonds_8 = create_sprite(diamonds_8_xpm, 0, 0, 0, 0);
    diamonds_9 = create_sprite(diamonds_9_xpm, 0, 0, 0, 0);
    diamonds_10 = create_sprite(diamonds_10_xpm, 0, 0, 0, 0);
    diamonds_J = create_sprite(diamonds_J_xpm, 0, 0, 0, 0);
    diamonds_Q = create_sprite(diamonds_Q_xpm, 0, 0, 0, 0);
    diamonds_K = create_sprite(diamonds_K_xpm, 0, 0, 0, 0);
    diamonds_A = create_sprite(diamonds_A_xpm, 0, 0, 0, 0);

    hearts_2 = create_sprite(hearts_2_xpm, 0, 0, 0, 0);
    hearts_3 = create_sprite(hearts_3_xpm, 0, 0, 0, 0);
    hearts_4 = create_sprite(hearts_4_xpm, 0, 0, 0, 0);
    hearts_5 = create_sprite(hearts_5_xpm, 0, 0, 0, 0);
    hearts_6 = create_sprite(hearts_6_xpm, 0, 0, 0, 0);
    hearts_7 = create_sprite(hearts_7_xpm, 0, 0, 0, 0);
    hearts_8 = create_sprite(hearts_8_xpm, 0, 0, 0, 0);
    hearts_9 = create_sprite(hearts_9_xpm, 0, 0, 0, 0);
    hearts_10 = create_sprite(hearts_10_xpm, 0, 0, 0, 0);
    hearts_J = create_sprite(hearts_J_xpm, 0, 0, 0, 0);
    hearts_Q = create_sprite(hearts_Q_xpm, 0, 0, 0, 0);
    hearts_K = create_sprite(hearts_K_xpm, 0, 0, 0, 0);
    hearts_A = create_sprite(hearts_A_xpm, 0, 0, 0, 0);

    spades_2 = create_sprite(spades_2_xpm, 0, 0, 0, 0);
    spades_3 = create_sprite(spades_3_xpm, 0, 0, 0, 0);
    spades_4 = create_sprite(spades_4_xpm, 0, 0, 0, 0);
    spades_5 = create_sprite(spades_5_xpm, 0, 0, 0, 0);
    spades_6 = create_sprite(spades_6_xpm, 0, 0, 0, 0);
    spades_7 = create_sprite(spades_7_xpm, 0, 0, 0, 0);
    spades_8 = create_sprite(spades_8_xpm, 0, 0, 0, 0);
    spades_9 = create_sprite(spades_9_xpm, 0, 0, 0, 0);
    spades_10 = create_sprite(spades_10_xpm, 0, 0, 0, 0);
    spades_J = create_sprite(spades_J_xpm, 0, 0, 0, 0);
    spades_Q = create_sprite(spades_Q_xpm, 0, 0, 0, 0);
    spades_K = create_sprite(spades_K_xpm, 0, 0, 0, 0);
    spades_A = create_sprite(spades_A_xpm, 0, 0, 0, 0);

    a = create_sprite(a_xpm, 0, 0, 0, 0);
    b = create_sprite(b_xpm, 0, 0, 0, 0);
    c = create_sprite(c_xpm, 0, 0, 0, 0);
    d = create_sprite(d_xpm, 0, 0, 0, 0);
    e = create_sprite(e_xpm, 0, 0, 0, 0);
    f = create_sprite(f_xpm, 0, 0, 0, 0);
    g = create_sprite(g_xpm, 0, 0, 0, 0);
    h = create_sprite(h_xpm, 0, 0, 0, 0);
    i = create_sprite(i_xpm, 0, 0, 0, 0);
    j = create_sprite(j_xpm, 0, 0, 0, 0);
    k = create_sprite(k_xpm, 0, 0, 0, 0);
    l = create_sprite(l_xpm, 0, 0, 0, 0);
    m = create_sprite(m_xpm, 0, 0, 0, 0);
    n = create_sprite(n_xpm, 0, 0, 0, 0);
    o = create_sprite(o_xpm, 0, 0, 0, 0);
    p = create_sprite(p_xpm, 0, 0, 0, 0);
    q = create_sprite(q_xpm, 0, 0, 0, 0);
    r = create_sprite(r_xpm, 0, 0, 0, 0);
    s = create_sprite(s_xpm, 0, 0, 0, 0);
    t = create_sprite(t_xpm, 0, 0, 0, 0);
    u = create_sprite(u_xpm, 0, 0, 0, 0);
    v = create_sprite(v_xpm, 0, 0, 0, 0);
    w = create_sprite(w_xpm, 0, 0, 0, 0);
    x = create_sprite(x_xpm, 0, 0, 0, 0);
    y = create_sprite(y_xpm, 0, 0, 0, 0);
    z = create_sprite(z_xpm, 0, 0, 0, 0);
    zero = create_sprite(zero_xpm, 0, 0, 0, 0);
    one = create_sprite(one_xpm, 0, 0, 0, 0);
    two = create_sprite(two_xpm, 0, 0, 0, 0);
    three = create_sprite(three_xpm, 0, 0, 0, 0);
    four = create_sprite(four_xpm, 0, 0, 0, 0);
    five = create_sprite(five_xpm, 0, 0, 0, 0);
    six = create_sprite(six_xpm, 0, 0, 0, 0);
    seven = create_sprite(seven_xpm, 0, 0, 0, 0);
    eight = create_sprite(eight_xpm, 0, 0, 0, 0);
    nine = create_sprite(nine_xpm, 0, 0, 0, 0);
    space = create_sprite(space_xpm, 0, 0, 0, 0);
    hyphen = create_sprite(hyphen_xpm, 0, 0, 0, 0);
    colon = create_sprite(colon_xpm, 0, 0, 0, 0);
}

void (destroy_sprites)() {
    destroy_sprite(menu_background);
    destroy_sprite(create_player_background);
    destroy_sprite(choose_player_background);
    destroy_sprite(game_background);
    destroy_sprite(deal_deck);
    destroy_sprite(close_button);
    destroy_sprite(coin_sprite);
    destroy_sprite(coin_shadow_sprite);

    destroy_sprite(mouse_cursor);
    destroy_sprite(mouse_cursor_click);
    
    destroy_sprite(card_back);

    destroy_sprite(clubs_2);
    destroy_sprite(clubs_3);
    destroy_sprite(clubs_4);
    destroy_sprite(clubs_5);
    destroy_sprite(clubs_6);
    destroy_sprite(clubs_7);
    destroy_sprite(clubs_8);
    destroy_sprite(clubs_9);
    destroy_sprite(clubs_10);
    destroy_sprite(clubs_J);
    destroy_sprite(clubs_Q);
    destroy_sprite(clubs_K);
    destroy_sprite(clubs_A);

    destroy_sprite(diamonds_2);
    destroy_sprite(diamonds_3);
    destroy_sprite(diamonds_4);
    destroy_sprite(diamonds_5);
    destroy_sprite(diamonds_6);
    destroy_sprite(diamonds_7);
    destroy_sprite(diamonds_8);
    destroy_sprite(diamonds_9);
    destroy_sprite(diamonds_10);
    destroy_sprite(diamonds_J);
    destroy_sprite(diamonds_Q);
    destroy_sprite(diamonds_K);
    destroy_sprite(diamonds_A);

    destroy_sprite(hearts_2);
    destroy_sprite(hearts_3);
    destroy_sprite(hearts_4);
    destroy_sprite(hearts_5);
    destroy_sprite(hearts_6);
    destroy_sprite(hearts_7);
    destroy_sprite(hearts_8);
    destroy_sprite(hearts_9);
    destroy_sprite(hearts_10);
    destroy_sprite(hearts_J);
    destroy_sprite(hearts_Q);
    destroy_sprite(hearts_K);
    destroy_sprite(hearts_A);

    destroy_sprite(spades_2);
    destroy_sprite(spades_3);
    destroy_sprite(spades_4);
    destroy_sprite(spades_5);
    destroy_sprite(spades_6);
    destroy_sprite(spades_7);
    destroy_sprite(spades_8);
    destroy_sprite(spades_9);
    destroy_sprite(spades_10);
    destroy_sprite(spades_J);
    destroy_sprite(spades_Q);
    destroy_sprite(spades_K);
    destroy_sprite(spades_A);

    destroy_sprite(a);
    destroy_sprite(b);
    destroy_sprite(c);
    destroy_sprite(d);
    destroy_sprite(e);
    destroy_sprite(f);
    destroy_sprite(g);
    destroy_sprite(h);
    destroy_sprite(i);
    destroy_sprite(j);
    destroy_sprite(k);
    destroy_sprite(l);
    destroy_sprite(m);
    destroy_sprite(n);
    destroy_sprite(o);
    destroy_sprite(p);
    destroy_sprite(q);
    destroy_sprite(r);
    destroy_sprite(s);
    destroy_sprite(t);
    destroy_sprite(u);
    destroy_sprite(v);
    destroy_sprite(w);
    destroy_sprite(x);
    destroy_sprite(y);
    destroy_sprite(z);
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
    destroy_sprite(space);
    destroy_sprite(hyphen);
    destroy_sprite(colon);
}

Sprite* (find_sprite)(const char *name) {
    for (int i = 0; sprite_table[i].name != NULL; i++) {
        if (strcmp(sprite_table[i].name, name) == 0) {
            return *sprite_table[i].sprite;
        }
    }
    return NULL;
}
