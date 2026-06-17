#ifndef _LAWNMOWER_H_
#define _LAWNMOWER_H_

#include <stdbool.h>
#include "constants.h"
#include "zombie.h"

typedef struct {
    int   row;
    float x;
    bool  rolling;    /* está a rolar? */
    bool  available;  /* ainda não foi usada */
} Lawnmower;

/* Inicializa as 5 lawnmowers */
void init_lawnmowers(void);

/* Atualiza posição e colisões com zombies — chamar a cada tick */
void update_lawnmowers(Zombie *zombies, int max_zombies);

/* Tenta ativar a lawnmower da linha dada.
   Retorna true se ativou (zombie não conta como vida perdida),
   false se já foi usada (perde vida). */
bool try_activate_lawnmower(int row);

/* Devolve true se a lawnmower daquela linha está atualmente a rolar */
bool is_lawnmower_rolling(int row);

const Lawnmower *get_lawnmowers(void);

#endif /* _LAWNMOWER_H_ */
