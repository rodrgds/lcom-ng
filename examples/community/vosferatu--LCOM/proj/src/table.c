#include "table.h"
#include <math.h>


Table* newTable(unsigned int opponent){
	Table* table = (Table*) malloc(sizeof(Table));
	table->opponent = opponent;

	if (opponent){
		table->x = 325;
		table->y = 30;
	}
	else {
		table->x = 100;
		table->y = 30;
	}

	table->number0 = loadBitmap("0big.bmp");
	table->number1 = loadBitmap("1big.bmp");
	table->number2 = loadBitmap("2big.bmp");
	table->number3 = loadBitmap("3big.bmp");
	table->number4 = loadBitmap("4big.bmp");
	table->number5 = loadBitmap("5big.bmp");
	table->number6 = loadBitmap("6big.bmp");
	table->number7 = loadBitmap("7big.bmp");
	table->number8 = loadBitmap("8big.bmp");
	table->number9 = loadBitmap("9big.bmp");
	table->minus = loadBitmap("minus.bmp");

	table->w = table->number1->bitmapInfoHeader.width;
	table->h = table->number1->bitmapInfoHeader.height;

	table->playerScore = 0;
	table->PCScore = 0;

	return table;
}

void resetTable(Table* table){
	table->playerScore = 0;
	table->PCScore = 0;
	return;
}

void drawTable(Table* table){
	if(table->opponent == 0){
		if (table->playerScore >= 10)
			drawNumber(table, floor(table->playerScore/10.0), table->x - 40, table->y);
		drawNumber(table, (table->playerScore % 10), table->x, table->y);
		return;
	}

	if (table->playerScore >= 10)
		drawNumber(table, floor(table->playerScore/10.0), table->x - 40, table->y);
	drawNumber(table, (table->playerScore % 10), table->x, table->y);

	drawBitmapT(table->minus, table->x + 50, table->y + 20, ALIGN_LEFT);

	if (table->PCScore >= 10){
		drawNumber(table, floor(table->PCScore/10.0), table->x + 100, table->y);
		drawNumber(table, (table->PCScore % 10), table->x + 140, table->y);
		return;
	}

	drawNumber(table, (table->PCScore % 10), table->x + 100, table->y);
}

void drawNumber(Table* table, int number, int x, int y){

	switch(number){
		case 0:
			drawBitmapT(table->number0, x, y, ALIGN_LEFT);
			break;
		case 1:
			drawBitmapT(table->number1, x, y, ALIGN_LEFT);
			break;
		case 2:
			drawBitmapT(table->number2, x, y, ALIGN_LEFT);
			break;
		case 3:
			drawBitmapT(table->number3, x, y, ALIGN_LEFT);
			break;
		case 4:
			drawBitmapT(table->number4, x, y, ALIGN_LEFT);
			break;
		case 5:
			drawBitmapT(table->number5, x, y, ALIGN_LEFT);
			break;
		case 6:
			drawBitmapT(table->number6, x, y, ALIGN_LEFT);
			break;
		case 7:
			drawBitmapT(table->number7, x, y, ALIGN_LEFT);
			break;
		case 8:
			drawBitmapT(table->number8, x, y, ALIGN_LEFT);
			break;
		case 9:
			drawBitmapT(table->number9, x, y, ALIGN_LEFT);
			break;
		default:
			break;
	}

}

void deleteTable(Table* table){
	deleteBitmap(table->number0);
	deleteBitmap(table->number1);
	deleteBitmap(table->number2);
	deleteBitmap(table->number3);
	deleteBitmap(table->number4);
	deleteBitmap(table->number5);
	deleteBitmap(table->number6);
	deleteBitmap(table->number7);
	deleteBitmap(table->number8);
	deleteBitmap(table->number9);
	deleteBitmap(table->minus);


	free(table);
}
