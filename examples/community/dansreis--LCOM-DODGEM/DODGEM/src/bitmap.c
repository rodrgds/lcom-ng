
#include "bitmap.h"
#include "stdio.h"
#include "graphics.h"
#include "utilities.h"
#include "string.h"
#include "DODGEM.h"




Bitmap* loadBitmap(const char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL)
        return NULL;

    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, 2, 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    int rd;
    do {
        if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
            break;
        if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
            break;
    } while (0);

    if (rd = !1) {
        fprintf(stderr, "Error reading file\n");
        exit(-1);
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
    unsigned char* bitmapImage = (unsigned char*) malloc(
            bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;

    return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > MODE1024_H_RES || y + height < 0
            || y > MODE1024_V_RES)
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > MODE1024_H_RES)
            drawWidth = MODE1024_H_RES;
    } else if (x + drawWidth >= MODE1024_H_RES) {
        drawWidth = MODE1024_H_RES - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= MODE1024_V_RES)
        	continue;

        bufferStartPos = (char*)getVideoBuffer();
        bufferStartPos += x * 2 + pos * MODE1024_H_RES * 2;

        imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

        memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
    }
}

int number_draw_width(int number)
{
	if(number == 10)
		return 20;
	else
		return 39;
}

void drawBitmapNumber(Bitmap* bmp, int x, int y,int number, Alignment alignment) {


	if (bmp == NULL)
		return;


	int width = bmp->bitmapInfoHeader.width;
	int height = bmp->bitmapInfoHeader.height;


	int drawWidth = number_draw_width(number);
	int xCorrection=0;

	xCorrection+=(number)*40;



	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > MODE1024_H_RES || y + height < 0
			|| y > MODE1024_V_RES)
		return;


	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > MODE1024_H_RES)
			drawWidth = MODE1024_H_RES;
	} else if (x + drawWidth >= MODE1024_H_RES) {
		drawWidth = MODE1024_H_RES - x;
	}

	char* bufferStartPos;
	char* imgStartPos;


	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0|| pos >= MODE1024_V_RES)
			continue;

		bufferStartPos = (char*)getVideoBuffer();
		bufferStartPos += x * 2 + pos * MODE1024_H_RES * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

		memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}

void drawWhiteScore(int x_inicial,int y_inicial,int segundos,int centesimas)
{
	/*drawBitmapNumber(game->Numbers,800, 690,segundos, ALIGN_LEFT);*/



	unsigned char string[50];

	sprintf(string,"%d",segundos);

	int i = 0;
	for(i; i < strlen(string);i++)
	{
		drawBitmapNumber(game->NumbersWhite,x_inicial, y_inicial,string[i]-'0', ALIGN_LEFT);
		x_inicial+=number_draw_width(string[i]-'0');
	}

	if (centesimas == -1)
		return;


	drawBitmapNumber(game->NumbersWhite,x_inicial,y_inicial ,10, ALIGN_LEFT);
	x_inicial+=number_draw_width(10);


	i = 0;
	sprintf(string,"%d",centesimas);
	if(centesimas <10)
	{
		drawBitmapNumber(game->NumbersWhite,x_inicial,y_inicial ,0, ALIGN_LEFT);
		x_inicial+=number_draw_width(0);
	}

	for(i; i < strlen(string);i++)
	{
		drawBitmapNumber(game->NumbersWhite,x_inicial, y_inicial,string[i]-'0', ALIGN_LEFT);
		x_inicial+=number_draw_width(string[i]-'0');
	}

}

