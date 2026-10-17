#ifndef FONT_H
#define FONT_H
#include "vbe.h"
#include "field.h"

void draw_0(unsigned long x,unsigned long y,int color);
void draw_1(unsigned long x,unsigned long y,int color);
void draw_2(unsigned long x,unsigned long y,int color);
void draw_3(unsigned long x,unsigned long y,int color);
void draw_4(unsigned long x,unsigned long y,int color);
void draw_5(unsigned long x,unsigned long y,int color);
void draw_6(unsigned long x,unsigned long y,int color);
void draw_7(unsigned long x,unsigned long y,int color);
void draw_8(unsigned long x,unsigned long y,int color);
void draw_9(unsigned long x,unsigned long y,int color);
void draw_A(unsigned long x,unsigned long y,int color);
void draw_B(unsigned long x,unsigned long y,int color);
void draw_C(unsigned long x,unsigned long y,int color);
void draw_D(unsigned long x,unsigned long y,int color);
void draw_E(unsigned long x,unsigned long y,int color);
void draw_F(unsigned long x,unsigned long y,int color);
void draw_G(unsigned long x,unsigned long y,int color);
void draw_H(unsigned long x,unsigned long y,int color);
void draw_I(unsigned long x,unsigned long y,int color);
void draw_J(unsigned long x,unsigned long y,int color);
void draw_K(unsigned long x,unsigned long y,int color);
void draw_L(unsigned long x,unsigned long y,int color);
void draw_M(unsigned long x,unsigned long y,int color);
void draw_N(unsigned long x,unsigned long y,int color);
void draw_O(unsigned long x,unsigned long y,int color);
void draw_P(unsigned long x,unsigned long y,int color);
void draw_Q(unsigned long x,unsigned long y,int color);
void draw_R(unsigned long x,unsigned long y,int color);
void draw_S(unsigned long x,unsigned long y,int color);
void draw_T(unsigned long x,unsigned long y,int color);
void draw_U(unsigned long x,unsigned long y,int color);
void draw_V(unsigned long x,unsigned long y,int color);
void draw_W(unsigned long x,unsigned long y,int color);
void draw_X(unsigned long x,unsigned long y,int color);
void draw_Y(unsigned long x,unsigned long y,int color);
void draw_Z(unsigned long x,unsigned long y,int color);
void draw_2dots(unsigned long x, unsigned long y, int color);
void draw_space(unsigned long x, unsigned long y, int color);
void draw_unrecognized(unsigned long x, unsigned long y, int color);

void draw_string(char* c, unsigned long x, unsigned long y, unsigned long color);
void draw_char(char c, unsigned long x, unsigned long y, unsigned long color);
void draw_cp(int c,unsigned long x,unsigned long y,int color);








#endif
