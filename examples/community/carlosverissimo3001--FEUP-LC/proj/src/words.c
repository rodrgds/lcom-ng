#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>

#include "words.h"

/* Contains xpms of all letters of the alphabet */
xpm_image_t alphabet[27];

/* Word dictionary */
char *words[DICTIONARY_SIZE] = {"BEE", "BED", "BRANCH", "DINOSAUR", "JAIL", "HAT", "TAIL",
                  "KING", "FLY", "SPOON", "BOAT", "NIGHT", "FAMILY", "FIRE", "SWING",
                  "HEART", "BUTTON", "HOOK", "COMPUTER", "BOW", "RAINBOW", "ISLAND", "COAT",
                  "PILLOW", "SNOWFLAKE", "COOKIE", "DRAGON", "BALLOON", "FEET",
                  "BUNNY", "PLANT", "MONKEY", "SUN", "CUBE", "DRUM", "HAMBURGER", "NOSE",
                  "JAR", "CHIMNEY", "PIZZA"};


void(load_letter)(char letter) {
  switch (letter) {
    case 'A': xpm_load(A_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'B': xpm_load(B_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'C': xpm_load(C_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'D': xpm_load(D_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'E': xpm_load(E_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'F': xpm_load(F_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'G': xpm_load(G_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'H': xpm_load(H_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'I': xpm_load(I_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'J': xpm_load(J_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'K': xpm_load(K_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'L': xpm_load(L_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'M': xpm_load(M_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'N': xpm_load(N_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'O': xpm_load(O_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'P': xpm_load(P_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'Q': xpm_load(Q_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'R': xpm_load(R_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'S': xpm_load(S_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'T': xpm_load(T_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'U': xpm_load(U_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'V': xpm_load(V_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'W': xpm_load(W_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'X': xpm_load(X_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'Y': xpm_load(Y_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
    case 'Z': xpm_load(Z_xpm, XPM_8_8_8_8, &alphabet[letter - 65]); break;
  }
}

void(load_alphabet)() {
  for (int i = 0; i < 26; i++) {
    load_letter('A' + i);
  }
  xpm_load(Space_xpm, XPM_8_8_8_8, &alphabet[26]);
}

void(draw_word)(char *word, int x, int y) {
  uint16_t prev_letter_width = 0;
  for (int i = 0; i < (int) strlen(word); i++){
    draw_letter(word[i], x + prev_letter_width, y);
    prev_letter_width += get_width(word[i]);
  }
}

void(draw_word_with_color)(char *word, int x, int y, uint32_t color) {
  uint16_t prev_letter_width = 0;
  for (int i = 0; i < (int) strlen(word); i++){
    draw_letter_with_color(word[i], x + prev_letter_width, y, color);
    prev_letter_width += get_width(word[i]);
  }
}

void (draw_big_word)(char *word, int x, int y){
  int height_delta = 0;
  uint16_t prev_letter_width = 0;

  for (int i = 0; i < (int) strlen(word); i++){
    if (word[i] == ' '){
      draw_letter(word[i], x + prev_letter_width, y + height_delta);
      height_delta += 35;
      prev_letter_width = 0;
    }
    else{
      draw_letter(word[i], x + prev_letter_width, y + height_delta);
      prev_letter_width += get_width(word[i]);
    }
  }
}

void(draw_letter)(char c, int x, int y) {
  if (c != ' ' && (c < 'A' || c > 'Z')) return;
  if (c != ' ')
    draw_pixmap(x, y, &(alphabet[c - 65]), true);

  else
    draw_pixmap(x, y, &alphabet[26], true);

}

void (erase_letter)(char c , int x , int y){
  draw_letter_with_color(c,x,y,0x989898);
}

void(draw_letter_with_color)(char c, int x, int y, uint32_t color){
  if (c != ' ' && (c < 'A' || c > 'Z')) return;
  if (c != ' '){
    draw_pixmap_fixed_color(x, y, &(alphabet[c - 65]), color, true);
    //add_img_to_frame(&(alphabet[c-65]), x, y);
  }
  else{
    draw_pixmap_fixed_color(x, y, &alphabet[26], color, true);
    //add_img_to_frame(&(alphabet[26]), x, y);
  }
}


uint16_t(get_width)(char c) {
  if (c != ' ' && (c < 'A' || c > 'Z')) return 0;
  return (c == ' ') ? alphabet[26].width : alphabet[c - 65].width;
}

char* (get_random_word)(){
  time_t t;
  srand((unsigned) time(&t));

  int index = rand() % DICTIONARY_SIZE;

  return words[index];
}

void(add_character)(char *str, char c) {
  str[strlen(str)] = c;
}

char (remove_character)(char *str){
  char temp = str[strlen(str)-1];
  str[strlen(str)-1] = 0;
  return temp;
}


