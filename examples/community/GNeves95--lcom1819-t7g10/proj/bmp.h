
#ifndef _BMP_H
#define _BMP_H

#define BMP_TYPE		0x4D42

typedef struct tagBITMAPFILEHEADER {
	unsigned short  bfType;             // this is 2 bytes
	unsigned int bfSize;             // this is 4 bytes
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned int bfOffBits;
}tagBITMAPFILEHEADER;

typedef struct BITMAPV5HEADER { // DWORD, LONG: 4 bytes WORD: 2 bytes
	unsigned int        bV5Size; // this header size
	int   		    	bV5Width;
	int     		 	bV5Height; // If negative, first row is top
	unsigned short      bV5Planes;
	unsigned short      bV5BitCount; // Bits per pixel
	unsigned int        bV5Compression;
	unsigned int        bV5SizeImage;
	int    			   	bV5XPelsPerMeter;
	int  		     	bV5YPelsPerMeter;
	unsigned int        bV5ClrUsed;
	unsigned int        bV5ClrImportant;
	/*unsigned int        bV5RedMask;
	unsigned int        bV5GreenMask;
	unsigned int        bV5BlueMask;
	unsigned int        bV5AlphaMask;
	unsigned int        bV5CSType;
	CIEXYZTRIPLE	bV5Endpoints; // 36 bytes
	uint16_t        bV5GammaRed;
	uint16_t        bV5GammaGreen;
	uint16_t        bV5GammaBlue;
	uint16_t        bV5Intent;
	uint16_t        bV5ProfileData;
	uint16_t        bV5ProfileSize;
	uint16_t        bV5Reserved;*/
} BITMAPV5HEADER;

typedef struct Bitmap {
	BITMAPV5HEADER bmpInfoHeader;
	unsigned char *bmpData;
} Bitmap;

Bitmap* parseBmp(const char * file);

int drawBmpDouble(Bitmap *bmp, int x, int y);

#endif
