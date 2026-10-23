#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <stddef.h>
#include <stdint.h>
#include <lcom/xpm.h>
#include "../../lab1/rtc.h"

// Inicializa o modo gráfico e mapeia a VRAM,  retorna 0 em caso de sucesso ou 1 em caso de erro
int renderer_init(void);

// Encerra o modo gráfico e limpa os recursos alocados
void renderer_cleanup(void);

// Desenha o menu principal (por agora, apenas o fundo)
void renderer_draw_menu(void);

// Desenha o jogo (fundo, player e balas)
void renderer_draw_game(void);

// Desenha o ecra correspondente ao estado atual do jogo
void renderer_draw(void);

// Desenha um sprite na posição (x, y) usando a imagem e o mapa de cores fornecidos
void renderer_draw_sprite(xpm_image_t img, uint8_t *map, int x, int y);

// Desenha o menu de pausa
void renderer_draw_paused(void);

// Desenha a tela de game over
void renderer_draw_game_over(void);

// Desenha a tela de vitória
void renderer_draw_victory(void);

// Getters para dimensões da bala
int renderer_get_bullet_width(void);
int renderer_get_bullet_height(void);
int renderer_get_enemy_bullet_width(void);
int renderer_get_enemy_bullet_height(void);

// Desenha um sprite com um efeito de tint vermelho, para parecer que o player ou inimigo está a levar dano
void renderer_draw_sprite_red_tint(xpm_image_t img, uint8_t *map, int x, int y);

void renderer_set_victory_date(const rtc_date *date);

#endif /* _RENDERER_H_ */
