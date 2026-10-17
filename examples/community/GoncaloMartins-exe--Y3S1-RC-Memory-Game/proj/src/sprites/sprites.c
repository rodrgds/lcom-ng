#include "sprites.h"

#include <math.h>

Sprite *digit_0, *digit_1, *digit_2, *digit_3, *digit_4;
Sprite *digit_5, *digit_6, *digit_7, *digit_8, *digit_9;
Sprite *digit_colon;

Sprite *s_T, *s_I, *s_M, *s_E;
Sprite *s_A, *s_K, *s_N;

/**
 * @brief Creates a new sprite from an XPM map.
 * 
 * @param spriteMap The XPM map to convert into a sprite.
 * @return Pointer to the newly created Sprite, or NULL if allocation fails.
 */

Sprite *sprite_novo(xpm_map_t spriteMap) {
  Sprite *novoSprite = (Sprite *) malloc(sizeof(Sprite));
  if (!novoSprite) return NULL;

  xpm_image_t img;
  novoSprite->colors = (uint32_t *) xpm_load(spriteMap, XPM_8_8_8_8, &img);
  novoSprite->width = img.width;
  novoSprite->height = img.height;

  if (!novoSprite->colors) {
    free(novoSprite);
    return NULL;
  }

  return novoSprite;
}

/**
 * @brief Frees the memory used by a sprite.
 * 
 * @param sprite Pointer to the sprite to destroy.
 */

void sprite_destruir(Sprite *sprite) {
  if (!sprite || !(sprite)) return;

  if ((sprite)->colors) free((sprite)->colors);
  free(sprite);
  sprite = NULL;
}

/**
 * @brief Draws a sprite on screen at the specified position.
 * 
 * @param sprite The sprite to draw.
 * @param posX The X coordinate on screen.
 * @param posY The Y coordinate on screen.
 * @return 0 on success, 1 on error.
 */

int sprite_desenhar(Sprite *sprite, int posX, int posY) {
  if (!sprite) return 1;

  uint16_t largura = sprite->width;
  uint16_t altura = sprite->height;

  for (int h = 0; h < altura; h++) {
    for (int w = 0; w < largura; w++) {
      uint32_t raw_color = sprite->colors[w + h * largura];

      uint8_t a = (raw_color >> 24) & 0xFF;
      if (a != 0x00) continue;
      // uint8_t r = (raw_color >> 16) & 0xFF;
      // uint8_t g = (raw_color >> 8)  & 0xFF;
      // uint8_t b = raw_color & 0xFF;

      //uint32_t cor_index = rgb_to_minix_index(r, g, b);
      //printf("color index %x\n", raw_color);
      if (my_vg_draw_pixel(posX + w, posY + h, raw_color)) return 1;
    }
  }

  return 0;
}

/**
 * @brief Converts an RGB color to the closest matching Minix 16-color palette index.
 * 
 * @param r Red component (0–255)
 * @param g Green component (0–255)
 * @param b Blue component (0–255)
 * @return Index (0–15) of the closest color in the Minix palette.
 */

uint8_t rgb_to_minix_index(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t best_index = 0;
  double min_distance = 1e9;

  for (uint8_t i = 0; i < 16; i++) {
    int dr = r - minix_palette[i].r;
    int dg = g - minix_palette[i].g;
    int db = b - minix_palette[i].b;

    double distance = dr * dr + dg * dg + db * db;
    if (distance < min_distance) {
      min_distance = distance;
      best_index = i;
    }
  }

  return best_index;
}
/**
 * @brief Loads all sprites used in the game, including the mouse cursor.
 */

