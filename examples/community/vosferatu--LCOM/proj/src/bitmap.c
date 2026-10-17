/*
 * Bitmap Loader source file
 * @ Author: Henrique Ferrolho/Joao Mendes -- changes for use in the program DUELNIX
 * @ Source: http://difusal.blogspot.pt/2014/09/minixtutorial-8-loading-bmp-images.html
 * code approved: https://moodle.up.pt/mod/forum/discuss.php?d=4628
 */


#include "bitmap.h"

#include "stdio.h"
#include "utils.h"
#include "graphics.h"

#define READ_GREEN	0xE007

Bitmap* loadBitmap(const char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

    char* file = (char*) malloc(strlen(path_res) + strlen(filename) + 1);
    strcpy(file, path_res); strcat(file, filename); // file = path + name

    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(file, "rb");
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
    BitmapInfoHeader bmpInfoHeader;
    fread(&bmpInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
    unsigned char* bitmapImage = (unsigned char*) malloc(bmpInfoHeader.imageSize);
    //unsigned char* bitmapImage = (unsigned char*) malloc(bmpInfoHeader.width*bmpInfoHeader.height*sizeof(unsigned short));

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data
    fread(bitmapImage, bmpInfoHeader.imageSize, 1, filePtr);

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bmpInfoHeader;

    return bmp;
}

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL){
    	printf("was null 1\n");
    	 return;
    }

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;
    int hres = get_hres();
    int vres = get_vres();
    int bytesPerPixel = get_bitsPerPixel() / 8;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > hres || y + height < 0
            || y > vres)
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > hres)
            drawWidth = hres;
    } else if (x + drawWidth >= hres) {
        drawWidth = hres - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= vres)
            continue;

        bufferStartPos = (char*) getGraphicsBuffer();
        bufferStartPos += x * 2 + pos * hres * 2;

        imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;

        memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
    }
}

void drawBitmapT(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;
    int hres = get_hres();
    int vres = get_vres();
    int bytesPerPixel = get_bitsPerPixel() / 8;

    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > hres || y + height < 0
            || y > vres)
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > hres)
            drawWidth = hres;
    } else if (x + drawWidth >= hres) {
        drawWidth = hres - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i, j, k;
    unsigned short color = 0;

    for (i = 0; i < height; i++){
    	int pos = y + height - 1 - i;

    	if (pos < 0 || pos >= vres)
    		continue;

    	for (j = 0; j < drawWidth; j++){
    		bufferStartPos = getGraphicsBuffer();
    		bufferStartPos += (pos * hres + x + j) * bytesPerPixel;

    		imgStartPos = bmp->bitmapData + (xCorrection + i * width + j) * bytesPerPixel;

    		color = 0;
    		for (k = 0; k < bytesPerPixel; k++){
    			color <<= 8;
    			color |= *imgStartPos;
    			imgStartPos++;
    		}
    		imgStartPos -= 2;

    		if (color != READ_GREEN){
    			for (k = 0; k < bytesPerPixel - 1; k++){
    				*bufferStartPos = *imgStartPos;
    				imgStartPos++;
    				bufferStartPos++;
    			}
    			*bufferStartPos = *imgStartPos;
    		}
    	}
    }
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}
