#ifndef __TUI_H_
#define __TUI_H_

#include <minix/sysutil.h>
#include "video_txt.h"
#include "kbc.h"
#include "Jogo.h"
#include "Base.h"
#include "i8254.h"
#include <stdio.h>

#define	BLACK	0x00
#define GREY 0x08
#define YELLOW 0x06
#define PURPLE 0x05
#define RED	(0x01 << 2)
#define LIGHT_BLUE 0x03
#define GREEN	(0x01 << 1)
#define BLUE	0x01
#define WHITE	(RED | GREEN | BLUE)

typedef struct _botao
{
	int x;
	int y;
	int comp;
	int alt;
	char *name;
} botao;

typedef struct _Rato
{
	int x;
	int y;
	int rb;
	int lb;
	unsigned long irq;
} Rato;

void init_kbc();
void set_pos_rato(Rato * r, short newx, short newy);
void desenhar_rato(Rato * r, int x, int y);
int desenhaLinha(int x, int y, int fin, int ori);
int pontoMedio(int x, int y);
int desenhaBotao(botao bot);
void MenuPrincipal(Rato * r);
void JogoAmigavel(Rato * r);
void JogoRapido(Rato * r);
void _Opcoes(Rato * r);
void _Sair();
void EscEquipas(Rato * r);
void PreJogo(Rato * r);
void EscPlantel(Rato * r);
void EscFormacao(Rato * r);
void EscEstiloJogo(Rato * r);
void SimularJogo(Rato * r);
void PosJogo(Rato * r);
void AltCorLetras(Rato * r);
void AltCorLinhas(Rato * r);
void AltCorRato(Rato * r);
void _Dificuldade(Rato * r);
void AltSom(Rato * r);
void CampoFutebol();
void Mostra(Rato * r);
void desenhaPlantel(Equipa e);
int desenhaFormacao(Equipa e);

#endif __TUI_H_