void sprites_carregar_1(){
  loading1 = sprite_novo((xpm_map_t) loading1_xpm);
  instructions = sprite_novo((xpm_map_t) Instructions_xpm);
}
void sprites_carregar_2(){
  loading2 = sprite_novo((xpm_map_t) loading2_xpm);

  background = sprite_novo((xpm_map_t) background_xpm);
}
void sprites_carregar_3(){
  loading3 = sprite_novo((xpm_map_t) loading3_xpm);

  c1 = sprite_novo((xpm_map_t) carta1_xpm);
  c2 = sprite_novo((xpm_map_t) carta2_xpm);
  c3 = sprite_novo((xpm_map_t) carta3_xpm);
  c4 = sprite_novo((xpm_map_t) carta4_xpm);
  c5 = sprite_novo((xpm_map_t) carta5_xpm);
  c6 = sprite_novo((xpm_map_t) carta6_xpm);
  c7 = sprite_novo((xpm_map_t) carta7_xpm);
  c8 = sprite_novo((xpm_map_t) carta8_xpm);
  c9 = sprite_novo((xpm_map_t) carta9_xpm);
}
void sprites_carregar_4(){
  loading4 = sprite_novo((xpm_map_t) loading4_xpm);

  cartaAbacate1 = sprite_novo((xpm_map_t) cartaAbacate1_xpm);
  carregar_cartas_fruta(ABACATE);
}
void sprites_carregar_5(){
  loading5 = sprite_novo((xpm_map_t) loading5_xpm);

  cartaBanana1 = sprite_novo((xpm_map_t) cartaBanana1_xpm);
  carregar_cartas_fruta(BANANA);
}
void sprites_carregar_6(){
  loading6 = sprite_novo((xpm_map_t) loading6_xpm);

  cartaCereija1  = sprite_novo((xpm_map_t) cartaCereija1_xpm);
  carregar_cartas_fruta(CEREIJA);
}
void sprites_carregar_7(){
  loading7 = sprite_novo((xpm_map_t) loading7_xpm);

  cartaCoco1 = sprite_novo((xpm_map_t) cartaCoco1_xpm);
  carregar_cartas_fruta(COCO);
}
void sprites_carregar_8(){
  loading8 = sprite_novo((xpm_map_t) loading8_xpm);

  cartaLaranja1 = sprite_novo((xpm_map_t) cartaLaranja1_xpm);
  carregar_cartas_fruta(LARANJA);
}
void sprites_carregar_9(){
  loading9 = sprite_novo((xpm_map_t) loading9_xpm);

  cartaMaca1 = sprite_novo((xpm_map_t) cartaMaca1_xpm);
  carregar_cartas_fruta(MACA);
}
void sprites_carregar_10(){
  loading10 = sprite_novo((xpm_map_t) loading10_xpm);

  cartaMelancia1 = sprite_novo((xpm_map_t) cartaMelancia1_xpm); 
  carregar_cartas_fruta(MELANCIA);
}
void sprites_carregar_11(){
  loading11 = sprite_novo((xpm_map_t) loading11_xpm);

  cartaMorango1 = sprite_novo((xpm_map_t) cartaMorango1_xpm);
  carregar_cartas_fruta(MORANGO);
}
void sprites_carregar_12(){
  loading12 = sprite_novo((xpm_map_t) loading12_xpm);

  mouseCursorSprite = sprite_novo((xpm_map_t) mousecursor_xpm);

  cursor_x = 400;
  cursor_y = 300;

  letter_A = sprite_novo((xpm_map_t) letterA_xpm);
  letter_B = sprite_novo((xpm_map_t) letterB_xpm);
  letter_C = sprite_novo((xpm_map_t) letterC_xpm);
  letter_D = sprite_novo((xpm_map_t) letterD_xpm);
  letter_E = sprite_novo((xpm_map_t) letterE_xpm);
  letter_F = sprite_novo((xpm_map_t) letterF_xpm);
  letter_G = sprite_novo((xpm_map_t) letterG_xpm);
  letter_H = sprite_novo((xpm_map_t) letterH_xpm);
  letter_I = sprite_novo((xpm_map_t) letterI_xpm);
  letter_J = sprite_novo((xpm_map_t) letterJ_xpm);
  letter_K = sprite_novo((xpm_map_t) letterK_xpm);
}
void sprites_carregar_13(){
  loading13 = sprite_novo((xpm_map_t) loading13_xpm);

  letter_L = sprite_novo((xpm_map_t) letterL_xpm);
  letter_M = sprite_novo((xpm_map_t) letterM_xpm);
  letter_N = sprite_novo((xpm_map_t) letterN_xpm);
  letter_O = sprite_novo((xpm_map_t) letterO_xpm);
  letter_P = sprite_novo((xpm_map_t) letterP_xpm);
  letter_Q = sprite_novo((xpm_map_t) letterQ_xpm);
  letter_R = sprite_novo((xpm_map_t) letterR_xpm);
  letter_S = sprite_novo((xpm_map_t) letterS_xpm);
  letter_T = sprite_novo((xpm_map_t) letterT_xpm);
}
void sprites_carregar_14(){
  loading14 = sprite_novo((xpm_map_t) loading14_xpm);

  letter_U = sprite_novo((xpm_map_t) letterU_xpm);
  letter_V = sprite_novo((xpm_map_t) letterV_xpm);
  letter_W = sprite_novo((xpm_map_t) letterW_xpm);
  letter_X = sprite_novo((xpm_map_t) letterX_xpm);
  letter_Y = sprite_novo((xpm_map_t) letterY_xpm);
  letter_Z = sprite_novo((xpm_map_t) letterZ_xpm);
  letter_SPACE = sprite_novo((xpm_map_t) letterSPACE_xpm);
}
void sprites_carregar_15(){
  loading15 = sprite_novo((xpm_map_t) loading15_xpm);

  digit_0     = sprite_novo((xpm_map_t) char_0);
  digit_1     = sprite_novo((xpm_map_t) char_1);
  digit_2     = sprite_novo((xpm_map_t) char_2);
  digit_3     = sprite_novo((xpm_map_t) char_3);
  digit_4     = sprite_novo((xpm_map_t) char_4);
  digit_5     = sprite_novo((xpm_map_t) char_5);
  digit_6     = sprite_novo((xpm_map_t) char_6);
  digit_7     = sprite_novo((xpm_map_t) char_7);
  digit_8     = sprite_novo((xpm_map_t) char_8);
  digit_9     = sprite_novo((xpm_map_t) char_9);
  digit_colon = sprite_novo((xpm_map_t) char_colon);

  s_T = sprite_novo((xpm_map_t)(char_T));
  s_I = sprite_novo((xpm_map_t)(char_I));
  s_M = sprite_novo((xpm_map_t)(char_M));
  s_E = sprite_novo((xpm_map_t)(char_E));
  s_A = sprite_novo((xpm_map_t)(char_A));
  s_K = sprite_novo((xpm_map_t)(char_K));
  s_N = sprite_novo((xpm_map_t)(char_N));

}
void sprites_carregar_16(){
  loading16 = sprite_novo((xpm_map_t) loading16_xpm);
}

