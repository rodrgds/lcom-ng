#include <time.h>
#include "GameState.h"

#include "stdio.h"
#include "score.h"

#define MAKECODE_A 0x1e
#define MAKECODE_D 0x20
#define OBS_LEFT_X     200
#define OBS_CENTER_X   340
#define OBS_RIGHT_X    480
#define POKE_LEFT_X    230
#define POKE_CENTER_X  370
#define POKE_RIGHT_X   510
#define BACKGROUND_SPEED    3

GameState* newGameState(){
	srand(time(NULL));
	GameState* g = (GameState*) malloc(sizeof(GameState));

	g->s=newScore();
	g->y=0;
	g->complete=0;
	g->counter=0;
	g->sec=0;
	g->background=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/gamebackground.bmp");
	g->player=newPlayer();
	g->vel=BACKGROUND_SPEED;
	int i, dist=0;
	for(i=0;i<OBS_SIZE;i++)
	{
		g->obs_l[i] = newObstacle(OBS_LEFT_X, dist);
		g->obs_l[i]->use=1;
		g->obs_c[i] = newObstacle(OBS_CENTER_X, dist);
		g->obs_c[i]->use=0;
		g->obs_r[i] = newObstacle(OBS_RIGHT_X, dist);
		g->obs_r[i]->use=1;
		dist+=200;
	}
	dist=100;
	for(i=0;i<POKE_SIZE;i++)
	{
		g->poke_l[i]= newPoke(POKE_LEFT_X,dist);
		g->poke_c[i]= newPoke(POKE_CENTER_X,dist);
		g->poke_r[i]= newPoke(POKE_RIGHT_X,dist);
		dist+=200;
	}

	return g;
}

int updateGameState(GameState* g, unsigned long scancode, int counter) {
	if (scancode == VAL_ESC) {
		g->complete = 1;
		return returnScore(g->s);
	}
	updateCounterGame(g);
	moveBackGround(g);
	movePlayer(g->player, scancode);
	updateCounterPlayer(g->player);
	if ((g->sec == 15 && g->counter == 0) || (g->sec == 30 && g->counter == 0) || (g->sec == 60 && g->counter == 0)) {
		updateAllObstacles(g, 1);
		updateAllPokeballs(g,1);
	} else {
		updateAllObstacles(g, 0);
		updateAllPokeballs(g,0);
	}

	if ((g->sec == 3 && g->counter == 0) || (g->sec == 30 && g->counter == 0) || (g->sec == 60 && g->counter == 0))
		updatePlayerSpeed(g->player,1);

	if (checkPokeballsCollision(g) == 1)
		updateScore(g->s, 1);
	else
		updateScore(g->s, 0);

	if (checkEndGame(g) == 1) {
		return returnScore(g->s);
	}
	return 0;

}

void drawGameState(GameState* g){
	int score;
	drawBitmap(g->background,0,g->y,ALIGN_LEFT);
	drawPlayer(g->player);
	drawScore(g->s);

	unsigned i;
	for(i=0;i<OBS_SIZE;i++)
	{
		if (g->obs_l[i]->use == 1)
			drawObstacle(g->obs_l[i]);
		if (g->obs_c[i]->use == 1)
			drawObstacle(g->obs_c[i]);
		if (g->obs_r[i]->use == 1)
			drawObstacle(g->obs_r[i]);
	}
	for(i=0;i<POKE_SIZE;i++)
	{
		if (g->poke_l[i]->use == 1 && g->poke_l[i]->val==1)
			drawPoke(g->poke_l[i]);
		if (g->poke_c[i]->use == 1 && g->poke_c[i]->val==1)
			drawPoke(g->poke_c[i]);
		if (g->poke_r[i]->use == 1 && g->poke_r[i]->val==1)
			drawPoke(g->poke_r[i]);
	}

}

void deleteGameState(GameState* g) {
	if(g==NULL)
		return;
	deleteBitmap(g->background);
	deletePlayer(g->player);
	unsigned i;
	for(i=0;i<OBS_SIZE;i++)
	{
		deleteObstacle(g->obs_l[i]);
		deleteObstacle(g->obs_c[i]);
		deleteObstacle(g->obs_r[i]);
	}
	for(i=0;i<POKE_SIZE;i++)
	{
		deletePoke(g->poke_l[i]);
		deletePoke(g->poke_c[i]);
		deletePoke(g->poke_r[i]);
	}
	deleteScore(g->s);
	free(g);
}

void moveBackGround(GameState* g){
	g->y+=g->vel;
	if(g->y>=getVerResolution())
		g->y=0;
	if ((g->sec == 15 && g->counter == 0) || (g->sec == 30 && g->counter == 0) || (g->sec == 60 && g->counter == 0))
		g->vel++;
}

