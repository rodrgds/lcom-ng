#include "Bitmap.h"

#include "stdio.h"
#include "video_gr.h"
#include <minix/syslib.h>

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
int drawBitmap_byLine(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return 1;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;
    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > get_h_res() || y + height < 0
            || y > get_v_res())
        return 2;

    int xCorrection = 0;
    if (x < 0) {
    	xCorrection = -x;
    	drawWidth -= xCorrection;
    	x = 0;

    	if (drawWidth > get_h_res())
    		drawWidth = get_h_res();
    } else if (x + drawWidth >= get_h_res()) {
    	drawWidth = get_h_res() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;
    int i;
    int a;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;
        if (pos < 0 || pos >= get_v_res())
            continue;

        bufferStartPos = vg_adress();
        bufferStartPos += x * 2 + pos * get_h_res() * 2;
        imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
        memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
    }

    return 0;
}

int drawBitmap_byPixel(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return 1;
    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;
    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > get_h_res() || y + height < 0
            || y > get_v_res())
        return 2;

    int xCorrection = 0;
    if (x < 0) {
    	xCorrection = -x;
    	drawWidth -= xCorrection;
    	x = 0;

    	if (drawWidth > get_h_res())
    		drawWidth = get_h_res();
    } else if (x + drawWidth >= get_h_res()) {
    	drawWidth = get_h_res() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;
    int i;
    int a;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;
    	if (pos < 0 || pos >= get_v_res())
    		continue;

    	for(a = 0; a < width;a++){
    		bufferStartPos = vg_adress();
    		bufferStartPos += x * 2 + pos * get_h_res() * 2 + (a*2);
    		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2 + (a*2);
    		if(*imgStartPos == (int) -32)
    			if(*(imgStartPos+1) == (int) 7)
    				continue;
    			else
    				memcpy(bufferStartPos, imgStartPos,2);
    		else
    			memcpy(bufferStartPos, imgStartPos,2);


    	}
    }

    return 0;
}

int drawBitmap_onBoard(Bitmap* bmp, int x, int y, Alignment alignment,unsigned char**double_buffer) {
    if (bmp == NULL)
        return 1;
    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;
    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > get_h_res() || y + height < 0
            || y > get_v_res())
        return 2;

    int xCorrection = 0;
    if (x < 0) {
    	xCorrection = -x;
    	drawWidth -= xCorrection;
    	x = 0;

    	if (drawWidth > get_h_res())
    		drawWidth = get_h_res();
    } else if (x + drawWidth >= get_h_res()) {
    	drawWidth = get_h_res() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;
    int i;
    int a;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;
    	if (pos < 0 || pos >= get_v_res())
    		continue;

    	for(a = 0; a < width;a++){
    		bufferStartPos = *double_buffer;
    		bufferStartPos += x * 2 + pos * get_h_res() * 2 + (a*2);
    		imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2 + (a*2);
    		if(*imgStartPos == (int) -32)
    			if(*(imgStartPos+1) == (int) 7)
    				continue;
    			else
    				memcpy(bufferStartPos, imgStartPos,2);
    		else
    			memcpy(bufferStartPos, imgStartPos,2);


    	}
    }

    return 0;
}

int erase_square(Bitmap* bmp, int x, int y, Alignment alignment,unsigned char**double_buffer,unsigned char*background) {
    if (bmp == NULL)
        return 1;
    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;
    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;

    if (x + width < 0 || x > get_h_res() || y + height < 0
            || y > get_v_res())
        return 2;

    int xCorrection = 0;
    if (x < 0) {
    	xCorrection = -x;
    	drawWidth -= xCorrection;
    	x = 0;

    	if (drawWidth > get_h_res())
    		drawWidth = get_h_res();
    } else if (x + drawWidth >= get_h_res()) {
    	drawWidth = get_h_res() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;
    int i;
    int a;
    for (i = 0; i < height; i++) {
    	int pos = y + height - 1 - i;
    	if (pos < 0 || pos >= get_v_res())
    		continue;

    	for(a = 0; a < width;a++){
    		bufferStartPos = *double_buffer;
    		bufferStartPos += x * 2 + pos * get_h_res() * 2 + (a*2);
    		imgStartPos = background + x * 2 + pos * get_h_res() * 2 + (a*2);
    		if(*imgStartPos == (int) -32)
    			if(*(imgStartPos+1) == (int) 7)
    				continue;
    			else
    				memcpy(bufferStartPos, imgStartPos,2);
    		else
    			memcpy(bufferStartPos, imgStartPos,2);


    	}
    }

    return 0;
}



unsigned char *double_buffering(){

	char* my_double_buffer = (unsigned char *) malloc(get_h_res()*get_v_res()*2);
	char* bufferStartPos = vg_adress();
	memcpy(my_double_buffer, bufferStartPos,get_h_res()*get_v_res()*2);
	return my_double_buffer;

}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}
