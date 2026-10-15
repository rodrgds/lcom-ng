#ifndef PLAYER_H
#define PLAYER_H

#include "states.h"
#include <lcom/lcf.h>
#include "xpms.h"
#include "sprite.h"

#define MIN_X  0   
#define MAX_X  1152
#define MIN_Y  432-145 //player height is 150
#define MAX_Y  864

#define MIN_X_P2  100   
#define MAX_X_P2  850 + 150
#define MIN_Y_P2 40
#define MAX_Y_P2 210 + 152

//HITBOX player 1 normal anim
#define HIT_P1_X_MIN 45
#define HIT_P1_Y_MIN 80
#define HIT_P1_X_MAX 120
#define HIT_P1_Y_MAX 155

//HITBOX player 1 reverse anim (y is the same as the normal anim)
#define HIT_REV_P1_X_MAX 55
#define HIT_REV_P1_X_MIN -20

//HITBOX player 2 normal anim
#define HIT_P2_X_MIN -20
#define HIT_P2_Y_MIN 15
#define HIT_P2_X_MAX 120
#define HIT_P2_Y_MAX 160

//HITBOX player 2 reverse anim (y is the same as the normal anim)
#define HIT_REV_P2_X_MAX 110
#define HIT_REV_P2_X_MIN 40

// Defines the range from the player's racket to the ball, representing the area within which the player can potentially hit the ball.
#define HIT_RANGE_X ((HIT_P1_X_MAX - HIT_P1_X_MIN) / 2)
#define HIT_RANGE_Y ((HIT_P1_Y_MAX - HIT_P1_Y_MIN) / 2)

///Usado para diferenciar os jogadores
typedef enum{
  PLAYER1,
  PLAYER2
} Player_numb;


/**
 * @struct Player
 * @brief Struct que contem as informações do jogador
 * @var Player::x
 * Posição x atual do jogador
 * @var Player::y
 * Posição y atual do jogador
 * @var Player::xspeed
 * Velocidade do jogador no eixo dos x
 * @var Player::yspeed
 * Velocidade do jogador no eixo dos y
 * @var Player::moveanim
 * Valor atual da posição do sprite no array da animação de movimentação do jogador
 * @var Player::hitanim
 * Valor atual da posição do sprite no array da animação de acertar a bola
 * @var Player::startanim
 * Valor atual da posição do sprite no array da animação de serviço (acertar a bola no começo do jogo)
 * @var Player::move
 * Array com os sprites usados na animação de movimentação
 * @var Player::moverev
 * Array com os sprites invertidos usados na animação de movimentação
 * @var Player::hit 
 * Array com os sprites usados na animação de acertar a bola
 * @var  Player::hitrev
 * Array com os sprites invertidos usados na animação de acertar a bola
 * @var Player::start
 * Array com os sprites usados na animação de serviço 
 * @var  Player::hitrev
 * Array com os sprites invertidos usados na animação de serviço
 * @var Player::direction
 * Direção em que o jogador está virado
 * @var Player::state
 * Estado do jogador (movimento, acerto da bola, serviço, parado, ...)
 * @var Player::movement
 * Direção do movimento
 * @var Player::player_numb
 * Distingue o player 1 do player 2
*/
typedef struct{
  int x, y;

  int xspeed, yspeed;
  unsigned int moveanim, hitanim, startanim;

  Sprite currentSprite;

  Sprite move[2];
  Sprite moverev[2];
  Sprite hit[4];
  Sprite hitrev[4];
  Sprite start[6];
  Sprite startrev[6];

  Player_direction direction;
  Player_state state;
  Player_movement movement;
  Player_numb player_numb;

} Player;

/**
 * @brief Cria um player1
 * @returns Retorna um apontador para um Player
*/
Player *(createPlayer1)();

/**
 * @brief Cria um player2
 * @returns Retorna um apontador para um Player
*/
Player *(createPlayer2)();

/**
 * @brief Desenha um jogador no ecrã
 * @param player jogador a desenhar
 * @returns Retorna 1 se ocorreu algum erro e 0 se correu tudo bem
*/
int (drawPlayer)(Player *player);

