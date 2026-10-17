#include "image.h"
#include "stdio.h"
#include "video_gr.h"


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

void drawBitmap(Bitmap* bmp, void * buffer, unsigned short x_init, unsigned short y_init, unsigned short x_size, unsigned short y_size) {

	if (bmp->bitmapInfoHeader.bits != 16) return;
	unsigned short * ptr_start = (unsigned short *) buffer;
	unsigned short * image_ptr = (unsigned short *) bmp->bitmapData;
	unsigned short i, j;
	unsigned short x, y;
	unsigned int index1, index2;
	unsigned long max_index1 = h_res*v_res - 1;
	unsigned long max_index2 = bmp->bitmapInfoHeader.width * bmp->bitmapInfoHeader.height - 1;
	for(i=0; i < y_size; i++){
		for( j=0; j < x_size; j++){
			index1 = ((y_init+i) * h_res + x_init + j);
			x = j * bmp->bitmapInfoHeader.width / x_size;
			y = bmp->bitmapInfoHeader.height - i * bmp->bitmapInfoHeader.height / y_size;
			index2 = ( y * bmp->bitmapInfoHeader.width + x);
			if (index1 > 0 &&  index2 > 0 && index1 < max_index1 &&  index2 < max_index2) ptr_start[index1] = image_ptr[index2];
		}
	}
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}
