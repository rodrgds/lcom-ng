#include "sprite.h"

#define CREATE_SPRITE(data) *(data.sprite_ref) = createSprite((xpm_map_t)data.xpm)

typedef struct {
    char **xpm;
    Sprite **sprite_ref;
} SpriteData;

Sprite *createSprite(xpm_map_t sprite) {

  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sp == NULL) return NULL;

  sp->map = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);

  if (sp->map == NULL) {
    free(sp);
    return NULL;
  }

  sp->height = img.height;
  sp->width = img.width;

  return sp;
}

void destroy_sprite(Sprite *sp) {
  if (sp == NULL) return;
  if (sp->map) free(sp->map);
  free(sp);
  sp = NULL;
}

int drawSprite(Sprite *sprite, int x, int y) {
  uint16_t height = sprite->height, width = sprite->width;
  uint32_t current_color;

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      current_color = sprite->map[w + h * width];
      if (current_color == DEFOULT_COLOR) continue;
      int r = vg_draw_pixel(x + w, y + h, sprite->map[w + h * width]);
      if (r == 1) continue;
      if (r == 2) return 1;
    }
  }
  return 0;
}

// screate spries and store in array
void createSpritesFromMaps(Sprite** sprites, char*** xpms, int num) {
  for (int i = 0; i < num; ++i)
    sprites[i] = createSprite((xpm_map_t) xpms[i]);
}

void createCardsSprites(){
  char **card_xpms[33] = {
    Back_xpm, _6_of_clubs_xpm, _6_of_diamonds_xpm, _6_of_hearts_xpm, _6_of_spades_xpm,
    _7_of_clubs_xpm, _7_of_diamonds_xpm, _7_of_hearts_xpm, _7_of_spades_xpm,
    _8_of_clubs_xpm, _8_of_diamonds_xpm, _8_of_hearts_xpm, _8_of_spades_xpm,
    _9_of_clubs_xpm, _9_of_diamonds_xpm, _9_of_hearts_xpm, _9_of_spades_xpm,
    _10_of_clubs_xpm, _10_of_diamonds_xpm, _10_of_hearts_xpm, _10_of_spades_xpm,
    jack_of_clubs_xpm, jack_of_diamonds_xpm, jack_of_hearts_xpm, jack_of_spades_xpm,
    queen_of_clubs_xpm, queen_of_diamonds_xpm, queen_of_hearts_xpm, queen_of_spades_xpm,
    king_of_clubs_xpm, king_of_diamonds_xpm, king_of_hearts_xpm, king_of_spades_xpm};

  createSpritesFromMaps(cards, card_xpms, 33);
}

void createCoinsSprites(){
  char **coin_xpms[8] = {
    coin5_xpm, coin25_xpm, coin10_xpm, coin100_xpm,
    yellow5coin_xpm, yellow25coin_xpm, yellow10coin_xpm, yellow100coin_xpm};

  createSpritesFromMaps(coins, coin_xpms, 8);
}

void createLettersSprites(){
  char **letter_xpms[26] = {
    a_xpm, b_xpm, c_xpm, d_xpm, e_xpm, f_xpm, g_xpm, h_xpm, i_xpm, j_xpm,
    k_xpm, l_xpm, m_xpm, n_xpm, o_xpm, p_xpm, q_xpm, r_xpm, s_xpm, t_xpm,
    u_xpm, v_xpm, w_xpm, x_xpm, y_xpm, z_xpm};

  createSpritesFromMaps(letters, letter_xpms, 26);  
}

void createNumbersSprites(){
  char **number_xpms[10] = {
    _0_xpm, _1_xpm, _2_xpm, _3_xpm, _4_xpm, _5_xpm, _6_xpm, _7_xpm, _8_xpm, _9_xpm};

  createSpritesFromMaps(numbers, number_xpms, 10);   
}


void createSprites(SpriteData* spriteData, size_t spriteDataCount) {
    for (size_t i = 0; i < spriteDataCount; ++i) {
        CREATE_SPRITE(spriteData[i]);
    }
}

void createAllSprites() {
    SpriteData spriteData[] = {
        {menu_xpm, &background},
        {cursor_xpm, &cursor},
        {cursorClick_xpm, &cursorClick},
        {ExitNotPressed_xpm, &Exit},
        {ExitPressed_xpm, &ExitPressed},
        {StartNotPressed_xpm, &Start},
        {StartPressed_xpm, &StartPressed},
        {multiplayer_not_xpm, &multiplayer_not},
        {multiplayer_xpm, &multiplayer},
        {wait_xpm, &wait},
        {Hit_xpm, &Hit},
        {HitPressed_xpm, &HitPressed},
        {Stand_xpm, &Stand},
        {StandPressed_xpm, &StandPressed},
        {Loss_xpm, &Loss},
        {Win_xpm, &Win},
        {Tie_xpm, &Tie},
        {open_hand_xpm, &hand},
        {closed_hand_xpm, &handCloused},
        {edit_icon_xpm, &edit_icon},
        {doubleDot_xpm, &doubleDot},
        {table_xpm, &table},
        {table_pressed_xpm, &table_pressed},
        {red_cross_xpm, &red_cross},
        {refrashe_xpm, &refrashe}
    };

    createSprites(spriteData, sizeof(spriteData) / sizeof(SpriteData));

    // Card, coin, letter, number creation
    createCardsSprites();
    createCoinsSprites();
    createLettersSprites();
    createNumbersSprites();
}


void destroySprites(Sprite** sprites, int numSprites) {
  for (int i = 0; i < numSprites; i++) {
    destroy_sprite(sprites[i]);
  }
  free(sprites);
}

void destroyAllSprites() {
  Sprite* sprites[] = {
        background, cursor, Exit, ExitPressed, Start, StartPressed,multiplayer_not,multiplayer,
        cursorClick, Hit, HitPressed, Stand, StandPressed,
        Loss, Win, Tie, handCloused, hand, edit_icon, doubleDot, table,
        table_pressed, red_cross, refrashe};

  int numSprites = sizeof(sprites) / sizeof(sprites[0]);

  destroySprites(sprites, numSprites);
  destroySprites(cards, 33);
  destroySprites(coins, 8);
  destroySprites(numbers, 10);
  destroySprites(letters, 27);
}
