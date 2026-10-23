#ifndef BULLET_H
#define BULLET_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  BULLET_PLAYER,
  BULLET_ENEMY
} BulletType;

typedef struct {
  int x, y; // posição
  int direction; // -1 para cima, +1 para baixo
  int speed; // velocidade em pixels/frame
  bool is_active; // bala ativa ou não
  BulletType type; // tipo da bala (jogador ou inimigo)
} Bullet;

void bullet_init(void);
 
// Cria uma nova bala na posição (x y) com direção vertical -1 cima ou +1 baixo, retorna o índice da bala ou -1 se sem espaço
int bullet_spawn(int x, int y, int direction, int speed, BulletType type);
 
// Atualiza posição de todas as balas ativas
void bullet_update(void);
 
// Retorna o array de todas as balas
Bullet *bullet_get_all(void);
 
// Retorna o número de balas ativas
int bullet_get_count(void);
 
// Desativa uma bala pelo índice
void bullet_deactivate(int index);
 
// Limpa o sistema de balas
void bullet_cleanup(void);

#endif /* BULLET_H */