/**
 * @brief Atualiza a direção para onde o jogador está virado
 * @param direction Nova direção do jogador
 * @param player Jogador que vai ter a direção atualizada
*/
void (updatePlayerDirection)(Player_direction direction, Player *player);

/**
 * @brief Obtém os limites atuais para considerar que a bola atingiu a raquete do jogador ("hit box")
 * @param player Jogador que queremos obter os limites
 * @param x_min O minimo valor de x para considerar que ocorreu colisão
 * @param y_min O minimo valor de y para considerar que ocorreu colisão
 * @param x_max O valor máximo de x para considerar que ocorreu colisão
 * @param y_max O valor máximo de y para considerar que ocorreu colisão
*/
void (get_current_hit_limits)(Player *player,int *x_min, int *x_max, int *y_min, int *y_max);

/**
 * @brief Muda o movimento/estado do jogador tendo em conta valores de scancodes do teclado
 * @param player Jogador cujo movimento vai ser atualizado
 * @param scancode Valor do scancode a analisar
*/
void (changePlayerMovementKBD)(Player *player, uint8_t scancode);

/**
 * @brief Atualiza os movimentos/animações/estados do jogador tendo em conta valores recebidos do timer
 * @param player Jogador cujos movimentos vão ser atualizados
 * @param counter Valor do counter ligado às interrupções do timer
 * @param canHitAfterServe Indica se o jogador já pode iniciar a animação de acertar a bola depois de servir
*/
void (updatePlayerMovementsTimer)(Player *player, int counter, bool canHitAfterServe);

/**
 * @brief Atualiza os movimentos/estados do jogador tendo em conta informações recebidas do mouse
 * @param player Jogador cujos movimentos vão ser atualizados
 * @param isLB Indica se o mouse tem o botão esquerdo ativo
 * @param newBallX Retorna o novo valor de X para a bola quando o jogador escolhe um local para fazer o serviço
 * @param canHitAfterServe Indica se o jogador pode atingir a bola depois de servir
*/
void (updatePlayerMovementMouse)(Player *player, bool isLB, int *newBallX, bool canHitAfterServe);

/**
 * @brief Indica se o jogador deve ou não parar tendo em conta os breakcodes/scancodes do teclado
 * @param scancode Valor do scancode/breakcode do teclado
 * @param movement Direção do movimento do jogador
 * @returns True se o jogador deve parar ou falso caso contrário
*/
bool (stopPlayer)(uint8_t scancode, Player_movement movement);

/**
 * @brief Atualiza os valores de posição do jogador tendo em conta a sua direção de movimento
 * @param player Jogador cujos valores de posição vão ser atualizados
*/
void (movePlayer)(Player *player);

/**
 * @brief Atualiza a animação de movimento para um novo sprite
 * @param player Jogador cuja animação de movimento vai ser atualizada
*/
void (moveAnim)(Player *player);

/**
 * @brief Atualiza a animação de acertar a bola para um novo sprite
 * @param player Jogador que vai ter a animação de acertar a bola atualizada
*/
void (hitAnim)(Player *player);

/**
 * @brief Atualiza a animação de serviço para um novo sprite
 * @param player Jogador que vai ter a animação de serviço atualizada
*/
void (startAnim)(Player *player);

/**
 * @brief Atualiza a animação da escolha de local de serviço
 * @param player Jogador que vai ter a animação de escolha de serviço atualizada
*/
void (chooseStartAnim)(Player *player);

/**
 * @brief Repõe os valores de posição de estado do jogador quando algum dos jogadores marca algum ponto
 * @param player Jogador a repor
 * @param hasScored Indica se o jogador foi quem marcou o ponto
*/
void (resetPlayer)(Player *player, bool hasScored);

/**
 * @brief Liberta a memória alocada para o jogagor 1
 * @param player1 jogador que vamos libertar a memória
*/
void (destroyPlayer1)(Player *player1);

/**
 * @brief Liberta a memória alocada para o jogagor2
 * @param player2 jogador que vamos libertar a memória
*/
void (destroyPlayer2)(Player *player2);

#endif