// Carrega todos os sprites usados no jogo
void sprites_carregar() {return;}

void carregar_cartas_fruta(Fruta fruta) {
  switch (fruta) {
    case ABACATE:
      if(cartaAbacate2 == NULL){
        cartaAbacate2 = sprite_novo((xpm_map_t) cartaAbacate2_xpm);
        cartaAbacate3 = sprite_novo((xpm_map_t) cartaAbacate3_xpm);
        cartaAbacate4 = sprite_novo((xpm_map_t) cartaAbacate4_xpm);
        cartaAbacate5 = sprite_novo((xpm_map_t) cartaAbacate5_xpm);
        cartaAbacate6 = sprite_novo((xpm_map_t) cartaAbacate6_xpm);
        cartaAbacate7 = sprite_novo((xpm_map_t) cartaAbacate7_xpm);
        cartaAbacate8 = sprite_novo((xpm_map_t) cartaAbacate8_xpm);
        
        abacate_frames[0] = c9;
        abacate_frames[1] = c9;
        abacate_frames[2] = c8;
        abacate_frames[3] = c8;
        abacate_frames[4] = c7;
        abacate_frames[5] = c7;
        abacate_frames[6] = c6;
        abacate_frames[7] = c6;
        abacate_frames[8] = c5;
        abacate_frames[9] = c5;
        abacate_frames[10] = c4;
        abacate_frames[11] = c4;
        abacate_frames[12] = c3;
        abacate_frames[13] = c3;
        abacate_frames[14] = c2;
        abacate_frames[15] = c2;
        abacate_frames[16] = c1;
        abacate_frames[17] = c1;
        abacate_frames[18] = cartaAbacate8;
        abacate_frames[19] = cartaAbacate8;
        abacate_frames[20] = cartaAbacate7;
        abacate_frames[21] = cartaAbacate7;
        abacate_frames[22] = cartaAbacate6;
        abacate_frames[23] = cartaAbacate6;
        abacate_frames[24] = cartaAbacate5;
        abacate_frames[25] = cartaAbacate5;
        abacate_frames[26] = cartaAbacate4;
        abacate_frames[27] = cartaAbacate4;
        abacate_frames[28] = cartaAbacate3;
        abacate_frames[29] = cartaAbacate3;
        abacate_frames[30] = cartaAbacate2;
        abacate_frames[31] = cartaAbacate2;
        abacate_frames[32] = cartaAbacate1;
        abacate_frames[33] = cartaAbacate1;
      }
      break;

    case BANANA:
      if(cartaBanana2 == NULL){
        cartaBanana2 = sprite_novo((xpm_map_t) cartaBanana2_xpm);
        cartaBanana3 = sprite_novo((xpm_map_t) cartaBanana3_xpm);
        cartaBanana4 = sprite_novo((xpm_map_t) cartaBanana4_xpm);
        cartaBanana5 = sprite_novo((xpm_map_t) cartaBanana5_xpm);
        cartaBanana6 = sprite_novo((xpm_map_t) cartaBanana6_xpm);
        cartaBanana7 = sprite_novo((xpm_map_t) cartaBanana7_xpm);
        cartaBanana8 = sprite_novo((xpm_map_t) cartaBanana8_xpm);

        banana_frames[0] = c9;
        banana_frames[1] = c9;
        banana_frames[2] = c8;
        banana_frames[3] = c8;
        banana_frames[4] = c7;
        banana_frames[5] = c7;
        banana_frames[6] = c6;
        banana_frames[7] = c6;
        banana_frames[8] = c5;
        banana_frames[9] = c5;
        banana_frames[10] = c4;
        banana_frames[11] = c4;
        banana_frames[12] = c3;
        banana_frames[13] = c3;
        banana_frames[14] = c2;
        banana_frames[15] = c2;
        banana_frames[16] = c1;
        banana_frames[17] = c1;
        banana_frames[18] = cartaBanana8;
        banana_frames[19] = cartaBanana8;
        banana_frames[20] = cartaBanana7;
        banana_frames[21] = cartaBanana7;
        banana_frames[22] = cartaBanana6;
        banana_frames[23] = cartaBanana6;
        banana_frames[24] = cartaBanana5;
        banana_frames[25] = cartaBanana5;
        banana_frames[26] = cartaBanana4;
        banana_frames[27] = cartaBanana4;
        banana_frames[28] = cartaBanana3;
        banana_frames[29] = cartaBanana3;
        banana_frames[30] = cartaBanana2;
        banana_frames[31] = cartaBanana2;
        banana_frames[32] = cartaBanana1;
        banana_frames[33] = cartaBanana1;

      }
      break;

    case CEREIJA:
      if(cartaCereija2 == NULL){
        cartaCereija2 = sprite_novo((xpm_map_t) cartaCereija2_xpm);
        cartaCereija3 = sprite_novo((xpm_map_t) cartaCereija3_xpm);
        cartaCereija4 = sprite_novo((xpm_map_t) cartaCereija4_xpm);
        cartaCereija5 = sprite_novo((xpm_map_t) cartaCereija5_xpm);
        cartaCereija6 = sprite_novo((xpm_map_t) cartaCereija6_xpm);
        cartaCereija7 = sprite_novo((xpm_map_t) cartaCereija7_xpm);
        cartaCereija8 = sprite_novo((xpm_map_t) cartaCereija8_xpm);

        cereija_frames[0] = c9;
        cereija_frames[1] = c9;
        cereija_frames[2] = c8;
        cereija_frames[3] = c8;
        cereija_frames[4] = c7;
        cereija_frames[5] = c7;
        cereija_frames[6] = c6;
        cereija_frames[7] = c6;
        cereija_frames[8] = c5;
        cereija_frames[9] = c5;
        cereija_frames[10] = c4;
        cereija_frames[11] = c4;
        cereija_frames[12] = c3;
        cereija_frames[13] = c3;
        cereija_frames[14] = c2;
        cereija_frames[15] = c2;
        cereija_frames[16] = c1;
        cereija_frames[17] = c1;
        cereija_frames[18] = cartaCereija8;
        cereija_frames[19] = cartaCereija8;
        cereija_frames[20] = cartaCereija7;
        cereija_frames[21] = cartaCereija7;
        cereija_frames[22] = cartaCereija6;
        cereija_frames[23] = cartaCereija6;
        cereija_frames[24] = cartaCereija5;
        cereija_frames[25] = cartaCereija5;
        cereija_frames[26] = cartaCereija4;
        cereija_frames[27] = cartaCereija4;
        cereija_frames[28] = cartaCereija3;
        cereija_frames[29] = cartaCereija3;
        cereija_frames[30] = cartaCereija2;
        cereija_frames[31] = cartaCereija2;
        cereija_frames[32] = cartaCereija1;
        cereija_frames[33] = cartaCereija1;
      }
      break;

    case COCO:
      if(cartaCoco2 == NULL){
        cartaCoco2 = sprite_novo((xpm_map_t) cartaCoco2_xpm);
        cartaCoco3 = sprite_novo((xpm_map_t) cartaCoco3_xpm);
        cartaCoco4 = sprite_novo((xpm_map_t) cartaCoco4_xpm);
        cartaCoco5 = sprite_novo((xpm_map_t) cartaCoco5_xpm);
        cartaCoco6 = sprite_novo((xpm_map_t) cartaCoco6_xpm);
        cartaCoco7 = sprite_novo((xpm_map_t) cartaCoco7_xpm);
        cartaCoco8 = sprite_novo((xpm_map_t) cartaCoco8_xpm);

        coco_frames[0] = c9;
        coco_frames[1] = c9;
        coco_frames[2] = c8;
        coco_frames[3] = c8;
        coco_frames[4] = c7;
        coco_frames[5] = c7;
        coco_frames[6] = c6;
        coco_frames[7] = c6;
        coco_frames[8] = c5;
        coco_frames[9] = c5;
        coco_frames[10] = c4;
        coco_frames[11] = c4;
        coco_frames[12] = c3;
        coco_frames[13] = c3;
        coco_frames[14] = c2;
        coco_frames[15] = c2;
        coco_frames[16] = c1;
        coco_frames[17] = c1;
        coco_frames[18] = cartaCoco8;
        coco_frames[19] = cartaCoco8;
        coco_frames[20] = cartaCoco7;
        coco_frames[21] = cartaCoco7;
        coco_frames[22] = cartaCoco6;
        coco_frames[23] = cartaCoco6;
        coco_frames[24] = cartaCoco5;
        coco_frames[25] = cartaCoco5;
        coco_frames[26] = cartaCoco4;
        coco_frames[27] = cartaCoco4;
        coco_frames[28] = cartaCoco3;
        coco_frames[29] = cartaCoco3;
        coco_frames[30] = cartaCoco2;
        coco_frames[31] = cartaCoco2;
        coco_frames[32] = cartaCoco1;
        coco_frames[33] = cartaCoco1;
      }
      break;

    case LARANJA:
      if(cartaLaranja2 == NULL){
        cartaLaranja2 = sprite_novo((xpm_map_t) cartaLaranja2_xpm);
        cartaLaranja3 = sprite_novo((xpm_map_t) cartaLaranja3_xpm);
        cartaLaranja4 = sprite_novo((xpm_map_t) cartaLaranja4_xpm);
        cartaLaranja5 = sprite_novo((xpm_map_t) cartaLaranja5_xpm);
        cartaLaranja6 = sprite_novo((xpm_map_t) cartaLaranja6_xpm);
        cartaLaranja7 = sprite_novo((xpm_map_t) cartaLaranja7_xpm);
        cartaLaranja8 = sprite_novo((xpm_map_t) cartaLaranja8_xpm);

        laranja_frames[0] = c9;
        laranja_frames[1] = c9;
        laranja_frames[2] = c8;
        laranja_frames[3] = c8;
        laranja_frames[4] = c7;
        laranja_frames[5] = c7;
        laranja_frames[6] = c6;
        laranja_frames[7] = c6;
        laranja_frames[8] = c5;
        laranja_frames[9] = c5;
        laranja_frames[10] = c4;
        laranja_frames[11] = c4;
        laranja_frames[12] = c3;
        laranja_frames[13] = c3;
        laranja_frames[14] = c2;
        laranja_frames[15] = c2;
        laranja_frames[16] = c1;
        laranja_frames[17] = c1;
        laranja_frames[18] = cartaLaranja8;
        laranja_frames[19] = cartaLaranja8;
        laranja_frames[20] = cartaLaranja7;
        laranja_frames[21] = cartaLaranja7;
        laranja_frames[22] = cartaLaranja6;
        laranja_frames[23] = cartaLaranja6;
        laranja_frames[24] = cartaLaranja5;
        laranja_frames[25] = cartaLaranja5;
        laranja_frames[26] = cartaLaranja4;
        laranja_frames[27] = cartaLaranja4;
        laranja_frames[28] = cartaLaranja3;
        laranja_frames[29] = cartaLaranja3;
        laranja_frames[30] = cartaLaranja2;
        laranja_frames[31] = cartaLaranja2;
        laranja_frames[32] = cartaLaranja1;
        laranja_frames[33] = cartaLaranja1;
      }
      break;

    case MACA:
      if(cartaMaca2 == NULL){
        cartaMaca2 = sprite_novo((xpm_map_t) cartaMaca2_xpm);
        cartaMaca3 = sprite_novo((xpm_map_t) cartaMaca3_xpm);
        cartaMaca4 = sprite_novo((xpm_map_t) cartaMaca4_xpm);
        cartaMaca5 = sprite_novo((xpm_map_t) cartaMaca5_xpm);
        cartaMaca6 = sprite_novo((xpm_map_t) cartaMaca6_xpm);
        cartaMaca7 = sprite_novo((xpm_map_t) cartaMaca7_xpm);
        cartaMaca8 = sprite_novo((xpm_map_t) cartaMaca8_xpm);

        maca_frames[0] = c9;
        maca_frames[1] = c9;
        maca_frames[2] = c8;
        maca_frames[3] = c8;
        maca_frames[4] = c7;
        maca_frames[5] = c7;
        maca_frames[6] = c6;
        maca_frames[7] = c6;
        maca_frames[8] = c5;
        maca_frames[9] = c5;
        maca_frames[10] = c4;
        maca_frames[11] = c4;
        maca_frames[12] = c3;
        maca_frames[13] = c3;
        maca_frames[14] = c2;
        maca_frames[15] = c2;
        maca_frames[16] = c1;
        maca_frames[17] = c1;
        maca_frames[18] = cartaMaca8;
        maca_frames[19] = cartaMaca8;
        maca_frames[20] = cartaMaca7;
        maca_frames[21] = cartaMaca7;
        maca_frames[22] = cartaMaca6;
        maca_frames[23] = cartaMaca6;
        maca_frames[24] = cartaMaca5;
        maca_frames[25] = cartaMaca5;
        maca_frames[26] = cartaMaca4;
        maca_frames[27] = cartaMaca4;
        maca_frames[28] = cartaMaca3;
        maca_frames[29] = cartaMaca3;
        maca_frames[30] = cartaMaca2;
        maca_frames[31] = cartaMaca2;
        maca_frames[32] = cartaMaca1;
        maca_frames[33] = cartaMaca1;

      }
      break;

    case MELANCIA:
      if(cartaMelancia2 == NULL){
        cartaMelancia2 = sprite_novo((xpm_map_t) cartaMelancia2_xpm);
        cartaMelancia3 = sprite_novo((xpm_map_t) cartaMelancia3_xpm);
        cartaMelancia4 = sprite_novo((xpm_map_t) cartaMelancia4_xpm);
        cartaMelancia5 = sprite_novo((xpm_map_t) cartaMelancia5_xpm);
        cartaMelancia6 = sprite_novo((xpm_map_t) cartaMelancia6_xpm);
        cartaMelancia7 = sprite_novo((xpm_map_t) cartaMelancia7_xpm);
        cartaMelancia8 = sprite_novo((xpm_map_t) cartaMelancia8_xpm);

        melancia_frames[0] = c9;
        melancia_frames[1] = c9;
        melancia_frames[2] = c8;
        melancia_frames[3] = c8;
        melancia_frames[4] = c7;
        melancia_frames[5] = c7;
        melancia_frames[6] = c6;
        melancia_frames[7] = c6;
        melancia_frames[8] = c5;
        melancia_frames[9] = c5;
        melancia_frames[10] = c4;
        melancia_frames[11] = c4;
        melancia_frames[12] = c3;
        melancia_frames[13] = c3;
        melancia_frames[14] = c2;
        melancia_frames[15] = c2;
        melancia_frames[16] = c1;
        melancia_frames[17] = c1;
        melancia_frames[18] = cartaMelancia8;
        melancia_frames[19] = cartaMelancia8;
        melancia_frames[20] = cartaMelancia7;
        melancia_frames[21] = cartaMelancia7;
        melancia_frames[22] = cartaMelancia6;
        melancia_frames[23] = cartaMelancia6;
        melancia_frames[24] = cartaMelancia5;
        melancia_frames[25] = cartaMelancia5;
        melancia_frames[26] = cartaMelancia4;
        melancia_frames[27] = cartaMelancia4;
        melancia_frames[28] = cartaMelancia3;
        melancia_frames[29] = cartaMelancia3;
        melancia_frames[30] = cartaMelancia2;
        melancia_frames[31] = cartaMelancia2;
        melancia_frames[32] = cartaMelancia1;
        melancia_frames[33] = cartaMelancia1;

      }
      break;

    case MORANGO:
      if(cartaMorango2 == NULL){
        cartaMorango2 = sprite_novo((xpm_map_t) cartaMorango2_xpm);
        cartaMorango3 = sprite_novo((xpm_map_t) cartaMorango3_xpm);
        cartaMorango4 = sprite_novo((xpm_map_t) cartaMorango4_xpm);
        cartaMorango5 = sprite_novo((xpm_map_t) cartaMorango5_xpm);
        cartaMorango6 = sprite_novo((xpm_map_t) cartaMorango6_xpm);
        cartaMorango7 = sprite_novo((xpm_map_t) cartaMorango7_xpm);
        cartaMorango8 = sprite_novo((xpm_map_t) cartaMorango8_xpm);

        morango_frames[0] = c9;
        morango_frames[1] = c9;
        morango_frames[2] = c8;
        morango_frames[3] = c8;
        morango_frames[4] = c7;
        morango_frames[5] = c7;
        morango_frames[6] = c6;
        morango_frames[7] = c6;
        morango_frames[8] = c5;
        morango_frames[9] = c5;
        morango_frames[10] = c4;
        morango_frames[11] = c4;
        morango_frames[12] = c3;
        morango_frames[13] = c3;
        morango_frames[14] = c2;
        morango_frames[15] = c2;
        morango_frames[16] = c1;
        morango_frames[17] = c1;
        morango_frames[18] = cartaMorango8;
        morango_frames[19] = cartaMorango8;
        morango_frames[20] = cartaMorango7;
        morango_frames[21] = cartaMorango7;
        morango_frames[22] = cartaMorango6;
        morango_frames[23] = cartaMorango6;
        morango_frames[24] = cartaMorango5;
        morango_frames[25] = cartaMorango5;
        morango_frames[26] = cartaMorango4;
        morango_frames[27] = cartaMorango4;
        morango_frames[28] = cartaMorango3;
        morango_frames[29] = cartaMorango3;
        morango_frames[30] = cartaMorango2;
        morango_frames[31] = cartaMorango2;
        morango_frames[32] = cartaMorango1;
        morango_frames[33] = cartaMorango1;
      }
      break;
  }
}


