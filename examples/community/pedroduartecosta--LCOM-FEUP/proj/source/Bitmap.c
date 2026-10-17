#include "Bitmap.h"

#include "interface.h"

#define FIRST_BYTE_GREEN_COLOR  0xFFFFFFE0 // necessário a extensão devido ao sinal negativo
#define SECOND_BYTE_GREEN_COLOR 0x07

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
	int height = bmp->bitmapInfoHeader.height;

	if (alignment == ALIGN_CENTER)
		x -= width / 2;
	else if (alignment == ALIGN_RIGHT)
		x -= width;

	int xCorrection = 0;
	if (x < 0) {
		xCorrection = -x;
		x = 0;
	}
	char* bufferStartPos;
	char* imgStartPos;

	int i, j;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;
		if (pos < 0 || pos >= getVerResolution()) {
			pos -= getVerResolution();
		}
		for (j = 0; j < width; j++) {
			bufferStartPos = getGraphicsBufferTmp() + (x * getBytesPerPixel())
					+ (pos * getHorResolution() * getBytesPerPixel())
					+ (j * getBytesPerPixel());
			imgStartPos = bmp->bitmapData + xCorrection * getBytesPerPixel()
					+ j * getBytesPerPixel() + i * width * getBytesPerPixel();
			if (*imgStartPos != FIRST_BYTE_GREEN_COLOR
					&& *(imgStartPos + 1) != SECOND_BYTE_GREEN_COLOR)
				memcpy(bufferStartPos, imgStartPos, getBytesPerPixel());
		}
	}
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;

    free(bmp->bitmapData);
    free(bmp);
}
