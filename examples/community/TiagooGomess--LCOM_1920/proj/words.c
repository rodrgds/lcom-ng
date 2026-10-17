
#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "scancodes.h"
#include "words.h"
#include "Numeros.h"
#include "sprite.h"


const char* filename = "/home/lcom/labs/proj/words.txt";
char* current_word = "";
char* current_word_complete = "";
int numero_palavras_digitadas = 0;
extern int y0_word;
extern int x0_word;


extern Sprite *sprite_A;
extern Sprite *sprite_B;
extern Sprite *sprite_C;
extern Sprite *sprite_D;
extern Sprite *sprite_E;
extern Sprite *sprite_F;
extern Sprite *sprite_G;
extern Sprite *sprite_H;
extern Sprite *sprite_I;
extern Sprite *sprite_J;
extern Sprite *sprite_K;
extern Sprite *sprite_L;
extern Sprite *sprite_M;
extern Sprite *sprite_N;
extern Sprite *sprite_O;
extern Sprite *sprite_P;
extern Sprite *sprite_Q;
extern Sprite *sprite_R;
extern Sprite *sprite_S;
extern Sprite *sprite_T;
extern Sprite *sprite_U;
extern Sprite *sprite_V;
extern Sprite *sprite_W;
extern Sprite *sprite_X;
extern Sprite *sprite_Y;
extern Sprite *sprite_Z;

extern Sprite *sprite_0;
extern Sprite *sprite_1;
extern Sprite *sprite_2;
extern Sprite *sprite_3;
extern Sprite *sprite_4;
extern Sprite *sprite_5;
extern Sprite *sprite_6;
extern Sprite *sprite_7;
extern Sprite *sprite_8;
extern Sprite *sprite_9;


char* get_word() {
   FILE* words_file;
   words_file = fopen(filename, "r");
   if (words_file == NULL) {
       printf("Cannot open file \n");
       exit(0);
   }
   srand(time(0));
   int random_line = 1 + rand() % 2995;
   char* input = (char *) calloc(256, sizeof(char));
   int line = 0;
   while(fgets(input, 256 * sizeof(char), words_file)) {
        line++;
        if (line == random_line) {
            fclose(words_file);
            current_word = input;
            current_word_complete = input;
            return input;
        }
   }
   free(input);
   fclose(words_file);
   return input;
}

void update_word(char c) {

    if (strlen(current_word) == 0)
        return;

    if (current_word[0] == c) {
        memmove(current_word, current_word+1, strlen(current_word)); // remove o primeiro caracter de current_word

        if (strlen(current_word) == 1) {
            y0_word = 0;
            numero_palavras_digitadas++;
            current_word = "";
            get_word();
            srand(time(0));
            x0_word = rand() % 300;

            return;
        }
    }
}

