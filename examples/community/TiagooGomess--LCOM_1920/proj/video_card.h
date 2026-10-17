#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "sprite.h"
#include "RTC.h"

#define INDEXED_COLOR 0x04
#define DIRECT_COLOR 0x06

#define h_res 1280
#define v_res 1024

/**
 * @brief - Atualiza a palavra atual, retirando o primeiro caracter da palavra atual se o scancode da tecla teclada corresponder a esse caracter.
 * @param mode - Modo gráfico pretendido.
 */
void *(vg_init)(uint16_t mode);

/**
 * Cria todos os sprites necessários.
 */
void init_all_sprites();

/**
 * @brief - Desenha o fundo correspondente ao Sprite* sprite.
 * @param sprite - Fundo a desenhar.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 */
void draw_backgroud(Sprite* sprite, uint8_t* initial_address);

/**
 * @brief - Desenha sprite do rato.
 * @param sprite - Sprite do rato a desenhar.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 */
void draw_cursor(Sprite* sprite, uint8_t* initial_address);

/**
 * @brief - Desenha tudo o que precisamos no jogo (fundo, palavra atual, score, level e timer).
 * @param word - Palavra atual.
 * @param score - Pontuação atual.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 * @param y0_word - Coordenada y onde a palavra começa a ser desenhada.
 */
void draw_everything_in_game(char* word, int score, uint8_t* initial_address, int y0_word);

/**
 * Faz o free de todos os sprites inicializados.
 */
void free_everything();