void drawBlackScore(int x_inicial,int y_inicial,int segundos,int centesimas)
{
	/*drawBitmapNumber(game->Numbers,800, 690,segundos, ALIGN_LEFT);*/



	unsigned char string[50];

	sprintf(string,"%d",segundos);

	int i = 0;
	for(i; i < strlen(string);i++)
	{
		drawBitmapNumber(game->NumbersBlack,x_inicial, y_inicial,string[i]-'0', ALIGN_LEFT);
		x_inicial+=number_draw_width(string[i]-'0');
	}

	if (centesimas == -1)
		return;


	drawBitmapNumber(game->NumbersBlack,x_inicial,y_inicial ,10, ALIGN_LEFT);
	x_inicial+=number_draw_width(10);


	i = 0;
	sprintf(string,"%d",centesimas);
	if(centesimas <10)
	{
		drawBitmapNumber(game->NumbersBlack,x_inicial,y_inicial ,0, ALIGN_LEFT);
		x_inicial+=number_draw_width(0);
	}

	for(i; i < strlen(string);i++)
	{
		drawBitmapNumber(game->NumbersBlack,x_inicial, y_inicial,string[i]-'0', ALIGN_LEFT);
		x_inicial+=number_draw_width(string[i]-'0');
	}

}

void drawPart(Bitmap* bmp, int x, int y, int x_inicial, int y_inicial, int x_final, int y_final, Alignment alignment){


	if (bmp == NULL)
		return;


	int width = bmp->bitmapInfoHeader.width;
	int height = bmp->bitmapInfoHeader.height;


	int drawWidth = x_final - x_inicial;
	int xCorrection = 0;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > MODE1024_H_RES || y + height < 0
			|| y > MODE1024_V_RES)
		return;


	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > MODE1024_H_RES)
			drawWidth = MODE1024_H_RES;
	} else if (x + drawWidth >= MODE1024_H_RES) {
		drawWidth = MODE1024_H_RES - x;
	}

	char* bufferStartPos;
	char* imgStartPos;


	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0|| pos >= MODE1024_V_RES)
			continue;

		bufferStartPos = (char*)getVideoBuffer();
		bufferStartPos += x * 2 + pos * MODE1024_H_RES * 2;

		imgStartPos = bmp->bitmapData + x_inicial*2 + y_inicial*width + xCorrection * 2 + i * width * 2;

		memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}

void drawLosingText(int segundos, int centesimas){
    drawBitmap(game->ScoreBackground, 450, 250, ALIGN_LEFT);
    drawWhiteScore(580,350,segundos,centesimas);
    memcpy(getVideoMem(), getVideoBuffer(), MODE1024_H_RES * MODE1024_V_RES * 2);
}

int drawPlayerName(char* name,int x,int y) // desenha o nome do player no ecra
{
	int i = 0;
	for(i;i<12;i++)
	{
		if (name[i] == '\0' || name[i] == '\n' || name[i] == '*')
			return x;
		if(name[i] == '.')
			continue;
		else if(name[i]== ' ')
			x+=20;
		else if(name[i] == '0'||name[i] == '1'||name[i] == '2'||name[i] == '3'||name[i] == '4'||
				name[i] == '5'||name[i] == '6'||name[i] == '7'||name[i] == '8'||name[i] == '9')
		{
			int n = char_to_int(name[i]);
			drawBitmapNumber(game->NumbersBlack,x,y,n,ALIGN_LEFT);
			x+=40;
		}else if(name[i]=='i')
		{
			drawBitmapLetter(game->alphabet,x,y,name[i],ALIGN_LEFT);
			x+= 20;
		}
		else if(name[i]=='j')
		{
			drawBitmapLetter(game->alphabet,x,y,name[i],ALIGN_LEFT);
			x+= 30;
		}
		else if(name[i] == 'w')
		{
			drawBitmapLetter(game->alphabet,x,y,name[i],ALIGN_LEFT);
			x+= 50;
		}
		else
		{
			drawBitmapLetter(game->alphabet,x,y,name[i],ALIGN_LEFT);
			x+= 40;
		}
	}
	return x;
}