xpm_row_t* getXPM(char c) {
    xpm_row_t* letra_a = letra_A;
    xpm_row_t* letra_b = letra_B;
    xpm_row_t* letra_c = letra_C;
    xpm_row_t* letra_d = letra_D;
    xpm_row_t* letra_e = letra_E;
    xpm_row_t* letra_f = letra_F;
    xpm_row_t* letra_g = letra_G;
    xpm_row_t* letra_h = letra_H;
    xpm_row_t* letra_i = letra_I;
    xpm_row_t* letra_j = letra_J;
    xpm_row_t* letra_k = letra_K;
    xpm_row_t* letra_l = letra_L;
    xpm_row_t* letra_m = letra_M;
    xpm_row_t* letra_n = letra_N;
    xpm_row_t* letra_o = letra_O;
    xpm_row_t* letra_p = letra_P;
    xpm_row_t* letra_q = letra_Q;
    xpm_row_t* letra_r = letra_R;
    xpm_row_t* letra_s = letra_S;
    xpm_row_t* letra_t = letra_T;
    xpm_row_t* letra_u = letra_U;
    xpm_row_t* letra_v = letra_V;
    xpm_row_t* letra_w = letra_W;
    xpm_row_t* letra_x = letra_X;
    xpm_row_t* letra_y = letra_Y;
    xpm_row_t* letra_z = letra_Z;

    xpm_row_t* numero_0 = num_0;
    xpm_row_t* numero_1 = num_1;
    xpm_row_t* numero_2 = num_2;
    xpm_row_t* numero_3 = num_3;
    xpm_row_t* numero_4 = num_4;
    xpm_row_t* numero_5 = num_5;
    xpm_row_t* numero_6 = num_6;
    xpm_row_t* numero_7 = num_7;
    xpm_row_t* numero_8 = num_8;
    xpm_row_t* numero_9 = num_9;


    if (c == 'A' || c == 'a')
        return letra_a;
    if (c == 'B' || c == 'b') {
        return letra_b;
    }
    if (c == 'C' || c == 'c') {
        return letra_c;
    }
    if (c == 'D' || c == 'd') {
        return letra_d;
    }
    if (c == 'E' || c == 'e') {
        return letra_e;
    }
    if (c == 'F' || c == 'f') {
        return letra_f;
    }
    if (c == 'G' || c == 'g') {
        return letra_g;
    }
    if (c == 'H' || c == 'h') {
        return letra_h;
    }
    if (c == 'I' || c == 'i') {
        return letra_i;
    }
    if (c == 'J' || c == 'j') {
        return letra_j;
    }
    if (c == 'K' || c == 'k') {
        return letra_k;
    }
    if (c == 'L' || c == 'l') {
        return letra_l;
    }
    if (c == 'M' || c == 'm') {
        return letra_m;
    }
    if (c == 'N' || c == 'n') {
        return letra_n;
    }
    if (c == 'O' || c == 'o') {
        return letra_o;
    }
    if (c == 'P' || c == 'p') {
        return letra_p;
    }
    if (c == 'Q' || c == 'q') {
        return letra_q;
    }
    if (c == 'R' || c == 'r') {
        return letra_r;
    }
    if (c == 'S' || c == 's') {
        return letra_s;
    }
    if (c == 'T' || c == 't') {
        return letra_t;
    }
    if (c == 'U' || c == 'u') {
        return letra_u;
    }
    if (c == 'V' || c == 'v') {
        return letra_v;
    }
    if (c == 'X' || c == 'x') {
        return letra_x;
    }
    if (c == 'Y' || c == 'y') {
        return letra_y;
    }
    if (c == 'W' || c == 'w') {
        return letra_w;
    }
    if (c == 'Z' || c == 'z') {
        return letra_z;
    }
    if (c == '0') {
        return numero_0;
    }
    if (c == '1') {
        return numero_1;
    }
    if (c == '2') {
        return numero_2;
    }
    if (c == '3') {
        return numero_3;
    }
    if (c == '4') {
        return numero_4;
    }
    if (c == '5') {
        return numero_5;
    }
    if (c == '6') {
        return numero_6;
    }
    if (c == '7') {
        return numero_7;
    }
    if (c == '8') {
        return numero_8;
    }
    if (c == '9') {
        return numero_9;
    }
    return NULL;
}

unsigned get_letter_width(char c) {
    if (c == 'A' | c == 'a')
        return 65;
    else if (c == 'B' || c == 'b') {
        return 55;
    }
    else if (c == 'C' || c == 'c') {
        return 60;
    }
    else if (c == 'D' || c == 'd') {
        return 62;
    }
    else if (c == 'E' || c == 'e') {
        return 45;
    }
    else if (c == 'F' || c == 'f') {
        return 45;
    }
    else if (c == 'G' || c == 'g') {
        return 60;
    }
    else if (c == 'H' || c == 'h') {
        return 62;
    }
    else if (c == 'I' || c == 'i') {
        return 32;
    }
    else if (c == 'J' || c == 'j') {
        return 40;
    }
    else if (c == 'K' || c == 'k') {
        return 60;
    }
    else if (c == 'L' || c == 'l') {
        return 45;
    }
    else if (c == 'M' || c == 'm') {
        return 80;
    }
    else if (c == 'N' || c == 'n') {
        return 64;
    }
    else if (c == 'O' || c == 'o') {
        return 68;
    }
    else if (c == 'P' || c == 'p') {
        return 52;
    }
    else if (c == 'Q' || c == 'q') {
        return 68;
    }
    else if (c == 'R' || c == 'r') {
        return 62;
    }
    else if (c == 'S' || c == 's') {
        return 52;
    }
    else if (c == 'T' || c == 't') {
        return 54;
    }
    else if (c == 'U' || c == 'u') {
        return 58;
    }
    else if (c == 'V' || c == 'v') {
        return 66;
    }
    else if (c == 'X' || c == 'x') {
        return 64;
    }
    else if (c == 'Y' || c == 'y') {
        return 62;
    }
    else if (c == 'W' || c == 'w') {
        return 96;
    }
    else if (c == 'Z' || c == 'z') {
        return 60;
    }
    return 0;
}