/**
 * @brief Frees the memory of all loaded sprites.
 * 
 * This function should be called when the program is shutting down
 * or no longer needs the loaded sprites to free up memory.
 */

void sprites_liberar() {

  sprite_destruir(mouseCursorSprite);

  sprite_destruir(letter_A);
  sprite_destruir(letter_B);
  sprite_destruir(letter_C);
  sprite_destruir(letter_D);
  sprite_destruir(letter_E);
  sprite_destruir(letter_F);
  sprite_destruir(letter_G);
  sprite_destruir(letter_H);
  sprite_destruir(letter_I);
  sprite_destruir(letter_J);
  sprite_destruir(letter_K);
  sprite_destruir(letter_L);
  sprite_destruir(letter_M);
  sprite_destruir(letter_N);
  sprite_destruir(letter_O);
  sprite_destruir(letter_P);
  sprite_destruir(letter_Q);
  sprite_destruir(letter_R);
  sprite_destruir(letter_S);
  sprite_destruir(letter_T);
  sprite_destruir(letter_U);
  sprite_destruir(letter_V);
  sprite_destruir(letter_W);
  sprite_destruir(letter_X);
  sprite_destruir(letter_Y);
  sprite_destruir(letter_Z);
  sprite_destruir(letter_SPACE);

  sprite_destruir(c1);
  sprite_destruir(c2);
  sprite_destruir(c3);
  sprite_destruir(c4);
  sprite_destruir(c5);
  sprite_destruir(c6);
  sprite_destruir(c7);
  sprite_destruir(c8);
  sprite_destruir(c9);

  sprite_destruir(cartaAbacate1);
  sprite_destruir(cartaBanana1);
  sprite_destruir(cartaCereija1);
  sprite_destruir(cartaCoco1);  
  sprite_destruir(cartaLaranja1);
  sprite_destruir(cartaMaca1);
  sprite_destruir(cartaMelancia1);
  sprite_destruir(cartaMorango1);

  sprite_destruir(digit_0);
  sprite_destruir(digit_1);
  sprite_destruir(digit_2);
  sprite_destruir(digit_3);
  sprite_destruir(digit_4);
  sprite_destruir(digit_5);
  sprite_destruir(digit_6);
  sprite_destruir(digit_7);
  sprite_destruir(digit_8);
  sprite_destruir(digit_9);
  sprite_destruir(digit_colon);

  sprite_destruir(background);
  sprite_destruir(loading1);
  sprite_destruir(loading2);
  sprite_destruir(loading3);
  sprite_destruir(loading4);
  sprite_destruir(loading5);
  sprite_destruir(loading6);
  sprite_destruir(loading7);
  sprite_destruir(loading8);
  sprite_destruir(loading9);
  sprite_destruir(loading10);
  sprite_destruir(loading11);
  sprite_destruir(loading12);
  sprite_destruir(loading13);
  sprite_destruir(loading14);
  sprite_destruir(loading15);
  sprite_destruir(loading16);

  sprite_destruir(s_T);
  sprite_destruir(s_I);
  sprite_destruir(s_M);
  sprite_destruir(s_E);
  sprite_destruir(s_A);
  sprite_destruir(s_K);
  sprite_destruir(s_N);

}


