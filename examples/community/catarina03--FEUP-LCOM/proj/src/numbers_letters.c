#include <lcom/lcf.h>

#include "numbers_letters.h"

Numeros createNums(){
  //Numeros *numeros=(Numeros*)malloc(sizeof(Numeros));
  Numeros numeros;
  xpm_load(zero_xpm,XPM_5_6_5,&numeros.n[0]);
  xpm_load(um_xpm,XPM_5_6_5,&numeros.n[1]);
  xpm_load(dois_xpm,XPM_5_6_5,&numeros.n[2]);
  xpm_load(tres_xpm,XPM_5_6_5,&numeros.n[3]);
  xpm_load(quatro_xpm,XPM_5_6_5,&numeros.n[4]);
  xpm_load(cinco_xpm,XPM_5_6_5,&numeros.n[5]);
  xpm_load(seis_xpm,XPM_5_6_5,&numeros.n[6]);
  xpm_load(sete_xpm,XPM_5_6_5,&numeros.n[7]);
  xpm_load(oito_xpm,XPM_5_6_5,&numeros.n[8]);
  xpm_load(nove_xpm,XPM_5_6_5,&numeros.n[9]);
  return numeros;
}

int countdown(int time_in_seconds,Numeros *n){

    //// COORDINATES ////
    // Minutes: first digit = leftmost digit: x =  X_FIRST_D_MIN, y = Y_NUMBERS
    //          second digit = rightmost digit: x = X_FIRST_D_MIN, y = Y_NUMBERS
    // Seconds: first digit = leftmostmost digit: x = X_FIRST_D_SEC, y = Y_NUMBERS
    //          second digit = rightmost digit: x =  X_SECOND_D_SEC, y = Y_NUMBERS

    int Y_NUMBERS = 5;
    int X_FIRST_D_MIN = 710;
    int X_SECOND_D_MIN = 725;
    int X_FIRST_D_SEC = 750;
    int X_SECOND_D_SEC = 765;

    int seconds = time_in_seconds % 60; 
    int minutes = time_in_seconds/60;

    int first_d_sec = seconds / 10;
    int second_d_sec = seconds % 10;  
    int first_d_min = minutes / 10;
    int second_d_min = minutes % 10; 

    switch (first_d_sec) //Primeiro digito dos segundos
    {
    case 0:
        draw_sprite(X_FIRST_D_SEC, Y_NUMBERS, &n->n[0]);
        break;
    case 1:
        draw_sprite(X_FIRST_D_SEC, Y_NUMBERS, &n->n[1]);
        break;
    case 2:
        draw_sprite(X_FIRST_D_SEC, Y_NUMBERS, &n->n[2]);
        break;
    case 3:
        draw_sprite(X_FIRST_D_SEC, Y_NUMBERS, &n->n[3]);
        break;
    case 4:
        draw_sprite(X_FIRST_D_SEC, Y_NUMBERS, &n->n[4]);
        break;
    case 5:
        draw_sprite(X_FIRST_D_SEC, Y_NUMBERS, &n->n[5]);
        break;
    default:
        break;
    }

    switch (second_d_sec) //Segundo digito dos segundos
    {
    case 0:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[0]);
        break;
    case 1:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[1]);
        break;
    case 2:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[2]);
        break;
    case 3:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[3]);
        break;
    case 4:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[4]);
        break;
    case 5:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[5]);
        break;
    case 6:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[6]);
        break;
    case 7:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[7]);
        break;
    case 8:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[8]);
        break;
    case 9:
        draw_sprite(X_SECOND_D_SEC, Y_NUMBERS, &n->n[9]);
        break;
    default:
        break;
    }

    switch (first_d_min) //Primeiro digito dos minutos
    {
    case 0:
        draw_sprite(X_FIRST_D_MIN, Y_NUMBERS, &n->n[0]);
        break;
    case 1:
        draw_sprite(X_FIRST_D_MIN, Y_NUMBERS, &n->n[1]);
        break;
    case 2:
        draw_sprite(X_FIRST_D_MIN, Y_NUMBERS, &n->n[2]);
        break;
    case 3:
        draw_sprite(X_FIRST_D_MIN, Y_NUMBERS, &n->n[3]);
        break;
    case 4:
        draw_sprite(X_FIRST_D_MIN, Y_NUMBERS, &n->n[4]);
        break;
    case 5:
        draw_sprite(X_FIRST_D_MIN, Y_NUMBERS, &n->n[5]);
        break;
    default:
        break;
    }

    switch (second_d_min) //Segundo digito dos minutos
    {
    case 0:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[0]);
        break;
    case 1:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[1]);
        break;
    case 2:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[2]);
        break;
    case 3:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[3]);
        break;
    case 4:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[4]);
        break;
    case 5:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[5]);
        break;
    case 6:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[6]);
        break;
    case 7:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[7]);
        break;
    case 8:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[8]);
        break;
    case 9:
        draw_sprite(X_SECOND_D_MIN, Y_NUMBERS, &n->n[9]);
        break;
    default:
        break;
    }

    return 0;
}