void displayWord(int x0, int y0, char* word, uint8_t* initial_address) {
    int x_atual = x0;
    for (size_t i = 0; i < strlen(word)-1; i++) { // o programa encrava quando meto i < strlen(word)
        if (i != 0)
            x_atual += get_letter_width(word[i])/2;

        Sprite* letra_atual = getSprite(word[i]);
        letra_atual->x = x_atual;
        letra_atual->y = y0;
        draw_sprite(letra_atual, initial_address);

        x_atual += get_letter_width(word[i])/2;
    }
}

void displayWordWithColor(int x0, int y0, uint32_t color, char* word, uint8_t* initial_address) {
    int x_atual = x0;
    for (size_t i = 0; i < strlen(word); i++) {
        if (i != 0)
            x_atual += get_letter_width(word[i])/2;
        if (word[i] == ' ')
            break;
        Sprite* letra_atual = getSprite(word[i]);
        letra_atual->x = x_atual;
        letra_atual->y = y0;
        draw_sprite_with_color(letra_atual, color, initial_address);

        x_atual += get_letter_width(word[i])/2;
    }
}



// Copiado de https://stackoverflow.com/questions/31778365/convert-int-to-char-c
char * inttostr(int n) {
    char * result;
    if (n == 0)
        result = malloc(2);
    else if (n > 0)
        result = malloc(floor(log10(n)) + 2);
    else
        result = malloc(floor(log10(n)) + 3);
    sprintf(result, "%d", n);
    return result;
}

void displayNumber(int x0, int y0, unsigned num, uint8_t* initial_address) {
    int x_atual = x0;
    char* num_string = inttostr(num);
    for (size_t i = 0; i < strlen(num_string); i++) {

        Sprite* numero_atual = getSprite(num_string[i]);
        numero_atual->x = x_atual;
        numero_atual->y = y0;
        draw_sprite(numero_atual, initial_address);

        x_atual += 45;
    }
    free(num_string);
}

Sprite* getSprite(char c) {
    if (c == 'A' || c == 'a') {
        return sprite_A;
    }
    else if (c == 'B' || c == 'b') {
        return sprite_B;
    }
    else if (c == 'C' || c == 'c') {
        return sprite_C;
    }
    else if (c == 'D' || c == 'd') {
        return sprite_D;
    }
    else if (c == 'E' || c == 'e') {
        return sprite_E;
    }
    else if (c == 'F' || c == 'f') {
        return sprite_F;
    }
    else if (c == 'G' || c == 'g') {
        return sprite_G;
    }
    else if (c == 'H' || c == 'h') {
        return sprite_H;
    }
    else if (c == 'I' || c == 'i') {
        return sprite_I;
    }
    else if (c == 'J' || c == 'j') {
        return sprite_J;
    }
    else if (c == 'K' || c == 'k') {
        return sprite_K;
    }
    else if (c == 'L' || c == 'l') {
        return sprite_L;
    }
    else if (c == 'M' || c == 'm') {
        return sprite_M;
    }
    else if (c == 'N' || c == 'n') {
        return sprite_N;
    }
    else if (c == 'O' || c == 'o') {
        return sprite_O;
    }
    else if (c == 'P' || c == 'p') {
        return sprite_P;
    }
    else if (c == 'Q' || c == 'q') {
        return sprite_Q;
    }
    else if (c == 'R' || c == 'r') {
        return sprite_R;
    }
    else if (c == 'S' || c == 's') {
        return sprite_S;
    }
    else if (c == 'T' || c == 't') {
        return sprite_T;
    }
    else if (c == 'U' || c == 'u') {
        return sprite_U;
    }
    else if (c == 'V' || c == 'v') {
        return sprite_V;
    }
    else if (c == 'X' || c == 'x') {
        return sprite_X;
    }
    else if (c == 'Y' || c == 'y') {
        return sprite_Y;
    }
    else if (c == 'W' || c == 'w') {
        return sprite_W;
    }
    else if (c == 'Z' || c == 'z') {
        return sprite_Z;
    }
    else if (c == '0') {
        return sprite_0;
    }
    else if (c == '1') {
        return sprite_1;
    }
    else if (c == '2') {
        return sprite_2;
    }
    else if (c == '3') {
        return sprite_3;
    }
    else if (c == '4') {
        return sprite_4;
    }
    else if (c == '5') {
        return sprite_5;
    }
    else if (c == '6') {
        return sprite_6;
    }
    else if (c == '7') {
        return sprite_7;
    }
    else if (c == '8') {
        return sprite_8;
    }
    else if (c == '9') {
        return sprite_9;
    }
    return NULL;
}
