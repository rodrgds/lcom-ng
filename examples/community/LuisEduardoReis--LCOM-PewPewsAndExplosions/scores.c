#include "scores.h"
#include "stdio.h"
#include "stdlib.h"
#include "sprites.h"
#include "util.h"
#include "game.h"

extern char* filepath;

static Highscore highscores[10];

/*!
 * Guarda as Pontuacoes mais altas no ficheiro "highscores.txt".
 */
void SaveScores() {

	FILE *file;

	char* path = malloc(strlen(filepath) + strlen("/highscores.txt") + 8);
	strcpy(path,filepath);
	strcat(path,"/highscores.txt");

	file = fopen(path, "wb");
	free(path);

	if (file == NULL) return;
	//printf("saving\n");

	int i;
	for(i = 0; i < 10; i++) {
		fwrite(&highscores[i], 1, sizeof(Highscore), file);
	}

	fclose(file);
};

/*!
 * Adiciona uma pontuacao à lista de pontuacoes mais altas mantendo a ordem.
 * \param h pontuacao a adicionar
 */
void AddScore(Highscore h) {
	int i; Highscore aux;
	for(i = 0; i < 10; i++)
		if (highscores[i].score < h.score) {
			aux = highscores[i];
			highscores[i] = h;
			h = aux;
		}
};

/*!
 * Devolve a pontuacao mais alta actualmente
 * \return estrutura com informacoes sobre a pontacao mais alta
 */
Highscore getHighscore() {return highscores[0];}

/*!
 * Carrega as Pontuacoes mais altas a partir do ficheiro "highscores.txt" caso exista.
 * Caso contrario sao inicializadas com "lugares vazios"
 */
void LoadScores() {
	FILE *file;

	char* path = malloc(strlen(filepath) + strlen("/highscores.txt") + 8);
	strcpy(path,filepath);
	strcat(path,"/highscores.txt");

	file = fopen("/home/lcom/projeto/highscores.txt", "rb");
	free(path);

	int i;
	for(i = 0; i < 10; i++) {
			strcpy(highscores[i].name,"<None>");
			highscores[i].score = -1;
	}


	if (file == NULL) return;
	//printf("loading\n");

	int r;
	for(i = 0; i < 10; i++) {
		fread(&highscores[i], 1, sizeof(Highscore), file);
	}


	fclose(file);
};


static int anim = 0;
/*!
 * Desenha a lista de pontuacoes mais altas no ecra
 * \param x coordenada x onde desenhar a lista
 * \param y coordenada y onde desenhar a lista
 */
void DrawScores(int x, int y) {
	int i,n = 0; char text[128]; Highscore h;
	for(i = 0; i < 10; i++) {
		h = highscores[i];
		if (h.score < 0)
			sprintf(text, "%2dX - %d - %s%n",i+1,max(0,h.score),h.name,&n);
		else
			sprintf(text, "%2dX - %d - %s %.2x|%.2x|20%.2x %.2x:%.2x%n",i+1,max(0,h.score),h.name,h.date.day,h.date.month,h.date.year,h.date.hours,h.date.minutes,&n);
		text[2] = 0xF8;

		if (h.score >= 0 && i == 0)
			drawTextBlend(text, x - n*4 - 4, y + i*15, hsvTorgb(anim,1.0,1.0));
		else
			drawText(text, x - n*4 - 4, y + i*15);
	}
	anim += 5 * (60.0/FPS);
}
