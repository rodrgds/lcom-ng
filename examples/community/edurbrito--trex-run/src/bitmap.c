#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "vdh.h"
#include "bitmap.h"

Bitmap* loadBitmap(const char* filename){

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
        printf("Error reading file\n");
        exit(-1);
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    // allocate enough memory for the bitmap image data
    unsigned char* bitmapImage = (unsigned char*) malloc(bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // read in the bitmap image data (determina o tipo) - aula teorica
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

void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment, Buffer buffer){

    if (bmp == NULL){
      alignment=0;
      return;
    }

    int width       = bmp->bitmapInfoHeader.width;
    int height      = bmp->bitmapInfoHeader.height;
    int img_width   = width;
    int xCorrection = 0;
    unsigned char* img;

    if (alignment == ALIGN_CENTER)
  		x -= width / 2;

  	else if (alignment == ALIGN_RIGHT)
  		x -= width;

    if (x + width < 0 || x > getHres() || y + height < 0 || y > getVres())
        return;

    if (x < 0) {
        xCorrection = -x;
        img_width = img_width + x;
        x = 0;
    }

    else if (x + img_width >= getHres()) {
        img_width = getHres() - x;
    }

    uint8_t * buffer_pos1;
    char * buffer_pos2;
    unsigned char * buffer_pos3;

    for (int i = 0; i < height; i++) {
        int pos = y + height - (i+1);
        if (pos < 0 || pos >= getVres())
            continue;

        switch (buffer)
        {
        case REAL_BUFFER:

            buffer_pos1 = get_vid_mem();
            buffer_pos1 += (x * 2) + (pos * getHres() * 2);
            img = bmp->bitmapData + (xCorrection * 2) + (i * width * 2);
            for(int j=0; j<img_width *2 ; j+=2){
                if(img[j] != pink_first_byte && img[j+1] != pink_second_byte){
                    buffer_pos1[j] = img[j];
                    buffer_pos1[j+1] = img[j+1];
                }
            }

            break;
        case SECOND_BUFFER:

            buffer_pos2 = get_double_buffer();
            buffer_pos2 += (x * 2) + (pos * getHres() * 2);
            img = bmp->bitmapData + (xCorrection * 2) + (i * width * 2);
            for(int j=0; j<img_width *2 ; j+=2){
                if(img[j] != pink_first_byte && img[j+1] != pink_second_byte){
                    buffer_pos2[j] = img[j];
                    buffer_pos2[j+1] = img[j+1];
                }
            }

            break;
        case THIRD_BUFFER:

            buffer_pos3 = get_rd_buffer();
            buffer_pos3 += (x * 2) + (pos * getHres() * 2);
            img = bmp->bitmapData + (xCorrection * 2) + (i * width * 2);
            for(int j=0; j<img_width *2 ; j+=2){
                buffer_pos3[j] = img[j];
                buffer_pos3[j+1] = img[j+1];
            }

            break;
        
        default:
            break;
        }
    }
    return;
}

int drawBitmap_collision(Bitmap* bmp, int x, int y, Alignment alignment){

    if (bmp == NULL){
      alignment=0;
      return -1;
    }

    int width       = bmp->bitmapInfoHeader.width;
    int height      = bmp->bitmapInfoHeader.height;
    int img_width   = width;
    int xCorrection = 0;
    unsigned char* img;

    if (x + width < 0 || x > getHres() || y + height < 0 || y > getVres())
        return -1;

    if (x < 0) {
        xCorrection = -x;
        img_width = img_width + x;
        x = 0;
    }

    else if (x + img_width >= getHres()) {
        img_width = getHres() - x;
    }

    for (int i = 0; i < height; i++) {
        int pos = y + height - (i+1);

        if (pos < 0 || pos >= getVres())
            continue;

        unsigned char *buffer_pos = get_rd_buffer();
        buffer_pos += (x * 2) + (pos * getHres() * 2);
        img = bmp->bitmapData + (xCorrection * 2) + (i * width * 2);

        for(int j=0; j<img_width *2 ; j+=2){
            if(img[j] != pink_first_byte && img[j+1] != pink_second_byte && buffer_pos[j] != pink_first_byte && buffer_pos[j+1] != pink_second_byte){
                return 1;
            }
        }
    }
    return 0;
}

void deleteBitmap(Bitmap* bmp){
    if (bmp == NULL)
        return;
    free(bmp->bitmapData);
    free(bmp);
}
