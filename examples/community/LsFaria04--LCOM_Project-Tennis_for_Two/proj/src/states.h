#ifndef STATES_H
#define STATES_H

/// @brief Contém os estados do jogo
typedef enum{
  GAME,
  START_MENU,
  PAUSE_MENU,
  RESTART,
  QUIT
} Game_state;

/// @brief Indica a direção para onde o jogador está virado
typedef enum {
  LEFTD,
  RIGHTD
} Player_direction;

/// @brief Indica a direção do movimento do jogador
typedef enum{
  LEFT_PLAYER,
  RIGHT_PLAYER,
  UP_PLAYER,
  DOWN_PLAYER
} Player_movement;

/// @brief Indica o estado atual do jogador
typedef enum {
  MOVE,
  START,
  CHOOSE_START,
  CHOOSE_START_STOP,
  HIT,
  STOP
} Player_state;

/// @brief Indica o estado atual do movimento da bola
typedef enum {
  UP_BALL_START,
  DOWN_BALL_START,
  UP_BALL,
  DOWN_BALL
} Ball_direction;

#endif
