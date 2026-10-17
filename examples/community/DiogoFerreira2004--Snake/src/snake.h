#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "./Devices/graphics/VBE.h"
#include "./Devices/keyboard/KBC.h"
#include "./Devices/keyboard/i8042.h"
#include "./Devices/timer/i8254.h"
#include "button_pixmap.h"
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdbool.h>
#include <stdint.h>


/**@brief Tamanho inicial da cobra */
#define SNAKE_INITIAL_SIZE 3
/**@brief Tamanho máximo da cobra */
#define SNAKE_MAX_SIZE 600
/**@brief Largura do tabuleiro */
#define GRID_WIDTH 25
/**@brief Altura do tabuleiro */
#define GRID_HEIGHT 20
/**@brief Tamanho de cada célula */
#define CELL_SIZE 25
/**@brief Velocidade da cobra */
#define GAME_SPEED 8
/**@brief Velocidade elevada da cobra */
#define EXTRA_GAME_SPEED 16
/**@brief Máximo de número de paredes */
#define MAX_WALLS 100


/**@brief Cor preta */
#define COLOR_BLACK 0x000000
/**@brief Cor branca */
#define COLOR_WHITE 0xFFFFFF
/**@brief Cor vermelha */
#define COLOR_RED 0xFF0000
/**@brief Cor verde */
#define COLOR_GREEN 0x00FF00
/**@brief Cor azul */
#define COLOR_BLUE 0x0000FF
/**@brief Cor cinzenta */
#define COLOR_GRAY 0x808080
/**@brief Cor amarela*/
#define COLOR_YELLOW 0xFFFF00


/**@brief Estados do jogo */
typedef enum {
  MENU,
  MODE_SELECT,
  PLAYING,
  RULES,
  END,
  EXIT,
} GameState;


/**@brief Modos de jogo */
typedef enum {
  CLASSIC = 0,
  WALLS,
  SWAP_ARROWS,
  FRUIT_POWER,
} GameMode;


/**@brief Tipos de comida */
typedef enum {
  NORMAL,
  SPEED,
  SIZE,
} FruitType;


/**@brief Direções */
typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT
} Direction;


/**@brief Posição */
typedef struct {
  int16_t x;
  int16_t y;
} Position;


/**@brief Segmento da cobra */
typedef struct {
  Position position;
} SnakeSegment;


/**@brief Atributos do botão*/
typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  char *text;
  bool selected;
} Button;


/**@brief Parede */
typedef struct {
  int x, y;
} Wall;


/**@brief Variáveis do jogo */
typedef struct {
  GameState state;
  SnakeSegment snake[SNAKE_MAX_SIZE];
  int snake_size;
  Direction direction;
  Direction next_direction;
  Position food;
  int score;
  int speed;
  int counter;
  int counter_end;
  Button start_button;
  Button rules_button;
  Button exit_button;
  int food_timer;
  int food_lifetime;
  Wall walls[MAX_WALLS];
  int wall_count;
  uint8_t selected_modes;
  Button classic_mode_button;
  Button walls_mode_button;
  Button swap_arrows_button;
  Button fruit_power_button;
  FruitType fruit_type;
  int extra_speed_timer;
  bool board_needs_full_redraw;
} Game;


/**
 *@brief Função auxiliar que inicializa as variáveis da struct Game
 *
 *
 * @return Retorna 0 em caso de sucesso
 */
int snake_init();

/**
 *@brief Função auxiliar que atualiza a posição atual da cobra no jogo (muda as variáveis da struct Game), aplicando a nova direção escolhida pelo uitlizador. 
 *Primeiro, move todos os segmentos da cobra, atualizando o vetor de SnakeSegments. 
 *Depois, atualiza a primeira posição desse vetor, movendo a cabeça da cobra na direção atual.
 *
 */
void snake_move();

