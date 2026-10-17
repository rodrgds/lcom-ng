#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "video_gr.h"
#include "RTC.h"
#include "../xpm/abecedario/abecedario.h"
#include "../xpm/numeros/numeros.h"

extern int global_counter;

/**
 * @brief alphabet xpms
 * 
 */
xpm_image_t letter_A;
xpm_image_t letter_B;
xpm_image_t letter_C;
xpm_image_t letter_D;
xpm_image_t letter_E;
xpm_image_t letter_F;
xpm_image_t letter_G;
xpm_image_t letter_H;
xpm_image_t letter_I;
xpm_image_t letter_J;
xpm_image_t letter_K;
xpm_image_t letter_L;
xpm_image_t letter_M;
xpm_image_t letter_N;
xpm_image_t letter_O;
xpm_image_t letter_P;
xpm_image_t letter_Q;
xpm_image_t letter_R;
xpm_image_t letter_S;
xpm_image_t letter_T;
xpm_image_t letter_U;
xpm_image_t letter_V;
xpm_image_t letter_W;
xpm_image_t letter_X;
xpm_image_t letter_Y;
xpm_image_t letter_Z;
xpm_image_t number_0;
xpm_image_t number_1;
xpm_image_t number_2;
xpm_image_t number_3;
xpm_image_t number_4;
xpm_image_t number_5;
xpm_image_t number_6;
xpm_image_t number_7;
xpm_image_t number_8;
xpm_image_t number_9;
xpm_image_t barra_img;
xpm_image_t doisPontos;

void createXPMs(){
    //Load xpms
    xpm_load(A, XPM_8_8_8, &letter_A);
    xpm_load(B, XPM_8_8_8, &letter_B);
    xpm_load(C, XPM_8_8_8, &letter_C);
    xpm_load(D, XPM_8_8_8, &letter_D);
    xpm_load(E, XPM_8_8_8, &letter_E);
    xpm_load(F, XPM_8_8_8, &letter_F);
    xpm_load(G, XPM_8_8_8, &letter_G);
    xpm_load(H, XPM_8_8_8, &letter_H);
    xpm_load(I, XPM_8_8_8, &letter_I);
    xpm_load(J, XPM_8_8_8, &letter_J);
    xpm_load(K, XPM_8_8_8, &letter_K);
    xpm_load(L, XPM_8_8_8, &letter_L);
    xpm_load(M, XPM_8_8_8, &letter_M);
    xpm_load(N, XPM_8_8_8, &letter_N);
    xpm_load(O, XPM_8_8_8, &letter_O);
    xpm_load(P, XPM_8_8_8, &letter_P);
    xpm_load(Q, XPM_8_8_8, &letter_Q);
    xpm_load(R, XPM_8_8_8, &letter_R);
    xpm_load(S, XPM_8_8_8, &letter_S);
    xpm_load(T, XPM_8_8_8, &letter_T);
    xpm_load(U, XPM_8_8_8, &letter_U);
    xpm_load(V, XPM_8_8_8, &letter_V);
    xpm_load(W, XPM_8_8_8, &letter_W);
    xpm_load(X, XPM_8_8_8, &letter_X);
    xpm_load(Y, XPM_8_8_8, &letter_Y);
    xpm_load(Z, XPM_8_8_8, &letter_Z);

    xpm_load(nr_0, XPM_8_8_8, &number_0);
    xpm_load(nr_1, XPM_8_8_8, &number_1);
    xpm_load(nr_2, XPM_8_8_8, &number_2);
    xpm_load(nr_3, XPM_8_8_8, &number_3);
    xpm_load(nr_4, XPM_8_8_8, &number_4);
    xpm_load(nr_5, XPM_8_8_8, &number_5);
    xpm_load(nr_6, XPM_8_8_8, &number_6);
    xpm_load(nr_7, XPM_8_8_8, &number_7);
    xpm_load(nr_8, XPM_8_8_8, &number_8);
    xpm_load(nr_9, XPM_8_8_8, &number_9);

    xpm_load(barra, XPM_8_8_8, &barra_img);
    xpm_load(dois_pontos, XPM_8_8_8, &doisPontos);
}

