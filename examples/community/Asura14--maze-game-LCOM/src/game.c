#include "video_gr.h"
#include "game.h"
#include "time.h"
#include "stdlib.h"
#include "math.h"
#include "keyboard.h"

#include "timer.h"


#define RIGTH 1
#define LEFT 2
#define UP 0
#define DOWN 3

struct character{
	int x,y;
	int limRight, limLeft, limTop, limBottom;
} c;


void show(char board[WIDTH][LENGTH]){
	unsigned int i, j ;
	unsigned long x = STARTBOARDX, y =STARTBOARDY;
	for(i= 0; i < WIDTH; i++){
		for(j=0; j<LENGTH; j++){
			printf("%c\n", board[i][j]);
			if(board[i][j] == ' '){
				drawCube(x, CUBE_SIZE, y, CUBE_SIZE, 1, 3);
				x = x + CUBE_SIZE;
			}
			else if(board[i][j] == 'r') {
				drawCube(x,CUBE_SIZE, y, CUBE_SIZE, 18, 2);
				x = x + CUBE_SIZE;
			}
			else if(board[i][j] == 'P'){
				drawCube(x, CUBE_SIZE, y, CUBE_SIZE, 31,2);
				x = x + CUBE_SIZE;
			}
			else if(board[i][j] == 'Q'){
				drawCube(x, CUBE_SIZE, y, CUBE_SIZE, 6, 2);
				x = x + CUBE_SIZE;
			}

		}
		x = STARTBOARDX;
		y = y + CUBE_SIZE;
	}
}

/*
void findPos(int &x, int &y, char board[WIDTH][LENGTH]){
	int i , j ;
	for(i = 0; i < WIDTH; i++){
		for(j = 0; j< LENGTH; j++){
			if(board[i][j] == 'r'){
				x = i; y = j;
				return;
			}
		}
	}
}
*/

char board1[WIDTH][LENGTH] = {
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P','P','P', 'P', 'P'},
		{'P', ' ',' ', ' ', ' ', ' ',' ', 'P',' ', ' ', ' ', ' ', ' ', ' ', ' ','P', ' ', ' ',' ', ' ', ' ',' ', 'P', ' ','P'},
		{'P', 'P','P', 'P', ' ', 'P',' ', 'P',' ', 'P', 'P', 'P', ' ', 'P', ' ','P', ' ', ' ',' ', ' ', 'P',' ', 'P', ' ','P'},
		{'P', ' ',' ', 'P', ' ', 'P',' ', 'P',' ', ' ', ' ', 'P', ' ', 'P', ' ','P', ' ', ' ',' ', ' ', 'P',' ', 'P', ' ','P'},
		{'P', ' ',' ', 'P', ' ', 'P',' ', 'P',' ', 'P', ' ', 'P', ' ', 'P', ' ','P', ' ', ' ',' ', ' ', 'P',' ', 'P', ' ','P'},
		{'P', 'P','P', 'P', ' ', 'P',' ', 'P',' ', 'P', ' ', 'P', ' ', 'P', ' ','P', 'P', 'P','P', 'P', 'P',' ', 'P', ' ','P'},
		{'P', ' ',' ', ' ', ' ', 'P',' ', ' ',' ', 'P', ' ', 'P', ' ', 'P', ' ',' ', ' ', ' ',' ', ' ', ' ',' ', ' ', ' ','P'},
		{'P', 'P',' ', 'P', ' ', 'P',' ', 'P','P', 'P', ' ', 'P', ' ', 'P', ' ','P', 'P', 'P','P', 'P', 'P',' ', 'P', ' ','P'},
		{'P', 'P',' ', 'P', ' ', 'P',' ', ' ',' ', ' ', ' ', 'P', ' ', ' ', ' ','P', ' ', ' ',' ', ' ', ' ',' ', 'P', ' ','P'},
		{'P', ' ',' ', 'P', ' ', 'P',' ', 'P','P', 'P', ' ', 'P', ' ', 'P', ' ','P', ' ', 'P','P', 'P', 'P',' ', 'P', ' ','P'},
		{'P', ' ','P', 'P', ' ', 'P',' ', 'P',' ', 'P', ' ', ' ', ' ', 'P', ' ','P', ' ', ' ',' ', ' ', ' ',' ', 'P', ' ','P'},
		{'P', ' ','P', 'P', ' ', 'P',' ', 'P',' ', 'P', ' ', 'P', 'P', 'P', ' ','P', ' ', 'P','P', 'P', 'P','P', 'P', ' ','P'},
		{'P', ' ','P', 'P', ' ', 'P',' ', 'P',' ', 'P', ' ', 'P', ' ', ' ', ' ','P', ' ', 'P','P', ' ', ' ',' ', 'P', 'P','P'},
		{'P', ' ',' ', ' ', ' ', ' ',' ', 'P',' ', ' ', ' ', 'P', ' ', 'P', 'P','P', ' ', ' ',' ', ' ', 'P',' ', ' ', 'Q','P'},
		{'P', ' ','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', ' ','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},
		{'P', 'P','P', 'P', 'P', 'P','P', 'P','P', 'P', 'P', 'P', 'P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P', 'P', 'P','P'},

};


int checkMove(char board[WIDTH][LENGTH], int direction){

	switch(direction){
	case DOWN:
		if(board[c.x + 1][c.y] == 'P'){
			return 0;
		}
		else if(board[c.x +1 ][c.y] == 'Q')
			return 2;
		break;
	case LEFT:
		if(board[c.x][c.y - 1] == 'P')
			return 0;
		else if(board[c.x][c.y - 1] == 'Q')
			return 2;
		break;
	case RIGTH:
		if(board[c.x][c.y + 1] == 'P'){
			return 0;
		}
		else if(board[c.x][c.y + 1] == 'Q')
			return 2;
		break;
	case UP:
		if(board[c.x -1 ][c.y] == 'P'){
			return 0;
		}
		else if(board[c.x - 1][c.y] == 'Q')
			return 2;
		break;
	default: break;
	}

	return 1;
}



void game(){

	int endGame = 0;
	c.x = 1;
	c.y = 1;
	while(endGame==0){
		show(board1);
		int direction = -1;
		direction = interrupt();
		printf("direction %x\n", direction);
		if (direction == -2) {
			endGame = 1;
		}
		if(direction != -1){

			if(checkMove(board1, direction) == 1){
				switch(direction){
				case 0:
					board1[c.x - 1][c.y] = 'r';
					board1[c.x][c.y] = ' ';
					c.x--;
					break;
				case 1:
					board1[c.x][c.y + 1] = 'r';
					board1[c.x][c.y] = ' ';
					c.y++;
					break;
				case 2:
					board1[c.x][c.y - 1] = 'r';
					board1[c.x][c.y] = ' ';
					c.y--;
					break;
				case 3:
					board1[c.x + 1][c.y] = 'r';
					board1[c.x][c.y] = ' ';
					c.x++;
					break;
				default:
					break;
				}
			}
			else{
				if(checkMove(board1, direction) == 2){
					endGame=1;
				}
			}
		}
	}
	printf("END GAME!!!\n");
}
