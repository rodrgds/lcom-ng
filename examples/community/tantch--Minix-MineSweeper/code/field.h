#ifndef FIELD_H
#define FIELD_H
#include "vbe.h"
#include <minix/syslib.h>
#include <minix/drivers.h>

 /**
  *
  * Struct de cada quadrado do jogo qeu contem o conteudo de cada quadrado
  */

typedef struct {
	int revealed;
	int mine;
	int flag;
	int number;

} Campo_t;
/**
  *Funcao que preenche todo o ecra com  o buffer com uma cor
  *@param color Cor com que o ecra fica
  */
int vg_fill(unsigned long color);
/**
  * Desenha o rato no buffer
  *@param topx cordenada x da ponta da seta
  *@param top y cordenada u da ponta da seta
  *@param color cor do rato
  */
void draw_mouse(int topx, int topy, unsigned long color);
/**
  * 	desenha um campo de jogo
  *@param first_click A 1 se o primeiro clique ja foi feito
  *@param field campo do jogo
  */
void draw_grid_easy(int first_click, Campo_t field[][getLado()]);
/**
  * devolve a cordenada x do centro da coluna
  *@param col coluna do campo de jogo
  */
unsigned getx(int col);
/**
  * devolve a cordenada y do centro da linha
  *@param row linha do campo de jogo
  */
unsigned gety(int row);
/**
  * desenha no buffer uma mina
  *@param row linha onde e desenhada a mina
  *@param col coluna onde e desenhada a mina
  *@param color cor da mina
  */
void draw_mine(int row, int col,int color);
/**
  * desenha no buffer uma bandeira
  *@param row linha onde e desenhada a bandeira
  *@param col coluna onde e desenhada a bandeira
  *@param color cor da mina
  */
void draw_flag(int row, int col, unsigned long color);
/**
  * muda o fundo do quadrado para como que clicado
  *@param row linha do quadrado
  *@param col coluna do quadrado
  */
void square_clicked(unsigned long row, unsigned long col);
/**
  * desenha o numero no quadrado
  *@param row clinha do quadrado
  *@param col coluna do quadrado
  *@param number numero a desenhar
  */
void draw_number(int row, int col, int number);
/**
  * devolve a linha onde esta o ponto y
  *@param y cordenada y

  */
int getrow(int y);
/**
  *  devolve a coluna onde esta o ponto x
  *@param x cordenada x

  */
int getcol(int x);
/**
  * desenha um pixel no buffer
  *@param x cordenada horizontal do ponto
  *@param y cordenada vertical do ponto
  *@param color cor do ponto
  */
void set_pixel(int x, int y, unsigned long color);
/**
  * Passa o qeu esta desenahdo no buffer para o ecra
  */
void update_graf();
/**
  * desenha uma cruz na mina clicada
  *@param i coluna da mina
  *@param j coluna da mina
  */
void draw_lose(int i, int j);
/**
  * devolve o lado do campo de jogo
  */
int getLado();
/**
  * altera o valor do lado
  *@param ld tamanho do campo de jogo
  */
void setLado(int ld);
/**
  * devolve o numero de minas
  */
int getMines();
/**
  * altera o numero de minas
  *@param mn numero das minas

  */
void setMines(int mn);
/**
  * devolve numero de bandeiras em jogo
  */
int getFlags();
/**
  * altera o numero de bandeiras em jogo
  *@param n numero de bandeiras
  */
void setFlags(int n);
/**
  * incrementa o numero de bandeiras
  */
void incFlags();
/**
  * decrementa o numero de bandeiras
  */
void decFlags();
/**
  * devolve o numero de bandeiras certas
  */
int getFlagsCertas();
/**
  * altera o numero de bandeiras certas em jogo
  *@param n numero de bandeiras certas
  */
void setFlagsCertas(int n);
/**
  * incrementa o numero de bandeiras certas
  */
void incFlagsCertas();
/**
  * decrementa o numero de bandeiras certas
  */
void decFlagsCertas();
/**
  * devolve a cor das bandeiras em jogo

  */
int getFlagColor();
/**
  * altera a cor das bandeiras em jogo
 *@param n cor das bandeiras
  */
void setFlagColor(int n);
/**
  *Faz reset ao campo para um novo jogo

  */
void reset(Campo_t field[][getLado()]);
/**
  *desenha o menu
	*@param option opcao selecionada
  */
void draw_menu(int option);
/**
  *devolve os pontos do player1

  */
int getPlayer1();
/**
  *incrementa os pontos do player1

  */
void incPlayer1();
/**
  *devolve os pontos do player2

  */
int getPlayer2();
/**
  *incrementa os pontos do player2

  */
void incPlayer2();
#endif
