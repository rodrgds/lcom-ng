#include <minix/drivers.h>
#include "FinalState.h"
#include "fonts.h"
#include "keyboard.h"
#include "interface.h"
#include "mouse.h"
#include "vbe.h"
#include "Bitmap.h"
#include "Button.h"
#include "i8254.h"
#include "rtc.h"


FinalState* newFinalState() {
    FinalState* state = (FinalState*) malloc(sizeof(FinalState));

    state->background=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/finalbackground3.bmp");
	state->HighScoresBackground=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/highscoresbackground2.bmp");
	state->f=loadFonts();
	state->score=0;
	readHighScores(state);
	state->flag=1;
	state->scoreTable=0;


    return state;
}



int updateFinalState(FinalState* state, unsigned long scancode, int score, int scoreAux){
	int exit=0;
	int flag=0;

	if(!flag){
		state->score=scoreAux;
		flag=1;
	}
	state->play=0;

	if(scancode==VAL_ESC){
		state->scoreTable=0;

		return 0;
	}


	if(scancode==code2){
		state->play=1;
		state->score=0;
		state->flag=1;
		state->scoreTable=0;
	}


	if(scancode==code3){
		 mouse_unsubscribe();
		return 1;
	}

	if(scancode==code1)
		state->scoreTable=1;

	if(state->scoreTable==1){
		if(state->flag==1){
			if(state->score > atoi(state->first) || state->score > atoi(state->second) || state->score > atoi(state->third)){
				newHighScore(state);
				readHighScores(state);
				state->flag = 0;
			}

		}
	}

	return 0;
}

void readHighScores(FinalState* state){

	state->file = fopen("/home/lcom/lcom1516-t2g12/proj/source/highscores.txt", "r");

		if(state->file ==NULL){
			printf("FAIL \n");
			return;
		}

		fgets (state->first, 30, state->file);
		fgets (state->second, 30, state->file);
		fgets (state->third, 30, state->file);

		//fgets (state->firstName, 30, state->file);
		//fgets (state->secondName, 30, state->file);
		//fgets (state->thirdName, 30, state->file);

		fgets (state->firstDate, 30, state->file);
		fgets (state->secondDate, 30, state->file);
		fgets (state->thirdDate, 30, state->file);

		fclose(state->file);
}

void printHighScores(FinalState* state){
	stringDisplay(state->f, state->firstDate, 112, 230, 25);
	//stringDisplay(state->f, state->firstName, 330, 230);
	stringDisplay(state->f, state->first, 580, 230,6);

	stringDisplay(state->f, state->secondDate, 112, 300,25);
	//stringDisplay(state->f, state->secondName, 330, 300);
	stringDisplay(state->f, state->second, 580, 300,6);

	stringDisplay(state->f, state->thirdDate, 112, 370,25);
	//stringDisplay(state->f, state->thirdName, 330, 370);
	stringDisplay(state->f, state->third, 580, 370,6);
}

void printScores(FinalState* state){
	printf("%s \n",state->firstDate);
	//printf("%s \n",state->firstName);
	printf("%s \n",state->first);

	printf("%s \n",state->secondDate);
	//printf("%s \n",state->secondName);
	printf("%s \n",state->second);

	printf("%s \n",state->thirdDate);
	//printf("%s \n",state->thirdName);
	printf("%s \n",state->third);
}

void newHighScore(FinalState* state) {
	rtc_subscribe();

	sprintf(state->newHighScore, "%d", state->score);
	char temp[25];
	display_date(temp);
	sprintf(state->newHighScoreDate, "%s", temp);

	state->file = fopen("/home/lcom/lcom1516-t2g12/proj/source/highscores.txt",	"w");

	if (state->score > atoi(state->first)) {
		fprintf(state->file, "%s\n", state->newHighScore);
		fprintf(state->file, "%s", state->first);
		fprintf(state->file, "%s", state->second);
		fprintf(state->file, "%s\n", state->newHighScoreDate);
		fprintf(state->file, "%s", state->firstDate);
		fprintf(state->file, "%s", state->secondDate);
	} else if (state->score > atoi(state->second)) {
		fprintf(state->file, "%s", state->first);
		fprintf(state->file, "%s\n", state->newHighScore);
		fprintf(state->file, "%s", state->second);
		fprintf(state->file, "%s", state->firstDate);
		fprintf(state->file, "%s\n", state->newHighScoreDate);
		fprintf(state->file, "%s", state->secondDate);
	} else if (state->score > atoi(state->third)) {
		fprintf(state->file, "%s", state->first);
		fprintf(state->file, "%s", state->second);
		fprintf(state->file, "%s\n", state->newHighScore);
		fprintf(state->file, "%s", state->firstDate);
		fprintf(state->file, "%s", state->secondDate);
		fprintf(state->file, "%s", state->newHighScoreDate);
	}

	fclose(state->file);
	state->flag = 0;
}





int verifyFinalStateChange(FinalState* state){
	if(state->play)
		return 1;
	else
		return 0;
}

void drawFinalMenu(FinalState* state){
	if(!state->scoreTable){
		drawBitmap(state->background,0,0,ALIGN_LEFT);
	}
	else{
		drawBitmap(state->HighScoresBackground,0,0,ALIGN_LEFT);
		printHighScores(state);
	}


}

void deleteFinalState(FinalState* state) {
    deleteBitmap(state->background);
    deleteBitmap(state->HighScoresBackground);
    deleteFonts(state->f);

    free(state);
}
