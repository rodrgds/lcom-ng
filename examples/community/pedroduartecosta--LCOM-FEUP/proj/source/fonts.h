#ifndef __FONTS_H
#define __FONTS_H

#include "Bitmap.h"



#define shiftP		0x2a
#define shiftR		0xaa
#define codeA		0x1e
#define codeB		0x30
#define codeC		0x2e
#define codeD		0x20
#define codeE 		0x12
#define codeF		0x21
#define codeG		0x22
#define codeH		0x23
#define codeI		0x17
#define codeJ		0x24
#define codeK		0x25
#define codeL		0x26
#define codeM		0x32
#define codeN		0x31
#define codeO		0x18
#define codeP		0x19
#define codeQ		0x10
#define codeR		0x13
#define codeS		0x1f
#define codeT		0x14
#define codeU		0x16
#define codeV		0x2f
#define codeX		0x2d
#define codeY		0x15
#define codeW		0x11
#define codeZ		0x2c
#define code0		0xb
#define code1		0x2
#define code2		0x3
#define code3		0x4
#define code4		0x5
#define code5		0x6
#define code6		0x7
#define code7		0x8
#define code8		0x9
#define code9		0xa
#define codeEspaco	0x39
#define codeEnter	0x1c
#define codeBack	0xe

/** @defgroup Fonts Fonts
 * @{
 * Functions for manipulating fonts
 */

typedef struct{

	int shiftPressed;
	Bitmap* a;
	Bitmap* aa;
	Bitmap* b;
	Bitmap* bb;
	Bitmap* c;
	Bitmap* cc;
	Bitmap* d;
	Bitmap* dd;
	Bitmap* e;
	Bitmap* ee;
	Bitmap* f;
	Bitmap* ff;
	Bitmap* g;
	Bitmap* gg;
	Bitmap* h;
	Bitmap* hh;
	Bitmap* i;
	Bitmap* ii;
	Bitmap* j;
	Bitmap* jj;
	Bitmap* k;
	Bitmap* kk;
	Bitmap* l;
	Bitmap* ll;
	Bitmap* m;
	Bitmap* mm;
	Bitmap* n;
	Bitmap* nn;
	Bitmap* o;
	Bitmap* oo;
	Bitmap* p;
	Bitmap* pp;
	Bitmap* q;
	Bitmap* qq;
	Bitmap* r;
	Bitmap* rr;
	Bitmap* s;
	Bitmap* ss;
	Bitmap* t;
	Bitmap* tt;
	Bitmap* u;
	Bitmap* uu;
	Bitmap* v;
	Bitmap* vv;
	Bitmap* x;
	Bitmap* xx;
	Bitmap* y;
	Bitmap* yy;
	Bitmap* w;
	Bitmap* ww;
	Bitmap* z;
	Bitmap* zz;
	//symbols
	Bitmap* barra;
	Bitmap* doispontos;
	Bitmap* espaco;
	Bitmap* traco;
	//numbers
	Bitmap* zero;
	Bitmap* um;
	Bitmap* dois;
	Bitmap* tres;
	Bitmap* quatro;
	Bitmap* cinco;
	Bitmap* seis;
	Bitmap* sete;
	Bitmap* oito;
	Bitmap* nove;



}Fonts;

/**
 * @brief Loads the fonts bitmaps
 *
 */

Fonts* loadFonts();

/**
 * @brief returns the letter corresponding to the make code
 *
 * @param keyboard make code
 */
char receiveCharMinusculo(unsigned long scancode);


/**
 * @brief returns the letter corresponding to the make code in capital
 *
 * @param keyboard make code
 */
char receiveCharMaiusculo(unsigned long scancode);


/**
 * @brief prints a string
 *
 * @param font to be used
 * @param string to be printed
 * @param x coordinate
 * @param y coordinate
 * @param length of the string
 */
void stringDisplay(Fonts* f, char string[], int x, int y, int length);

/**
 * @brief frees fonts
 *
 * @param font to be deleted
 */

void deleteFonts(Fonts* f);

/**@}*/

#endif
