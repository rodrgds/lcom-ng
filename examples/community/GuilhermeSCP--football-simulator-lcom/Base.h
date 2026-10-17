#ifndef __BASE_H_
#define __BASE_H_

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <stdio.h>

/*======GUARDA-REDES=======*/
#define GR = 1
/*======DEFESAS=======*/
#define DEF = 2
/*======MÉDIOS=======*/
#define MED = 3
/*======AVANÇADOS=======*/
#define ATA = 4


typedef struct _Jogador
{
	char *nome;
	int ove;
	int pos;
	int tit;
	int num;
}Jogador;

typedef struct _Posicao
{
	int nome;
	int pos_ecra;
}Posicao;

typedef struct _Tactica
{
	char *nome;
	int def;
	int med;
	int ata;
	/*Posicao pos1;
	Posicao pos2;
	Posicao pos3;
	Posicao pos4;
	Posicao pos5;
	Posicao pos6;
	Posicao pos7;
	Posicao pos8;
	Posicao pos9;
	Posicao pos10;
	Posicao pos11;*/
}Tactica;

typedef struct _Equipa
{
	Tactica tac;
	Jogador jog1;
	Jogador jog2;
	Jogador jog3;
	Jogador jog4;
	Jogador jog5;
	Jogador jog6;
	Jogador jog7;
	Jogador jog8;
	Jogador jog9;
	Jogador jog10;
	Jogador jog11;
	Jogador jog12;
	Jogador jog13;
	Jogador jog14;
	Jogador jog15;
	Jogador jog16;
	Jogador jog17;
	Jogador jog18;
	int ove_t;
	int num;
}Equipa;

typedef struct _Tacticas
{
	Tactica tac1;
	Tactica tac2;
	Tactica tac3;
	Tactica tac4;
	Tactica tac5;
	Tactica tac6;
	Tactica tac7;
}Tacticas;

Equipa equipa1;
Equipa equipa2;
Equipa equipa3;
Equipa equipa4;
Tacticas tacticas;

void criaEquipasTacticas();
int getDefOv(Equipa e);
int getMedOv(Equipa e);
int getAtaOv(Equipa e);
int getGROv(Equipa e);
int getNDef(Tactica tac);
int getNMed(Tactica tac);
int getNAta(Tactica tac);
char getNameJogador(Equipa e, int num);
int getOvJogador(Equipa e, int num);
char getPosJogador(Equipa e, int num);
void setEquipa (int e, int nro);
int getNumEquipa(Equipa e);
void Substituicao(Equipa e, int num1, int num2);
#endif __BASE_H_