/**
 * @brief Check if cursor is over a text line (for highlighting and selection)
 * @param x_start Starting X coordinate of the line
 * @param y Y coordinate of the line
 * @param letter_count Number of letters in the line
 * @param spacing Space between letters
 * @param letter_width Width of individual letters
 * @param letter_height Height of individual letters
 * @return true if cursor is over the line, false otherwise
 */
bool is_cursor_over_line(int x_start, int y, int letter_count, int spacing, int letter_width, int letter_height) {
  int x_end = x_start + (letter_width + spacing) * letter_count;
  return cursor_x >= x_start && cursor_x < x_end &&
         cursor_y >= y && cursor_y < y + letter_height;
}

/**
 * @brief Draw a line of text with optional highlighting
 * @param letters Array of sprite pointers for each letter
 * @param count Number of letters to draw
 * @param x_start Starting X coordinate
 * @param y Y coordinate
 * @param spacing Space between letters
 */
void draw_line(Sprite **letters, int count, int x_start, int y, int spacing) {
  bool highlight = is_cursor_over_line(x_start, y, count, spacing, letters[0]->width, letters[0]->height);

  int x = x_start;
  for (int i = 0; i < count; i++) {
    draw_letter_with_highlight(letters[i], x, y, highlight);

    // Apply reduced spacing for spaces
    if (letters[i] == letter_SPACE) {
      x += letters[i]->width + spacing / 15; 
    } else {
      x += letters[i]->width + spacing;
    }
  }
}