int printCharacter(char character, int x_pos, int y_pos){

    //Convert all the letters to lowercase
    if (character >= 'A' && character <= 'Z')
         character += 32;

    switch(character){
        case 'a':
            vg_draw_img(letter_A, x_pos, y_pos);
            return letter_A.width;
        case 'b':
            vg_draw_img(letter_B, x_pos, y_pos);
            return letter_B.width;
        case 'c':
            vg_draw_img(letter_C, x_pos, y_pos);
            return letter_C.width;
        case 'd':
            vg_draw_img(letter_D, x_pos, y_pos);
            return letter_D.width;
        case 'e':
            vg_draw_img(letter_E, x_pos, y_pos);
            return letter_E.width;
        case 'f':
            vg_draw_img(letter_F, x_pos, y_pos);
            return letter_F.width;
        case 'g':
            vg_draw_img(letter_G, x_pos, y_pos);
            return letter_G.width;
        case 'h':
            vg_draw_img(letter_H, x_pos, y_pos);
            return letter_H.width;
        case 'i':
            vg_draw_img(letter_I, x_pos, y_pos);
            return letter_I.width;
        case 'j':
            vg_draw_img(letter_J, x_pos, y_pos);
            return letter_J.width;
        case 'k':
            vg_draw_img(letter_K, x_pos, y_pos);
            return letter_K.width;
        case 'l':
            vg_draw_img(letter_L, x_pos, y_pos);
            return letter_L.width;
        case 'm':
            vg_draw_img(letter_M, x_pos, y_pos);
            return letter_M.width;
        case 'n':
            vg_draw_img(letter_N, x_pos, y_pos);
            return letter_N.width;
        case 'o':
            vg_draw_img(letter_O, x_pos, y_pos);
            return letter_O.width;
        case 'p':
            vg_draw_img(letter_P, x_pos, y_pos);
            return letter_P.width;
        case 'q':
            vg_draw_img(letter_Q, x_pos, y_pos);
            return letter_Q.width;
        case 'r':
            vg_draw_img(letter_R, x_pos, y_pos);
            return letter_R.width;
        case 's':
            vg_draw_img(letter_S, x_pos, y_pos);
            return letter_S.width;
        case 't':
            vg_draw_img(letter_T, x_pos, y_pos);
            return letter_T.width;
        case 'u':
            vg_draw_img(letter_U, x_pos, y_pos);
            return letter_U.width;
        case 'v':
            vg_draw_img(letter_V, x_pos, y_pos);
            return letter_V.width;
        case 'w':
            vg_draw_img(letter_W, x_pos, y_pos);
            return letter_W.width;
        case 'x':
            vg_draw_img(letter_X, x_pos, y_pos);
            return letter_X.width;
        case 'y':
            vg_draw_img(letter_Y, x_pos, y_pos);
            return letter_Y.width;
        case 'z':
            vg_draw_img(letter_Z, x_pos, y_pos);
            return letter_Z.width;

        case ' ':
            return 20;

        case '0':
            vg_draw_img(number_0, x_pos,y_pos);
            return number_0.width;
        case '1':
            vg_draw_img(number_1, x_pos, y_pos);
            return number_1.width;
        case '2':
            vg_draw_img(number_2, x_pos, y_pos);
            return number_2.width;
        case '3':
            vg_draw_img(number_3, x_pos, y_pos);
            return number_3.width;
        case '4':
            vg_draw_img(number_4, x_pos, y_pos);
            return number_4.width;
        case '5':
            vg_draw_img(number_5, x_pos, y_pos);
            return number_5.width;
        case '6':
            vg_draw_img(number_6, x_pos, y_pos);
            return number_6.width;
        case '7':
            vg_draw_img(number_7, x_pos, y_pos);
            return number_7.width;
        case '8':
            vg_draw_img(number_8, x_pos, y_pos);
            return number_8.width;
        case '9':
            vg_draw_img(number_9, x_pos, y_pos);
            return number_9.width;
        case '/':
            vg_draw_img(barra_img, x_pos, y_pos);
            return barra_img.width;
        case ':':
            vg_draw_img(doisPontos, x_pos, y_pos);
            return doisPontos.width;
    }

    return 0;
}

void printWord(char word[], int x_pos, int y_pos){

    int xPos = x_pos;
    for(size_t i=0; i<strlen(word); i++) {
        xPos += printCharacter(word[i], xPos, y_pos)-8;
    }
}

void printCounter(){
    char counter[16];
    int minutes = global_counter / 3600;
    int seconds = (global_counter % 3600) / 60;
    snprintf(counter, sizeof counter, "%02d:%02d", minutes, seconds);
    printWord(counter, 640, 180);
}

void printDate(){
    char date[11];
    char time[6];
    snprintf(date, sizeof date, "%02d/%02d/20%02d", getDayRTC(), getMonthRTC(), getYearRTC());
    snprintf(time, sizeof time, "%02d:%02d", getHoursRTC(), getMinutesRTC());
    printWord(date, 25, 5);
    printWord(time, 630, 5);
}