Letras *createLetras(){
  Letras *letras=(Letras*)malloc(sizeof(Letras));
  xpm_load(A_xpm, XPM_5_6_5, &letras->n[0]);
  xpm_load(B_xpm, XPM_5_6_5, &letras->n[1]);
  xpm_load(C_xpm, XPM_5_6_5, &letras->n[2]);
  xpm_load(D_xpm, XPM_5_6_5, &letras->n[3]);
  xpm_load(E_xpm, XPM_5_6_5, &letras->n[4]);
  xpm_load(F_xpm, XPM_5_6_5, &letras->n[5]);
  xpm_load(G_xpm, XPM_5_6_5, &letras->n[6]);
  xpm_load(H_xpm, XPM_5_6_5, &letras->n[7]);
  xpm_load(I_xpm, XPM_5_6_5, &letras->n[8]);
  xpm_load(J_xpm, XPM_5_6_5, &letras->n[9]);
  xpm_load(K_xpm, XPM_5_6_5, &letras->n[10]);
  xpm_load(L_xpm, XPM_5_6_5, &letras->n[11]);
  xpm_load(M_xpm, XPM_5_6_5, &letras->n[12]);
  xpm_load(N_xpm, XPM_5_6_5, &letras->n[13]);
  xpm_load(O_xpm, XPM_5_6_5, &letras->n[14]);
  xpm_load(P_xpm, XPM_5_6_5, &letras->n[15]);
  xpm_load(Q_xpm, XPM_5_6_5, &letras->n[16]);
  xpm_load(R_xpm, XPM_5_6_5, &letras->n[17]);
  xpm_load(S_xpm, XPM_5_6_5, &letras->n[18]);
  xpm_load(T_xpm, XPM_5_6_5, &letras->n[19]);
  xpm_load(U_xpm, XPM_5_6_5, &letras->n[20]);
  xpm_load(V_xpm, XPM_5_6_5, &letras->n[21]);
  xpm_load(W_xpm, XPM_5_6_5, &letras->n[22]);
  xpm_load(X_xpm, XPM_5_6_5, &letras->n[23]);
  xpm_load(Y_xpm, XPM_5_6_5, &letras->n[24]);
  xpm_load(Z_xpm, XPM_5_6_5, &letras->n[25]);
  xpm_load(barra_xpm, XPM_5_6_5, &letras->n[26]);
  xpm_load(dois_pontos_xpm, XPM_5_6_5, &letras->n[27]);
  return letras;
}


int printNum(int num,Numeros *n,int x,int y){
    char digits[20];
    sprintf(digits, "%d", num); 
    //itoa(num,digits,10);
    for (size_t i=0;i<strlen(digits);i++){
        switch (digits[i])
        {
        case '0':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[0]);
            break;
        case '1':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[1]);
            break;
        case '2':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[2]);
            break;
        case '3':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[3]);
            break;
        case '4':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[4]);
            break;   
        case '5':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[5]);
            break;
        case '6':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[6]);
            break;
        case '7':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[7]);
            break;
        case '8':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[8]);
            break;
        case '9':
            draw_sprite(x + i*LETTER_SPACING, y, &n->n[9]);
            break;
        default:
            break;
        }
    }
    return 0;
}


int printLetras(char frase[], Letras *l, int x_initial, int y){
    for (size_t i = 0; i < strlen(frase); i++){
        switch (frase[i])
        {
        case ('A'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[0]);
            break;
        case('a'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[0]);
            break;
        case ('B'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[1]);
            break;    
        case ('b'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[1]);
            break;    
        case ('C'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[2]);
            break;
        case ('c'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[2]);
            break;
        case ('D'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[3]);
            break;    
        case ('d'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[3]);
            break;   
        case ('E'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[4]);
            break;
        case ('e'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[4]);
            break;
        case ('F'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[5]);
            break;
        case ('f'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[5]);
            break;
        case ('G'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[6]);
            break;    
        case ('g'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[6]);
            break;    
        case ('H'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[7]);
            break;
        case ('h'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[7]);
            break;
        case ('I'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[8]);
            break;    
        case ('i'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[8]);
            break;   
        case ('J'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[9]);
            break;
        case ('j'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[9]);
            break;
        case ('K'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[10]);
            break;
        case ('k'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[10]);
            break;
        case ('L'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[11]);
            break;   
        case ('l'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[11]);
            break;   
        case ('M'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[12]);
            break;
        case ('m'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[12]);
            break;
        case ('N'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[13]);
            break;    
        case ('n'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[13]);
            break;    
        case ('O'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[14]);
            break;
        case ('o'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[14]);
            break;
        case ('P'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[15]);
            break;
        case ('p'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[15]);
            break;
        case ('Q'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[16]);
            break;    
        case ('q'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[16]);
            break; 
        case ('R'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[17]);
            break;
        case ('r'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[17]);
            break;
        case ('S'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[18]);
            break;    
        case ('s'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[18]);
            break;  
        case ('T'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[19]);
            break;
        case ('t'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[19]);
            break;
        case ('U'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[20]);
            break;
        case ('u'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[20]);
            break;
        case ('V'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[21]);
            break;    
        case ('v'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[21]);
            break;    
        case ('W'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[22]);
            break;
        case ('w'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[22]);
            break;
        case ('X'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[23]);
            break;    
        case ('x'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[23]);
            break;    
        case ('Y'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[24]);
            break;
        case ('y'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[24]);
            break;
        case ('Z'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[25]);
            break;
        case ('z'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[25]);
            break;
        case ('/'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[26]);
            break;
        case (':'):
            draw_sprite(x_initial + i*LETTER_SPACING, y, &l->n[27]);
            break;
        default:
            break;
        }
    }
    return 0;

}
