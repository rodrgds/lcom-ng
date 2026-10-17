#ifndef JOGO_H
#define JOGO_H

#include <stdlib.h>
#include <time.h>

#include "i8042.h"
#include "i8254.h"
#include "kbc.h"
#include "video_gr.h"
#include "jogador.h"
#include "plataforma.h"
#include "log.h"
#include "image.h"
// #include "sprite.h"

double FPS;
unsigned int NPLATFORMS;

typedef struct{

  Jogador *jogador;
  Plataforma *plataformas;

} Jogo;

int start(Jogo* jogo);
int loop(Jogo* jogo);
int quit(Jogo* jogo);

int checkCollisions(Jogo *jogo);

int kbd_subscribe_int(Jogo* jogo);
int kbd_unsubscribe_int(Jogo* jogo);
int kbd_int_handler(Jogo* jogo);

int timer_subscribe_int(Jogo* jogo, int hook);
int timer_unsubscribe_int(Jogo* jogo, int hook);
int timer_set_square(unsigned long timer, unsigned long freq);
int timer_get_conf(unsigned long timer, unsigned char *st);

#endif
