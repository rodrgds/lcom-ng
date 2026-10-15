#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <lcom/lcf.h>
#include "menu.h"

/**
 * @brief Desenha e inicializa o background do jogo
 * @param background Buffer com o conteúdo do background
 * @returns 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (drawBackground)(uint32_t *background);

/**
 * @brief Inicializa os valores necessários para que o background do menu funcione
 * @param background Buffer com o conteúdo do background
 * @param isStart Usado para verificar qual menu é que vai ser inicializado
 * @param menu Menu que vai ter o seu background inicializado
 * @param day Dia atual
 * @param month Mês atual
 * @param year Ano atual
 * @returns 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (initializeMenuBackground)(uint32_t *background, bool isStart, Menu *menu, int day, int month, int year);

/**
 * @brief Desenha a data no background do menu
 * @param background Buffer com o conteúdo do background
 * @param day Dia atual
 * @param month Mês atual
 * @param year Ano atual
 * @returns 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (drawDateBackground)(uint32_t *background, int day, int month, int year);

/**
 * @brief Desenha um elemento no background de um menu
 * @param background Buffer com o conteúdo do background
 * @param element Elemento a desenhar
 * @param x posição x inicial
 * @param y posição y inicial
 * @param width Comprimento do elemento
 * @param height Altura do elemento
 * @returns 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (drawElementsInMenuBackground)(uint32_t *background, uint32_t *element, int x, int y, int width, int height);

/**
 * @brief Atualiza o background para criar um novo frame, apagando os conteúdos que se encontravam antes no buffer da gráfica
 * @param background Buffer com o conteúdo do background
 * @returns 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (refreshBackground)(uint32_t *background);

#endif
