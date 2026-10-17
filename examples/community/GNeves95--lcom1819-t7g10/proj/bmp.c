#include <lcom/lcf.h>
#include <stdio.h>

#include "bmp.h"
#include "vga.h"

Bitmap* parseBmp(const char * file){
	Bitmap *bmp = (Bitmap *)malloc(sizeof(Bitmap));
	FILE *fd;
	fd = fopen(file, "r");
	
	if(fd == NULL){
		printf("%s(%s): File doesn't exist!\n",__func__, file);
		return NULL;
	}
	
	tagBITMAPFILEHEADER bmpFileHeader;
	
	//fread(&bmpFileHeader, sizeof(tagBITMAPFILEHEADER),1,fd);
	
	fread(&bmpFileHeader, 2, 1, fd);
	
	if(((bmpFileHeader.bfType) != BMP_TYPE)){
		printf("%s(\'%s\'): File not BMP!\n",__func__, file);
		fclose(fd);
		return NULL;
	}
	
	fread(&bmpFileHeader.bfSize, 4, 1, fd);
	fread(&bmpFileHeader.bfReserved1, 2, 1, fd);
	fread(&bmpFileHeader.bfReserved2, 2, 1, fd);
	fread(&bmpFileHeader.bfOffBits, 4, 1, fd);
	
	BITMAPV5HEADER bmpInfoHeader;
	
	fread(&bmpInfoHeader, sizeof(BITMAPV5HEADER),1,fd);
	
	fseek(fd, bmpFileHeader.bfOffBits, SEEK_SET);
	
	unsigned char * bmpData = (unsigned char *)malloc(bmpInfoHeader.bV5SizeImage);
	
	fread(bmpData, 1, bmpInfoHeader.bV5SizeImage,fd);
	
	printf("%d - %d\n\n", bmpInfoHeader.bV5Height, bmpInfoHeader.bV5Width);
	
	if(bmpData == NULL){
		printf("%s(\'%s\'): Couldn't read pixel array!\n",__func__, file);
		fclose(fd);
		return NULL;
	}
	
	fclose(fd);
	
	bmp->bmpInfoHeader = bmpInfoHeader;
	bmp->bmpData = bmpData;
	
	return bmp;
	/*fread(&bmpFileHeader, 2, 1, fd);
	
	if(bmpFileHeader.bfType != BMP_TYPE){
		printf("%s(\'%s\'): File not BMP!\n",__func__, file);
		fclose(fd);
		return NULL;
	}
	
	if(fread(&(bmpFileHeader.bfSize), 4, 1, fd)<=0){
		printf("%s(\'%s\'): Failed retrieving File Size\n", __func__, file);
		return NULL;
	}
	
	if(fread(&(bmpFileHeader.bfReserved1), 2, 1, fd)<=0){
		printf("%s(\'%s\'): Failed retrieving first reserved byte\n", __func__, file);
		return NULL;
	}
	
	if(fread(&(bmpFileHeader.bfReserved2), 2, 1, fd)<=0){
		printf("%s(\'%s\'): Failed retrieving second reserved byte\n", __func__, file);
		return NULL;
	}
	
	fseek(fd, 0x00A, SEEK_SET);
	
	if(fread(&(bmpFileHeader.bfOffBits), 4, 1, fd)<=0){
		printf("%s(\'%s\'): Failed retrieving offset\n", __func__, file);
		return NULL;
	}
	
	long curr_pos = ftell(fd);
	
	(*bmp).bV5Size
	
	return bmp;*/
}

int drawBmpDouble(Bitmap *bmp, int x, int y){
	int width = bmp->bmpInfoHeader.bV5Width, height = bmp->bmpInfoHeader.bV5Height;
	printf("\twidth: %d\n\theight: %d\n", width, height);
	for(unsigned int i=0; i < (unsigned int)height; i++){
		for(unsigned int j=0; j < (unsigned int)width; j++){
			printf("color: 0x%X\n", (*((bmp->bmpData) + (j+i*width)*bits_per_pixel/8)));
			//if((j+i*width)*bits_per_pixel/8 > bmp->bmpInfoHeader.bV5SizeImage) return 1;
			if(i < v_res && j < h_res /*&& (((*((bmp->bmpData) + (j+i*width)*bits_per_pixel/8)) ^ 0xFFFFFF) == 0)*/) set_pixel_double(x+j,y+i,*((bmp->bmpData) + (j+i*width)*bits_per_pixel/8));
		}
	}
	return 0;
}
