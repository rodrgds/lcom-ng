#include "font.h"
#include "xpm/fonts_xpm.h"

void (font_init)() {
  font = (font_t *) malloc(sizeof(font_t));
  if (font == NULL) {
    return;
  }

  font->letter_A = create_sprite((xpm_map_t) letter_A_xpm);
  font->letter_B = create_sprite((xpm_map_t) letter_B_xpm);
  font->letter_C = create_sprite((xpm_map_t) letter_C_xpm);
  font->letter_D = create_sprite((xpm_map_t) letter_D_xpm);
  font->letter_E = create_sprite((xpm_map_t) letter_E_xpm);
  font->letter_F = create_sprite((xpm_map_t) letter_F_xpm);
  font->letter_G = create_sprite((xpm_map_t) letter_G_xpm);
  font->letter_H = create_sprite((xpm_map_t) letter_H_xpm);
  font->letter_I = create_sprite((xpm_map_t) letter_I_xpm);
  font->letter_J = create_sprite((xpm_map_t) letter_J_xpm);
  font->letter_K = create_sprite((xpm_map_t) letter_K_xpm);
  font->letter_L = create_sprite((xpm_map_t) letter_L_xpm);
  font->letter_M = create_sprite((xpm_map_t) letter_M_xpm);
  font->letter_N = create_sprite((xpm_map_t) letter_N_xpm);
  font->letter_O = create_sprite((xpm_map_t) letter_O_xpm);
  font->letter_P = create_sprite((xpm_map_t) letter_P_xpm);
  font->letter_Q = create_sprite((xpm_map_t) letter_Q_xpm);
  font->letter_R = create_sprite((xpm_map_t) letter_R_xpm);
  font->letter_S = create_sprite((xpm_map_t) letter_S_xpm);
  font->letter_T = create_sprite((xpm_map_t) letter_T_xpm);
  font->letter_U = create_sprite((xpm_map_t) letter_U_xpm);
  font->letter_V = create_sprite((xpm_map_t) letter_V_xpm);
  font->letter_W = create_sprite((xpm_map_t) letter_W_xpm);
  font->letter_X = create_sprite((xpm_map_t) letter_X_xpm);
  font->letter_Y = create_sprite((xpm_map_t) letter_Y_xpm);
  font->letter_Z = create_sprite((xpm_map_t) letter_Z_xpm);
  font->zero = create_sprite((xpm_map_t) zero_xpm);
  font->one = create_sprite((xpm_map_t) one_xpm);
  font->two = create_sprite((xpm_map_t) two_xpm);
  font->three = create_sprite((xpm_map_t) three_xpm);
  font->four = create_sprite((xpm_map_t) four_xpm);
  font->five = create_sprite((xpm_map_t) five_xpm);
  font->six = create_sprite((xpm_map_t) six_xpm);
  font->seven = create_sprite((xpm_map_t) seven_xpm);
  font->eight = create_sprite((xpm_map_t) eight_xpm);
  font->nine = create_sprite((xpm_map_t) nine_xpm);
  font->space = create_sprite((xpm_map_t) space_xpm);
}

