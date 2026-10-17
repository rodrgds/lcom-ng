#ifndef _VBE_H_
#define _VBE_H_

#include "./../../snake.h"
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <minix/sysutil.h>

#define SET_VBE_MODE_AH 0x4F
#define SET_VBE_MODE_AL 0x02
#define RETURN_VBE_MODE_INFO_AH 0x4F
#define RETURN_VBE_MODE_INFO_AL 0x01
#define RETURN_VBE_CONTROLLER_INFORMATION_AH 0x4F
#define RETURN_VBE_CONTROLLER_INFORMATION_INFO_AL 0x00
#define INDEXED_MODE 0x105
#define FIRST_DIRECT_COLOR_MODE 0x110
#define SECOND_DIRECT_COLOR_MODE 0x115
#define THIRD_DIRECT_COLOR_MODE 0x11A
#define FOURTH_DIRECT_COLOR_MODE 0x14C
#define CGA_MODE 0x03
#define TRANSPARENCY_COLOR_1_5_5_5 0x8000
#define INT_NUMBER 0x10


/**
 *@brief Função que configura o modo gráfico
 *
 * 
 * @param mode modo gráfico
 * 
 * @return retorna 0 em caso de sucesso
 */
int(vbe_set_mode)(uint16_t mode);

/**
 *@brief Função que configura o modo gráfico e mapeia a memória de vídeo
 *
 * 
 * @param mode modo gráfico
 * 
 */
void *(vg_init) (uint16_t mode);


/**
 *@brief Função que desenha um pixel no ecrã
 *
 * 
 * @param x coordenada horizontal onde desenhar o pixel
 * @param y coordenada vertical onde desenhar o pixel
 * @param color cor do pixel
 * 
 * @return retorna 0 em caso de sucesso
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);


/**
 *@brief Função que desenha um retângulo no ecrã
 *
 * 
 * @param x coordenada horizontal onde começar a desenhar o retângulo
 * @param y coordenada vertical onde começar a desenhar o retângulo
 * @param width largura do retângulo
 * @param height altura do retângulo
 * @param color cor do retângulo
 * 
 * @return retorna 0 em caso de sucesso
 */
int(our_vg_draw_rectangle_implementation)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 *@brief Função que limpa o buffer da memória de vídeo
 *
 * 
 */
void (vg_clear)();


/**
 *@brief Função que desenha uma imagem em formato xpm no ecrã
 *
 * 
 * @param x coordenada horizontal onde começar a desenhar o xpm
 * @param y coordenada vertical onde começar a desenhar o xpm
 * 
 * @return retorna 0 em caso de sucesso
 */
int(draw_xpm_image)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif
