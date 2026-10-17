#ifndef _BUFFER_H_
#define _BUFFER_H_

void initBuffer();

void buffer_drawPixel(unsigned int index, unsigned char color);

void printBuffer(unsigned char* destiny_adress);

void clearBuffer();

void fillBuffer(unsigned char color);

void freeBuffer();

#endif //_BUFFER_H_
