#include "score.h"
#include "Bitmap.h"

#define POKE_SCORE   60

Score* newScore() {
	Score* s = (Score*) malloc(sizeof(Score));
	s->score=0;
	s->score1=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score1.bmp");
	s->score2=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score2.bmp");
	s->score3=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score3.bmp");
	s->score4=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score4.bmp");
	s->score5=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score5.bmp");
	s->score6=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score6.bmp");
	s->score7=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score7.bmp");
	s->score8=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score8.bmp");
	s->score9=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score9.bmp");
	s->score0=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score0.bmp");
	s->scoreBoard=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/scoreboard.bmp");
	return s;
}

void updateScore(Score* s,int hitPoke){
	s->score++;
	if(hitPoke==1)
		s->score+=POKE_SCORE;
}

unsigned int returnScore(Score* s){
	return s->score;
}

void drawScore(Score* s){

	drawBitmap(s->scoreBoard,5,18,ALIGN_LEFT);
	char scoreStr[11];
	snprintf(scoreStr, sizeof(scoreStr), "%u", s->score);
	int x=18;
	int y=55;
	int i;
	for (i = 0; i<6 && scoreStr[i] != '\0'; i++){
		char number=scoreStr[i];
		switch (number) {
			case '1':
				drawBitmap(s->score1,x,y,ALIGN_LEFT);
				break;
			case '2':
				drawBitmap(s->score2,x,y,ALIGN_LEFT);
				break;
			case '3':
				drawBitmap(s->score3,x,y,ALIGN_LEFT);
				break;
			case '4':
				drawBitmap(s->score4,x,y,ALIGN_LEFT);
				break;
			case '5':
				drawBitmap(s->score5,x,y,ALIGN_LEFT);
				break;
			case '6':
				drawBitmap(s->score6,x,y,ALIGN_LEFT);
				break;
			case '7':
				drawBitmap(s->score7,x,y,ALIGN_LEFT);
				break;
			case '8':
				drawBitmap(s->score8,x,y,ALIGN_LEFT);
				break;
			case '9':
				drawBitmap(s->score9,x,y,ALIGN_LEFT);
				break;
			case '0':
				drawBitmap(s->score0,x,y,ALIGN_LEFT);
				break;
			default:
				break;
			}
		x+=10;
	}
}

void deleteScore(Score* s){
	if(s==NULL)
		return;
	deleteBitmap(s->score1);
	deleteBitmap(s->score2);
	deleteBitmap(s->score3);
	deleteBitmap(s->score4);
	deleteBitmap(s->score5);
	deleteBitmap(s->score6);
	deleteBitmap(s->score7);
	deleteBitmap(s->score8);
	deleteBitmap(s->score9);
	deleteBitmap(s->score0);

	free(s);
}