/**
 * @brief Draw a single letter with optional highlighting effect
 * @param letter Sprite pointer for the letter
 * @param x X coordinate
 * @param y Y coordinate
 * @param highlight Whether to apply highlight effect
 */
void draw_letter_with_highlight(Sprite *letter, int x, int y, bool highlight) {
  if (!letter || !letter->colors) return;

  for (int j = 0; j < letter->height; j++) {
    for (int i = 0; i < letter->width; i++) {
      uint32_t raw_color = letter->colors[j * letter->width + i];
      uint8_t a = (raw_color >> 24) & 0xFF;

      if (a == 0xFF) continue; // transparente, ignora

      uint8_t r = (raw_color >> 16) & 0xFF;
      uint8_t g = (raw_color >> 8) & 0xFF;
      uint8_t b = raw_color & 0xFF;

      if (highlight) {
        r = 255;
        g = 255;
        b = 0;
      }

      uint32_t new_color = (a << 24) | (r << 16) | (g << 8) | b;
      my_vg_draw_pixel(x + i, y + j, new_color);
    }
  }
}

/**
 * @brief Draw the main menu with highlighted options
 */
void desenhar_menu() {
  Sprite *start[] = {letter_S, letter_T, letter_A, letter_R, letter_T};
  Sprite *instructions[] = {letter_I, letter_N, letter_S, letter_T, letter_R, letter_U,
                            letter_C, letter_T, letter_I, letter_O, letter_N, letter_S};
  Sprite *quit[] = {letter_Q, letter_U, letter_I, letter_T};
  
  sprite_desenhar(background, 0, 0);

  draw_line(start, 5, 401, 200, 10);         // 451 - 50
  draw_line(instructions, 12, 226, 300, 10); // 276 - 50
  draw_line(quit, 4, 426, 400, 10);          // 476 - 50
}