int checkEndGame(GameState *g){
	int final = 0;
	unsigned i;
	Collisions* player;
	Collisions* col_l;
	Collisions* col_c;
	Collisions* col_r;
	for(i=0;i<OBS_SIZE;i++)
	{
		player = newCollision(g->player->x, g->player->y,
				g->player->x + g->player->wPlayer,
				g->player->y + g->player->hPlayer);
		col_l = newCollision(g->obs_l[i]->x, g->obs_l[i]->y,
				g->obs_l[i]->x + g->obs_l[i]->wh,
				g->obs_l[i]->y + g->obs_l[i]->ht);
		col_c = newCollision(g->obs_c[i]->x, g->obs_c[i]->y,
				g->obs_c[i]->x + g->obs_c[i]->wh,
				g->obs_c[i]->y + g->obs_c[i]->ht);
		col_r = newCollision(g->obs_r[i]->x, g->obs_r[i]->y,
				g->obs_r[i]->x + g->obs_r[i]->wh,
				g->obs_r[i]->y + g->obs_r[i]->ht);

		if (checkCollision(player, col_l) == 1)
			final = 1;
		if (checkCollision(player, col_c) == 1)
			final = 1;
		if (checkCollision(player, col_r) == 1)
			final = 1;
	}

	deleteCollision(player);
	deleteCollision(col_l);
	deleteCollision(col_c);
	deleteCollision(col_r);

	return final;
}

void updateAllObstacles(GameState* g,int incSpeed){
	unsigned i;
	for (i = 0; i < OBS_SIZE; i++) {
		while (1) {
			if (g->obs_l[i]->use + g->obs_c[i]->use + g->obs_r[i]->use == 2)
				break;
			updateObstacleUse(g->obs_l[i]);
			updateObstacleUse(g->obs_c[i]);
			updateObstacleUse(g->obs_r[i]);
		}

		updateObstacleSpeed(g->obs_l[i],incSpeed);
		updateObstacleSpeed(g->obs_c[i],incSpeed);
		updateObstacleSpeed(g->obs_r[i],incSpeed);

		if (g->obs_l[i]->use == 1) {
			updateObstacle(g->obs_l[i]);
		} else {
			g->obs_l[i]->y = 0;
		}

		if (g->obs_c[i]->use == 1) {
			updateObstacle(g->obs_c[i]);
		} else {
			g->obs_c[i]->y = 0;
		}

		if (g->obs_r[i]->use == 1) {
			updateObstacle(g->obs_r[i]);
		} else {
			g->obs_r[i]->y = 0;
		}
	}
}

void updateCounterGame(GameState* g) {
	g->counter++;
	if(g->counter==60)
	{
		g->sec++;
		g->counter=0;
	}
}

void updateAllPokeballs(GameState* g,int incSpeed){
	unsigned i;
	for (i = 0; i < POKE_SIZE; i++) {
		while (1) {
			if (g->poke_l[i]->use + g->poke_c[i]->use + g->poke_r[i]->use ==1)
				break;
			updatePokeUse(g->poke_l[i]);
			updatePokeUse(g->poke_c[i]);
			updatePokeUse(g->poke_r[i]);
		}

		updatePokeSpeed(g->poke_l[i],incSpeed);
		updatePokeSpeed(g->poke_c[i],incSpeed);
		updatePokeSpeed(g->poke_r[i],incSpeed);

		if (g->poke_l[i]->use == 1)
			updatePoke(g->poke_l[i]);
		else
			g->poke_l[i]->y = 0;

		if (g->poke_c[i]->use == 1)
			updatePoke(g->poke_c[i]);
		else
			g->poke_c[i]->y = 0;

		if (g->poke_r[i]->use == 1)
			updatePoke(g->poke_r[i]);
		else
			g->poke_r[i]->y = 0;
	}
}

int checkPokeballsCollision(GameState* g){
	int final = 0;
	unsigned i;
	Collisions* player;
	Collisions* pokeball_l;
	Collisions* pokeball_c;
	Collisions* pokeball_r;
	for (i = 0; i < POKE_SIZE; i++) {
		player = newCollision(g->player->x, g->player->y,
				g->player->x + g->player->wPlayer,
				g->player->y + g->player->hPlayer);
		pokeball_l = newCollision(g->poke_l[i]->x, g->poke_l[i]->y,g->poke_l[i]->x + g->poke_l[i]->wh,g->poke_l[i]->y + g->poke_l[i]->ht);
		pokeball_c = newCollision(g->poke_c[i]->x, g->poke_c[i]->y,g->poke_c[i]->x + g->poke_c[i]->wh,g->poke_c[i]->y + g->poke_c[i]->ht);
		pokeball_r = newCollision(g->poke_r[i]->x, g->poke_r[i]->y,g->poke_r[i]->x + g->poke_r[i]->wh,g->poke_r[i]->y + g->poke_r[i]->ht);

		if (checkCollision(player, pokeball_l) == 1 && g->poke_l[i]->val==1)
			{
			g->poke_l[i]->val=0;
			final = 1;
			}
		if (checkCollision(player, pokeball_c) == 1 && g->poke_c[i]->val==1)
			{
			g->poke_c[i]->val=0;
			final = 1;
			}
		if (checkCollision(player, pokeball_r) == 1 && g->poke_r[i]->val==1)
			{
			g->poke_r[i]->val=0;
			final = 1;
			}
	}

	deleteCollision(player);
	deleteCollision(pokeball_l);
	deleteCollision(pokeball_c);
	deleteCollision(pokeball_r);

	return final;
}
