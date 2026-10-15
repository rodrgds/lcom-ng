#ifndef BALL_H
#define BALL_H

#include "states.h"
#include <lcom/lcf.h>
#include "xpms.h"
#include "sprite.h"
#include "player.h"

/**
 * @struct Ball
 * @brief Struct que contém as informações da bola
 * @var Ball::x
 * Posição x atual da bola
 * @var Ball::y
 * Posição y atual da bola
 * @var Ball::xspeed
 * Velocidade da bola no eixo dos x
 * @var Ball::yspeed
 * Velocidade da bola no eixo dos y
 * @var Ball::incline
 * Inclinação/declive do movimento da bola
 * @var Ball::currentSprite
 * Sprite que está a ser exibido
 * @var Ball::small
 * Sprite da bola de tamanho pequeno
 * @var Ball::medium
 * Sprite da bola de tamanho médio
 * @var Ball::large
 * Sprite da bola de tamanho grande
 * @var Ball::very_large 
 * Sprite da bola de tamanho muito grande
 * @var Ball::direction
 * Direção da bola (de baixo para cima ou de cima para baixo) 
*/
typedef struct{
  int x, y;

  int xspeed, yspeed;
  int incline;

  Sprite currentSprite;

  Sprite small;
  Sprite medium;
  Sprite large;
  Sprite very_large;

  Ball_direction direction;

} Ball;

/**
 * @brief Cria uma nova bola alocando memória para as suas estruturas
 * @returns Apontador para uma bola
*/
Ball *(createBall)();

/**
 * @brief Desenha a bola no ecrã
 * @param ball Bola que queremos desenhar
 * @returns 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (drawBall)(Ball *ball);

/**
 * @brief Atualiza a direção do movimento da bola
 * @param direction Nova direção do movimento da bola
 * @param ball Bola que vai ter o seu movimento atualizado
*/
void (updateBallDirection)(Ball_direction direction, Ball *ball);

/**
 * @brief Verifica se ocorreu uma colisão e atualiza so valores de direção e inclinação de acordo com a posição da colisão
 * @param ball Bola para verificar a colisão
 * @param player Player para verificar a colisão
*/
void (collisionPlayer)(Ball *ball, Player *player);

/**
 * @brief Atualiza os valores de x e y (move a bola no plano)
 * @param ball Bola que queremos mover
 * @param slow Indica se o movimento deve ser temporariamente mais lento
*/
void (moveBall)(Ball *ball, bool slow);

/**
 * @brief Verifica se ocorreu alguma colisão com as linhas laterais do campo
 * @param ball bola a ser verificada
 * @param background Conteúdo do background (onde se encontra o campo)
 * @returns true se a colisão ocorreu ou falso caso contrário
*/
bool (checkCollisionLine)(Ball *ball, uint32_t *background);

/**
 * @brief Reseta os valores da bola (posição, direção, ...) tendo em conta o jogador que marcou.
 * @param ball Bola a ser resetada
 * @param scoredPlayer Número do jogador que marcou
*/
void (resetBall)(Ball *ball, Player_numb scoredPlayer);

/**
 * @brief Liberta a memória alocada para as estruturas que compõe a bola
 * @param ball Bola cuja memória alocada vai ser libertada
*/
void (destroyBall)(Ball *ball);

#endif
