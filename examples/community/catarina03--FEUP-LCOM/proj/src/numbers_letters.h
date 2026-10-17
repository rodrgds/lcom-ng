#include <lcom/lcf.h>
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "stdlib.h"
#include "grafics.h"
#include "keyboard.h"
#include "i8042.h"
#include "game_structs.h"
#include "game_macros.h"
#include <stdbool.h>
#include <stdint.h>

#include "img_files/zero.xpm"
#include "img_files/um.xpm"
#include "img_files/dois.xpm"
#include "img_files/tres.xpm"
#include "img_files/quatro.xpm"
#include "img_files/cinco.xpm"
#include "img_files/seis.xpm"
#include "img_files/sete.xpm"
#include "img_files/oito.xpm"
#include "img_files/nove.xpm"
#include "img_files/A.xpm"
#include "img_files/B.xpm"
#include "img_files/C.xpm"
#include "img_files/D.xpm"
#include "img_files/E.xpm"
#include "img_files/F.xpm"
#include "img_files/G.xpm"
#include "img_files/H.xpm"
#include "img_files/I.xpm"
#include "img_files/J.xpm"
#include "img_files/K.xpm"
#include "img_files/L.xpm"
#include "img_files/M.xpm"
#include "img_files/N.xpm"
#include "img_files/O.xpm"
#include "img_files/P.xpm"
#include "img_files/Q.xpm"
#include "img_files/R.xpm"
#include "img_files/S.xpm"
#include "img_files/T.xpm"
#include "img_files/U.xpm"
#include "img_files/V.xpm"
#include "img_files/W.xpm"
#include "img_files/X.xpm"
#include "img_files/Y.xpm"
#include "img_files/Z.xpm"
#include "img_files/barra.xpm"
#include "img_files/dois_pontos.xpm"


/**
 * @brief Create a Letras object.
 * 
 * @return Letras* - Letras pointer to the Letras struct object created
 */
Letras* createLetras();

/**
 * @brief Prints a certain letter on the screen.
 * 
 * @param frase - array of characters to be printed on the screen
 * @param l - Letras pointer to a Letras object
 * @param x - X coordinate of the letter position on the screen 
 * @param y - Y coordinate of the letter position on the screen 
 * @return int 
 */
int printLetras(char frase[], Letras *l, int x, int y);

/**
 * @brief Create a Nums object.
 * 
 * @return Numeros - Numeros object created
 */
Numeros createNums();

/**
 * @brief Converts the time_in_seconds to minutes : seconds format.
 * 
 * @param time_in_seconds - The number we want to convert 
 * @param n - Numeros pointer to a Numeros object
 * @return int 0 if successful
 */
int countdown(int time_in_seconds,Numeros *n);

/**
 * @brief Prints a certain number on the screen.
 * 
 * @param num - Number to be printed
 * @param n - Numeros pointer to a Numeros object
 * @param x - X coordinate of the number position on the screen
 * @param y - Y coordinate of the number position on the screen
 * @return int - 0 if successful
 */
int printNum(int num,Numeros *n,int x,int y);

