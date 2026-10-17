 #ifndef GAME_H
#define GAME_H
#include "field.h"




void set_fields(int lado, int mines, int row, int col, Campo_t campo[][getLado()]);
void clear_fields(int lado,Campo_t field[][getLado()]);
int single_play(Campo_t field[][getLado()]);
void reveal(int i,int j,Campo_t field[][getLado()]);
int menu();
int multiplayer(int host, Campo_t field[][getLado()]) ;
#endif