int (draw_text)(char* text, int x, int y, uint8_t color) {
  if (text == NULL || font == NULL) {
    return 1;
  }

  // TODO: remove magic value
  uint8_t color_override[2] = {2, color};

  int i = 0;
  while (text[i] != '\0') {
    switch (text[i]) {
      case 'a':
      case 'A':
        draw_sprite_color_override(font->letter_A, x + i * 20, y, color_override);
        break;
      case 'b':
      case 'B':
        draw_sprite_color_override(font->letter_B, x + i * 20, y, color_override);
        break;
      case 'c':
      case 'C':
        draw_sprite_color_override(font->letter_C, x + i * 20, y, color_override);
        break;
      case 'd':
      case 'D':
        draw_sprite_color_override(font->letter_D, x + i * 20, y, color_override);
        break;
      case 'e':
      case 'E':
        draw_sprite_color_override(font->letter_E, x + i * 20, y, color_override);
        break;
      case 'f':
      case 'F':
        draw_sprite_color_override(font->letter_F, x + i * 20, y, color_override);
        break;
      case 'g':
      case 'G':
        draw_sprite_color_override(font->letter_G, x + i * 20, y, color_override);
        break;
      case 'h':
      case 'H':
        draw_sprite_color_override(font->letter_H, x + i * 20, y, color_override);
        break;
      case 'i':
      case 'I':
        draw_sprite_color_override(font->letter_I, x + i * 20, y, color_override);
        break;
      case 'j':
      case 'J':
        draw_sprite_color_override(font->letter_J, x + i * 20, y, color_override);
        break;
      case 'k':
      case 'K':
        draw_sprite_color_override(font->letter_K, x + i * 20, y, color_override);
        break;
      case 'l':
      case 'L':
        draw_sprite_color_override(font->letter_L, x + i * 20, y, color_override);
        break;
      case 'm':
      case 'M':
        draw_sprite_color_override(font->letter_M, x + i * 20, y, color_override);
        break;
      case 'n':
      case 'N':
        draw_sprite_color_override(font->letter_N, x + i * 20, y, color_override);
        break;
      case 'o':
      case 'O':
        draw_sprite_color_override(font->letter_O, x + i * 20, y, color_override);
        break;
      case 'p':
      case 'P':
        draw_sprite_color_override(font->letter_P, x + i * 20, y, color_override);
        break;
      case 'q':
      case 'Q':
        draw_sprite_color_override(font->letter_Q, x + i * 20, y, color_override);
        break;
      case 'r':
      case 'R':
        draw_sprite_color_override(font->letter_R, x + i * 20, y, color_override);
        break;
      case 's':
      case 'S':
        draw_sprite_color_override(font->letter_S, x + i * 20, y, color_override);
        break;
      case 't':
      case 'T':
        draw_sprite_color_override(font->letter_T, x + i * 20, y, color_override);
        break;
      case 'u':
      case 'U':
        draw_sprite_color_override(font->letter_U, x + i * 20, y, color_override);
        break;
      case 'v':
      case 'V':
        draw_sprite_color_override(font->letter_V, x + i * 20, y, color_override);
        break;
      case 'w':
      case 'W':
        draw_sprite_color_override(font->letter_W, x + i * 20, y, color_override);
        break;
      case 'x':
      case 'X':
        draw_sprite_color_override(font->letter_X, x + i * 20, y, color_override);
        break;
      case 'y':
      case 'Y':
        draw_sprite_color_override(font->letter_Y, x + i * 20, y, color_override);
        break;
      case 'z':
      case 'Z':
        draw_sprite_color_override(font->letter_Z, x + i * 20, y, color_override);
        break;
      case '0':
        draw_sprite_color_override(font->zero, x + i * 20, y, color_override);
        break;
      case '1':
        draw_sprite_color_override(font->one, x + i * 20, y, color_override);
        break;
      case '2':
        draw_sprite_color_override(font->two, x + i * 20, y, color_override);
        break;
      case '3':
        draw_sprite_color_override(font->three, x + i * 20, y, color_override);
        break;
      case '4':
        draw_sprite_color_override(font->four, x + i * 20, y, color_override);
        break;
      case '5':
        draw_sprite_color_override(font->five, x + i * 20, y, color_override);
        break;
      case '6':
        draw_sprite_color_override(font->six, x + i * 20, y, color_override);
        break;
      case '7':
        draw_sprite_color_override(font->seven, x + i * 20, y, color_override);
        break;
      case '8':
        draw_sprite_color_override(font->eight, x + i * 20, y, color_override);
        break;
      case '9':
        draw_sprite_color_override(font->nine, x + i * 20, y, color_override);
        break;
      case ' ':
        draw_sprite_color_override(font->space, x + i * 20, y, color_override);
        break;
      default:
        printf("Invalid character in text!: \"%c\"\n", text[i]);
        return 1;
    }

    i++;
  }

  return 0;
}

void (font_destroy)() {
  if (font == NULL) {
    return;
  }

  destroy_sprite(font->letter_A);
  destroy_sprite(font->letter_B);
  destroy_sprite(font->letter_C);
  destroy_sprite(font->letter_D);
  destroy_sprite(font->letter_E);
  destroy_sprite(font->letter_F);
  destroy_sprite(font->letter_G);
  destroy_sprite(font->letter_H);
  destroy_sprite(font->letter_I);
  destroy_sprite(font->letter_J);
  destroy_sprite(font->letter_K);
  destroy_sprite(font->letter_L);
  destroy_sprite(font->letter_M);
  destroy_sprite(font->letter_N);
  destroy_sprite(font->letter_O);
  destroy_sprite(font->letter_P);
  destroy_sprite(font->letter_Q);
  destroy_sprite(font->letter_R);
  destroy_sprite(font->letter_S);
  destroy_sprite(font->letter_T);
  destroy_sprite(font->letter_U);
  destroy_sprite(font->letter_V);
  destroy_sprite(font->letter_W);
  destroy_sprite(font->letter_X);
  destroy_sprite(font->letter_Y);
  destroy_sprite(font->letter_Z);
  destroy_sprite(font->zero);
  destroy_sprite(font->one);
  destroy_sprite(font->two);
  destroy_sprite(font->three);
  destroy_sprite(font->four);
  destroy_sprite(font->five);
  destroy_sprite(font->six);
  destroy_sprite(font->seven);
  destroy_sprite(font->eight);
  destroy_sprite(font->nine);
  destroy_sprite(font->space);
}
