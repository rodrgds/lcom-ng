#ifndef __PROJ_H
#define __PROJ_H

#include <stdlib.h>
#include "video_gr.h"
#include "pixmap.h"
#include "KBC.h"
#include "RTC.h"
#include "timer.h"
#include "highscores.h"
#include "algarismos.h"
#include "algarismos_small.h"
#include "alfabeto.h"
#include "interface_xpm.h"
#include <string.h>


typedef struct {
	int verticalPos;
	int horizontalPos;
} missile;

typedef struct {
	int verticalPos;
	int horizontalPos;
} enemy;

typedef struct {
	int verticalPos;
	int horizontalPos;
	int vida;
} boss;

typedef struct player_score {
	char nome[15];
	long  pontuacao;
	long int dia;
	long int mes;
	long int ano;
	long int hora;
	long int minuto;
} player_score;

/** @brief Ordena array de Highscores @param Array de Highscores */
void ordenaHighScores( player_score arrayResultados[]);
/** @brief Lê os Highscores do ficheiro @param Array de Highscores */
void readHighScores(player_score arrayResultados[]);
/** @brief Escreve os Novos Highscores no Ficheiro @param Array de Highscores */
void writeHighScores( player_score arrayResultados[]);
/** @brief Modulo do Jogo @param irq @param ipc @param message */
int jogo(int _irq_set, int _ipc_status, message _msg);
/** @brief Actualiza a posicao dos enimigos */
void actualizaEnemy(int move);
/** @brief Insercao do nome do jogador @param irq @param ipc @param message */
void insertNome(player_score jogador, int _irq_set, int _ipc_status, message _msg);
/** @brief Checka as colisoes dos misseis com inimigos @param array de misseis */
void checkColisao(missile vetor_misseis[]);
/** @brief Cria um enimigo */
void createEnemy();
/** @brief Cria a user interface */
void drawGUI();

#endif /* __PROJ */