/**
 * @brief Draw the instructions screen with improved layout and readability
 * Features:
 * - Better spaced text lines
 * - Clearer instruction sections
 * - RETURN button for navigation
 * - More readable layout
 */

void desenhar_instrucoes() {
  sprite_desenhar(instructions, 0, 0);
}


/**
 * @brief Frees the memory of the card sprites for the given fruit.
 * 
 * This function should be called when a specific fruit's card images
 * are no longer needed to free the associated memory.
 * 
 * @param fruta The type of fruit whose card sprites should be freed.
 */
void libertar_cartas_fruta(Fruta fruta) {
  switch (fruta) {
    case ABACATE:
      sprite_destruir(cartaAbacate1);
      sprite_destruir(cartaAbacate2);
      sprite_destruir(cartaAbacate3);
      sprite_destruir(cartaAbacate4);
      sprite_destruir(cartaAbacate5);
      sprite_destruir(cartaAbacate6);
      sprite_destruir(cartaAbacate7);
      sprite_destruir(cartaAbacate8);
      break;

    case BANANA:
      sprite_destruir(cartaBanana1);
      sprite_destruir(cartaBanana2);
      sprite_destruir(cartaBanana3);
      sprite_destruir(cartaBanana4);
      sprite_destruir(cartaBanana5);
      sprite_destruir(cartaBanana6);
      sprite_destruir(cartaBanana7);
      sprite_destruir(cartaBanana8);
      break;

    case CEREIJA:
      sprite_destruir(cartaCereija1);
      sprite_destruir(cartaCereija2);
      sprite_destruir(cartaCereija3);
      sprite_destruir(cartaCereija4);
      sprite_destruir(cartaCereija5);
      sprite_destruir(cartaCereija6);
      sprite_destruir(cartaCereija7);
      sprite_destruir(cartaCereija8);
      break;

    case COCO:
      sprite_destruir(cartaCoco1);
      sprite_destruir(cartaCoco2);
      sprite_destruir(cartaCoco3);
      sprite_destruir(cartaCoco4);
      sprite_destruir(cartaCoco5);
      sprite_destruir(cartaCoco6);
      sprite_destruir(cartaCoco7);
      sprite_destruir(cartaCoco8);
      break;

    case LARANJA:
      sprite_destruir(cartaLaranja1);
      sprite_destruir(cartaLaranja2);
      sprite_destruir(cartaLaranja3);
      sprite_destruir(cartaLaranja4);
      sprite_destruir(cartaLaranja5);
      sprite_destruir(cartaLaranja6);
      sprite_destruir(cartaLaranja7);
      sprite_destruir(cartaLaranja8);
      break;

    case MACA:
      sprite_destruir(cartaMaca1);
      sprite_destruir(cartaMaca2);
      sprite_destruir(cartaMaca3);
      sprite_destruir(cartaMaca4);
      sprite_destruir(cartaMaca5);
      sprite_destruir(cartaMaca6);
      sprite_destruir(cartaMaca7);
      sprite_destruir(cartaMaca8);
      break;

    case MELANCIA:
      sprite_destruir(cartaMelancia1);
      sprite_destruir(cartaMelancia2);
      sprite_destruir(cartaMelancia3);
      sprite_destruir(cartaMelancia4);
      sprite_destruir(cartaMelancia5);
      sprite_destruir(cartaMelancia6);
      sprite_destruir(cartaMelancia7);
      sprite_destruir(cartaMelancia8);
      break;

    case MORANGO:
      sprite_destruir(cartaMorango1);
      sprite_destruir(cartaMorango2);
      sprite_destruir(cartaMorango3);
      sprite_destruir(cartaMorango4);
      sprite_destruir(cartaMorango5);
      sprite_destruir(cartaMorango6);
      sprite_destruir(cartaMorango7);
      sprite_destruir(cartaMorango8);
      break;
  }
}
/**
 * @brief Waits approximately half a second using timer interrupts.
 *
 * Subscribes to timer interrupts and waits for a number of timer ticks 
 * corresponding to half a second (assuming 60Hz frequency). It handles 
 * the interrupts and increments a global counter until the time elapses.
 *
 * @return 0 on success, 1 if there is an error during subscription or unsubscription.
 */

