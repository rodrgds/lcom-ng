#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <lcom/lcf.h>

/**
 * @brief Inicializa o buffer da placa gráfica (vram) e inicializa o modo gráfico
 * @param Mode modo gráfico pretendido
 * @returns 1 se ocorrer um erro e 0 caso tenha corrido tudo bem
*/
int (map_VRAM)(uint16_t mode);

/**
 * @brief Desenha a cor num dos píxeis do ecrã
 * @param x valor do x
 * @param y valor o y
 * @param color Cor a ser desenhada píxel
 * @returns 1 se ocorrer um erro e 0 caso tenha corrido tudo bem
*/
int (vg_draw_color)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Retorna a informação do modo gráfico em uso para que possa ser usada em outros documentos
 * @returns informação do modo gráfico
*/
vbe_mode_info_t (get_mode_inf)();

/**
 * @brief Retorna a altura máxima do modo gráfico em uso
 * @returns altura máxima do modo gráfico em uso
*/
unsigned (get_hres)();

/**
 * @brief Retorna a largura máxima do modo gráfico em uso
 * @returns largura máxima do modo gráfico em uso
*/
unsigned (get_vres)();

/**
 * @brief Retorna a quantidade de bits por píxel no modo gráfico em uso
 * @returns quantidade de bits por píxel no modo gráfico em uso
*/
unsigned (get_bits_per_pixel)();

/**
 * @brief Retorna a quantidade de bytes por píxel no modo gráfico em uso
 * @returns quantidade de bytes por píxel no modo gráfico em uso
*/
unsigned (get_bytes_per_pixel)();

/**
 * @brief Retorna o buffer da memória gráfica
 * @returns buffer da memória gráfica
*/
char* (get_video_mem)();

/**
 * @brief Passa o conteúdo do segundo buffer para o buffer principal da memória gráfica
 * @returns conteúdo do segundo buffer para o buffer principal da memória gráfica
*/
void (swap_buffer)();

/**
 * @brief Retorna o segundo buffer
 * @return segundo buffer
*/
char* (get_second_buffer)();

/**
 * @brief Liberta a memória alocada para o segundo buffer
*/
void (free_second_buffer)();

/**
 * @brief limpa a tela
 * @return 1 se algum erro ocorrer se não 0
*/
int (clear_screen)();

#endif
