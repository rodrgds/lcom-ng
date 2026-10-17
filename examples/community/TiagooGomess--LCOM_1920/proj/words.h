#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sprite.h"
#include "video_card.h"
#include "Letras.h"
#include "scancodes.h"

/**
 * @brief - Gera uma palavra aleatória do ficheiro das palavras (words.c).
 * @return - A palavra escolhida.
 */
char* get_word();

/**
 * @brief - Atualiza a palavra atual, retirando o primeiro caracter da palavra atual se o scancode da tecla teclada corresponder a esse caracter.
 * @param c - Caracter que foi digitado.
 */
void update_word(char c);

/**
 * @brief - Obtém o XMP do caracter c.
 * @param c - Caracter.
 * @return - XPM do caracter c.
 */
xpm_row_t* getXPM(char c);

/**
 * @brief - Obtém o comprimento do xpm do caracter c.
 * @param c - Caracter.
 * @return - Comprimento pretendido.
 */
unsigned get_letter_width(char c);

/**
 * @brief - Desenha no ecrã a palavra pretendida.
 * @param x0 - Coordenada x onde a palavra começa a ser desenhada.
 * @param y0 - Coordenada y onde a palavra começa a ser desenhada.
 * @param word - Palavra que queremos desenhar.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 */
void displayWord(int x0, int y0, char* word, uint8_t* initial_address);

/**
 * @brief - Desenha no ecrã a palavra pretendida, com uma certa cor.
 * @param x0 - Coordenada x onde a palavra começa a ser desenhada.
 * @param y0 - Coordenada y onde a palavra começa a ser desenhada.
 * @param color - Cor da palavra.
 * @param word - Palavra que queremos desenhar.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 */
void displayWordWithColor(int x0, int y0, uint32_t color, char* word, uint8_t* initial_address);

/**
 * @brief - Desenha no ecrã o número pretendido.
 * @param x0 - Coordenada x onde a palavra começa a ser desenhada.
 * @param y0 - Coordenada y onde a palavra começa a ser desenhada.
 * @param num - Número que queremos desenhar.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 */
void displayNumber(int x0, int y0, unsigned num, uint8_t* initial_address);

/**
 * @brief - Obtém o Sprite* correspondente ao caracter c.
 * @param c - Caracter.
 * @return - Sprite* correspondente ao caracter c.
 */
Sprite* getSprite(char c);


