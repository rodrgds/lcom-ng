#include <lcom/lcf.h>
#include "Bitmap.h"
#include "libs/video.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

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

    if (rd != 1) {
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

void drawBitmap(Bitmap* bmp, int x, int y) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;


    if (x + width < 0 || x > getHorResolution() || y + height < 0
            || y > getVerResolution())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHorResolution())
            drawWidth = getHorResolution();
    } else if (x + drawWidth >= getHorResolution()) {
        drawWidth = getHorResolution() - x;
    }

    u_char* bufferStartPos;
    unsigned char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= getVerResolution())
            continue;

        bufferStartPos = getGraphicsBuffer();
        bufferStartPos += x * 2 + pos * getHorResolution() * 2;

        if (width % 2 == 0)
          imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
        else
          imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2 + i * 2;

        for (int i = 0; i < drawWidth; i++)
          if (*(imgStartPos+i*2) != 0 || *(imgStartPos+i*2+1) != 0 ){
            *(bufferStartPos+i*2) = *(imgStartPos+i*2);
            *(bufferStartPos+i*2+1) = *(imgStartPos+i*2+1);
          }
    }
}

void drawBitmapWithRedFilter(Bitmap* bmp, int x, int y, bool applyFilter) {
    if (bmp == NULL)
        return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;


    if (x + width < 0 || x > getHorResolution() || y + height < 0
            || y > getVerResolution())
        return;

    int xCorrection = 0;
    if (x < 0) {
        xCorrection = -x;
        drawWidth -= xCorrection;
        x = 0;

        if (drawWidth > getHorResolution())
            drawWidth = getHorResolution();
    } else if (x + drawWidth >= getHorResolution()) {
        drawWidth = getHorResolution() - x;
    }

    u_char* bufferStartPos;
    unsigned char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
        int pos = y + height - 1 - i;

        if (pos < 0 || pos >= getVerResolution())
            continue;

        bufferStartPos = getGraphicsBuffer();
        bufferStartPos += x * 2 + pos * getHorResolution() * 2;

        if (width % 2 == 0)
          imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2;
        else
          imgStartPos = bmp->bitmapData + xCorrection * 2 + i * width * 2 + i * 2;

        for (int i = 0; i < drawWidth; i++)
          if (*(imgStartPos+i*2) != 0 || *(imgStartPos+i*2+1) != 0 ){

            if (applyFilter){
                int color = *(imgStartPos+i*2);
                color += *(imgStartPos+i*2+1) << 8;
                int blue = color & 0x1F;
                int green = (color >> 5) & 0x3F;
                int red = (color >> 11) & 0x1F;
                green = green >> 1;
                color = ((blue + red + green)/3) << 11;
                *(bufferStartPos+i*2) = (u_char)color & 0xFF;
                *(bufferStartPos+i*2+1) = (u_char)(color >> 8) & 0xFF;
            } else {
                *(bufferStartPos+i*2) = *(imgStartPos+i*2);
                *(bufferStartPos+i*2+1) = *(imgStartPos+i*2+1);
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
