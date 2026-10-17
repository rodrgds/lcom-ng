#include "jogador.h"

int updateJogador(Jogador *jogador, double FPS){
  double friction = 0.95;
  jogador->yspeed+=0.1*(120/FPS)*(120/FPS);

  if(jogador->yspeed>(12*120)/FPS) jogador->yspeed=(12*120)/FPS;
  jogador->xspeed *= friction;

  jogador->x += jogador->xspeed;
  if(jogador->y < v_res/2-jogador->h/2 && jogador->yspeed < 0)
    jogador->y += 0;
  else jogador->y += jogador->yspeed;

  return 0;
}