void drawBitmapLetter(Bitmap* bmp, int x, int y,char letter, Alignment alignment)
{


	if (bmp == NULL)
		return;


	int width = bmp->bitmapInfoHeader.width;
	int height = bmp->bitmapInfoHeader.height;


	int drawWidth=0;

	int xCorrection= getLetterPos(letter);
	if (xCorrection == -1){
		return;
	}

	if(letter == 'i')
		drawWidth = 20;
	else if(letter == 'j')
		drawWidth = 30;
	else if(letter == 'w')
		drawWidth = 50;
	else
		drawWidth = 40;


	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	if (x + width < 0 || x > MODE1024_H_RES || y + height < 0
			|| y > MODE1024_V_RES)
		return;


	if (x < 0) {
		xCorrection = -x;
		drawWidth -= xCorrection;
		x = 0;

		if (drawWidth > MODE1024_H_RES)
			drawWidth = MODE1024_H_RES;
	} else if (x + drawWidth >= MODE1024_H_RES) {
		drawWidth = MODE1024_H_RES - x;
	}

	char* bufferStartPos;
	char* imgStartPos;


	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0|| pos >= MODE1024_V_RES)
			continue;

		bufferStartPos = (char*)getVideoBuffer();
		bufferStartPos += x * 2 + pos * MODE1024_H_RES * 2;

		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

		memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}

void drawHighscores(char* name,int segundos,int centesimas,int x,int y)
{
	drawPlayerName(name,x,y);
	drawBlackScore(780,y,segundos,centesimas);
	return;
}

void drawData(int day, int month, int year, int hours, int min, int sec, int x, int y){
	int digito;

	digito = day/10;
	drawBitmapNumber(game->NumbersBlack, x, y ,digito, ALIGN_LEFT);
	x+=40;
	digito = day%10;
	drawBitmapNumber(game->NumbersBlack, x, y ,digito, ALIGN_LEFT);
	x+=40;

	drawPart(game->Separadores, x, y, 0, 0, 20, 50, ALIGN_LEFT);
	x+=20;

	digito = month/10;
	drawBitmapNumber(game->NumbersBlack, x, y ,digito, ALIGN_LEFT);
	x+=40;
	digito = month%10;
	drawBitmapNumber(game->NumbersBlack, x, y ,digito, ALIGN_LEFT);
	x+=40;

	drawPart(game->Separadores, x, y, 0, 0, 20, 50, ALIGN_LEFT);
	x+=20;

	drawBitmapNumber(game->NumbersBlack, x, y ,2, ALIGN_LEFT);
	x+=40;
	drawBitmapNumber(game->NumbersBlack, x, y ,0, ALIGN_LEFT);
	x+=40;
	digito = year/10;
	drawBitmapNumber(game->NumbersBlack, x, y ,digito, ALIGN_LEFT);
	x+=40;
	digito = year%10;
	drawBitmapNumber(game->NumbersBlack, x, y ,digito, ALIGN_LEFT);
	x+=80;

	digito = hours/10;
	drawBitmapNumber(game->NumbersBlack, x, y,digito, ALIGN_LEFT);
	x+=40;
	digito = hours%10;
	drawBitmapNumber(game->NumbersBlack, x, y,digito, ALIGN_LEFT);
	x+=40;

	drawPart(game->Separadores, x, y, 20, 0, 30, 50, ALIGN_LEFT);
	x+=10;

	digito = min/10;
	drawBitmapNumber(game->NumbersBlack, x, y,digito, ALIGN_LEFT);
	x+=40;
	digito = min%10;
	drawBitmapNumber(game->NumbersBlack, x, y,digito, ALIGN_LEFT);
	x+=40;

	drawPart(game->Separadores, x, y, 20, 0, 30, 50, ALIGN_LEFT);
	x+=10;

	digito = sec/10;
	drawBitmapNumber(game->NumbersBlack, x, y,digito, ALIGN_LEFT);
	x+=40;
	digito = sec%10;
	drawBitmapNumber(game->NumbersBlack, x, y,digito, ALIGN_LEFT);
	x+=40;
}