int espera_meio_segundo() {
  int ipc_status;
  message msg;
  uint8_t bit_no;
  hook_id = 0;

  printf("Subscrevendo interrupção...\n");
  if (my_timer_subscribe_int(&bit_no) != 0) {
    printf("Erro ao subscrever interrupção do timer\n");
    return 1;
  }
  printf("bit_no: 0x%X\n", bit_no);

  counter = 0;

  printf("A esperar 0.5 segundos...\n");
  printf("IRQ bit mask: 0x%X\n", bit_no);
  while (counter < 5) {
    int r = driver_receive(ANY, &msg, &ipc_status);
    if (r != 0) {
      printf("Erro em driver_receive: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
        if (msg.m_notify.interrupts & bit_no) {
          printf("Interrupção do timer recebida. Counter = %d\n", counter);
          timer_int_handler();
        }
      }
    }
  }

  printf("Tempo decorrido!\n");

  if (my_timer_unsubscribe_int() != 0) {
    printf("Erro ao anular subscrição do timer\n");
    return 1;
  }

  return 0;
}
/**
 * @brief Draws a timer display in MM:SS format at a specified position.
 *
 * Converts a time value in seconds to minutes and seconds, then draws
 * the corresponding digit sprites and a colon between minutes and seconds.
 *
 * @param seconds Total time in seconds to display.
 * @param x X-coordinate where the timer display should start.
 * @param y Y-coordinate where the timer display should start.
 */
void desenhar_timer(uint32_t seconds, int x, int y) {
  uint8_t min = seconds / 60;
  uint8_t sec = seconds % 60;

  Sprite* digits[10] = {
      digit_0, digit_1, digit_2, digit_3, digit_4,
      digit_5, digit_6, digit_7, digit_8, digit_9
  };

  Sprite* to_draw[] = {
      digits[min / 10], digits[min % 10],
      digit_colon,
      digits[sec / 10], digits[sec % 10]
  };

  draw_line(to_draw, 5, x, y, 2); // espaçamento de 2px entre caracteres

  
}

void desenhar_parabens(int tempo_segundos) {
  Sprite *msg[] = {
    letter_C, letter_O, letter_N, letter_G, letter_R, letter_A, letter_T, letter_U, letter_L, letter_A, letter_T,letter_I, letter_O, letter_N
  };
  Sprite *tempo_txt[]= {
    s_T, s_I, s_M, s_E
  };
  Sprite *tmp[]={
    s_T, s_A, s_K, s_E, s_N
  };

  my_vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000FF);

  int x = 250, y = 250;
  for (size_t i = 0; i < sizeof(msg)/sizeof(Sprite*); i++) {
    sprite_desenhar(msg[i], x, y);
    x += msg[i]->width + 5;
  }

  x = 290; y = 310;
  for (size_t i = 0; i < sizeof(tempo_txt)/sizeof(Sprite*); i++) {
    sprite_desenhar(tempo_txt[i], x, y);
    x += tempo_txt[i]->width + 5;
  }
  x=x+10;
  for (size_t i = 0; i < sizeof(tmp)/sizeof(Sprite*); i++) {
    sprite_desenhar(tmp[i], x, y);
    x += tmp[i]->width + 5;
  }


  desenhar_timer(tempo_segundos, x + 10, y);  // tempo ao lado
  swap_buffers();


}