/**
 *@brief Função auxiliar que identifica a ocorrência de colisões com outros elementos do jogo. 
 *Verifica se a cobra colidiu com a fronteira do jogo, com as paredes do tabuleiro ou se colidiu consigo mesma.
 *Verifica, também, se a cobra colidiu com a comida. 
 *Se isso acontecer, o resultado pode variar dependendo do modo de jogo: a pontuação pode aumentar de forma diferente, podem surgir novas paredes no tabuleiro e a velocidade da cobra pode mudar.
 *
 *
 */
bool snake_check_collision();

/**
 *@brief Função auxiliar que gera uma nova posição de comida, guardando na struct Game.
 *Verifica se a comida colide com as paredes ou com a cobra. Se isso acontecer, gera uma nova posição.
 *Se o jogo estiver no modo FRUIT_POWER, é atribuído um tipo à nova comida, sendo menos provável que apareçam frutas mais valiosas.
 *
 *
 */
void snake_spawn_food();

/**
 *@brief Função auxiliar que desenha a parte estática do menu do jogo.
 *Desenha o título e o background do menu.
 *
 *
 */
void draw_static_menu();

/**
 *@brief Função auxiliar que desenha a parte estática do menu de escolha de modo jogo.
 *Desenha o título e o background do menu.
 *
 *
 */
void draw_static_mode_select();

  /**
   *@brief Função auxiliar que desenha no ecrã o menu do jogo.
   *Desenha os butões: START,RULES e EXIT, escolhendo o xpm correspondente que depende da seleção do utilizador.
   *
   *
   */
  void snake_draw_menu();

/**
 *@brief Função auxiliar que desenha o estado atual do tabuleiro. Desenha o tabuleiro em padrão quadriculado. 
 *Apaga a cauda anterior, se esta se moveu e apaga comida anterior, se tiver sido gerada uma nova.
 *Desenha a cobra com o vetor de posições SnakeSegment.
 *Dependendo do modo, desenha a comida com uma certa cor, que varia de acordo com o seu tipo e desenha paredes no tabuleiro.
 *Atualiza a pontuação do utilizador no canto do ecrâ.
 *Para um melhor desenpenho, guarada as popsições anterires da cobra e da comida para uma posterior atualização.
 *
 *
 */
void snake_draw_board();


/**
 *@brief Função auxiliar que desenha a borda do tabuleiro, tendo em conta os parâmetros.


 *@param board_x posição horizontal onde o tabuleiro começa
 *@param board_y posição vertival onde o tabuleiro começa
 *
 *
 */
void snake_draw_background(int board_x, int board_y);


/**
 *@brief Função auxiliar que desenha a página que apresenta as regras do jogo
 *
 */
void snake_draw_rules();



/**
 *@brief Função auxiliar que desenha a página final do jogo, quando o utilizador perde, mostrando os pontos que ele obteve
 *
 */
void snake_draw_end();



/**
 *@brief Função auxiliar que desenha a página final do jogo, quando o utilizador perde, mostrando os pontos que ele obteve
 *
 */
int(proj_main_loop)();


/**
 *@brief Função auxiliar que reinicia o jogo, reiniciando variáveis da struct Game como a pontuação e o tamanho e velocidade da cobra, para começar um novo jogo
 *
 */
void snake_reset();


/**
 *@brief Função auxiliar que desenha a pontuação atual do jogo, usando xpms de cada dígito
 *
 * 
 * @param x coordenada horizontal onde a pontuação começa a ser desenhada
 * @param y coordenada vertical onde a pontuação começa a ser desenhada
 *
 */
void snake_draw_points(int x, int y);


/**
 *@brief Função auxiliar que desenha o ecrã que permite selecionar o modo de jogo, usando xpms diferentes mediante o estado de cada botão
 *
 */
void snake_draw_mode_select();



/**
 *@brief Função auxiliar que adiciona paredes no tabuleiro, verificando se ainda há espaço e se a posição é válida
 *
 */
void snake_spawn_wall();

#endif
