#ifndef JOGADOR_H
#define JOGADOR_H

#include "video_gr.h"

typedef struct{
  unsigned short x;
  unsigned short y;
  double xspeed;
  double yspeed;
  unsigned short h,w;
} Jogador;

int updateJogador(Jogador *jogador, double FPS);

#endif
