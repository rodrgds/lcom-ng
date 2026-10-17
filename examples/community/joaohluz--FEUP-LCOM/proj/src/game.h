#ifndef _GAME_H_
#define _GAME_H_

/** @defgroup Game Game
 * @{
 *
 * Funções para programar a lógica do jogo.
 */

#include "graphics.h"
#include "xpm.h"
#include "menu.h"
#include "file.h"



/**
 * @brief Struct usada para definir o objeto Asteroid
 * @param x - coordenada x do Asteroid
 * @param y - coordenada y do Asteroid
 * @param speedX - velocidade no eixo dos x
 * @param speedY - velocidade no eixo dos y
 */
typedef struct {
     int x, y, speedX, speedY;
    
} Asteroid;

/**
 * @brief Struct usada para definir o objeto Alien
 */
typedef struct {
     int x, y, speed, shot, lives, level;
     bool alive,right, left, colision;
} Alien;

/**
 * @brief Struct usada para definir o objeto Jogo.
 * É nesta struct que é guardada a informação de todas as imagens usadas no jogo.
 */
typedef struct {

  xpm_image_t background_info;
  xpm_string_t* background_map;
  unsigned char* background_pic;

  xpm_image_t ship_info;
  xpm_string_t* ship_map;
  unsigned char* ship_pic;

  xpm_image_t asteroid_info;
  xpm_string_t* asteroid_map;
  unsigned char* asteroid_pic;

  xpm_image_t ship_explosion_info;
  xpm_string_t* ship_explosion_map;
  unsigned char* ship_explosion_pic;

  xpm_image_t name_info;
  xpm_string_t* name_map;
  unsigned char* name_pic;

  xpm_image_t menu_info;
  xpm_string_t* menu_map;
  unsigned char* menu_pic;

  xpm_image_t menuS_info;
  xpm_string_t* menuS_map;
  unsigned char* menuS_pic;

  xpm_image_t menuH_info;
  xpm_string_t* menuH_map;
  unsigned char* menuH_pic;

  xpm_image_t menuE_info;
  xpm_string_t* menuE_map;
  unsigned char* menuE_pic;

  xpm_image_t pause_info;
  xpm_string_t* pause_map;
  unsigned char* pause_pic;

  xpm_image_t gameOver_info;
  xpm_string_t* gameOver_map;
  unsigned char* gameOver_pic;

  xpm_image_t gameWon_info;
  xpm_string_t* gameWon_map;
  unsigned char* gameWon_pic;

  xpm_image_t instructions_info;
  xpm_string_t* instructions_map;
  unsigned char* instructions_pic;

  xpm_image_t shield_info;
  xpm_string_t* shield_map;
  unsigned char* shield_pic;

  xpm_image_t rato_info;
  xpm_string_t* rato_map;
  unsigned char* rato_pic;

  xpm_image_t SCORE_info;
  xpm_string_t* SCORE_map;
  unsigned char* SCORE_pic;

  xpm_image_t alien_info;
  xpm_string_t* alien_map;
  unsigned char* alien_pic;

  xpm_image_t shot_info;
  xpm_string_t* shot_map;
  unsigned char* shot_pic;

  xpm_image_t shot2_info;
  xpm_string_t* shot2_map;
  unsigned char* shot2_pic;

  xpm_image_t bang_info;
  xpm_string_t* bang_map;
  unsigned char* bang_pic;

  xpm_image_t num0_info;
  xpm_string_t* num0_map;
  unsigned char* num0_pic;

  xpm_image_t num1_info;
  xpm_string_t* num1_map;
  unsigned char* num1_pic;

  xpm_image_t num2_info;
  xpm_string_t* num2_map;
  unsigned char* num2_pic;

  xpm_image_t num3_info;
  xpm_string_t* num3_map;
  unsigned char* num3_pic;

  xpm_image_t num4_info;
  xpm_string_t* num4_map;
  unsigned char* num4_pic;

  xpm_image_t num5_info;
  xpm_string_t* num5_map;
  unsigned char* num5_pic;

  xpm_image_t num6_info;
  xpm_string_t* num6_map;
  unsigned char* num6_pic;

  xpm_image_t num7_info;
  xpm_string_t* num7_map;
  unsigned char* num7_pic;

  xpm_image_t num8_info;
  xpm_string_t* num8_map;
  unsigned char* num8_pic;

  xpm_image_t num9_info;
  xpm_string_t* num9_map;
  unsigned char* num9_pic;

  xpm_image_t lA_info;
  xpm_string_t* lA_map;
  unsigned char* lA_pic;

  xpm_image_t lB_info;
  xpm_string_t* lB_map;
  unsigned char* lB_pic;

  xpm_image_t lC_info;
  xpm_string_t* lC_map;
  unsigned char* lC_pic;

  xpm_image_t lD_info;
  xpm_string_t* lD_map;
  unsigned char* lD_pic;

  xpm_image_t lE_info;
  xpm_string_t* lE_map;
  unsigned char* lE_pic;

  xpm_image_t lF_info;
  xpm_string_t* lF_map;
  unsigned char* lF_pic;

  xpm_image_t lG_info;
  xpm_string_t* lG_map;
  unsigned char* lG_pic;

  xpm_image_t lH_info;
  xpm_string_t* lH_map;
  unsigned char* lH_pic;

  xpm_image_t lI_info;
  xpm_string_t* lI_map;
  unsigned char* lI_pic;

  xpm_image_t lJ_info;
  xpm_string_t* lJ_map;
  unsigned char* lJ_pic;

  xpm_image_t lK_info;
  xpm_string_t* lK_map;
  unsigned char* lK_pic;

  xpm_image_t lL_info;
  xpm_string_t* lL_map;
  unsigned char* lL_pic;

  xpm_image_t lM_info;
  xpm_string_t* lM_map;
  unsigned char* lM_pic;

  xpm_image_t lN_info;
  xpm_string_t* lN_map;
  unsigned char* lN_pic;

  xpm_image_t lO_info;
  xpm_string_t* lO_map;
  unsigned char* lO_pic;

  xpm_image_t lP_info;
  xpm_string_t* lP_map;
  unsigned char* lP_pic;

  xpm_image_t lQ_info;
  xpm_string_t* lQ_map;
  unsigned char* lQ_pic;

  xpm_image_t lR_info;
  xpm_string_t* lR_map;
  unsigned char* lR_pic;

  xpm_image_t lS_info;
  xpm_string_t* lS_map;
  unsigned char* lS_pic;

  xpm_image_t lT_info;
  xpm_string_t* lT_map;
  unsigned char* lT_pic;

  xpm_image_t lU_info;
  xpm_string_t* lU_map;
  unsigned char* lU_pic;

  xpm_image_t lV_info;
  xpm_string_t* lV_map;
  unsigned char* lV_pic;

  xpm_image_t lW_info;
  xpm_string_t* lW_map;
  unsigned char* lW_pic;

  xpm_image_t lX_info;
  xpm_string_t* lX_map;
  unsigned char* lX_pic;

  xpm_image_t lY_info;
  xpm_string_t* lY_map;
  unsigned char* lY_pic;

  xpm_image_t lZ_info;
  xpm_string_t* lZ_map;
  unsigned char* lZ_pic;

  xpm_image_t barra_info;
  xpm_string_t* barra_map;
  unsigned char* barra_pic;

  xpm_image_t highscores_info;
  xpm_string_t* highscores_map;
  unsigned char* highscores_pic;

  //Alien* aliens[3];
} Jogo;

/**
 * @brief Struct usada para definir o objeto Player
 */
typedef struct {
    int x, y, speed, shot, lives, score;
    bool right,left, alive, colision;
} Player;


/**
 * @brief Inicia o jogo fazendo load de todas as imagens necessárias
 */
Jogo* inicio();

/**
 * @brief Desenha o estado atual do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param player - struct com a informação relativa ao estado atual do jogador
 * @param alien - struct com a informação relativa ao estado atual do jogador
 */
void drawJogo(Jogo* jogo, Player* player, Alien* alien);

/**
 * @brief Desenha o background do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param player - struct com a informação relativa ao estado atual do jogador
 */
void draw_background(Jogo * jogo, Player* player);

/**
 * @brief Liberta o espaço de memória préviamnete ocupado pela struct Jogo
 * @param jogo - apontador para a struct
 */
void fim(Jogo* jogo);

/**
 * @brief Inicia a struct Player alocando a memória necessária e atribuindo valores a todos os parametros
 * @return Devolve o apontador para a struct Player
 */
Player* playerInit();

/**
 * @brief Dá aos parametros da struct Player os seus valores iniciais
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param player - struct com a informação relativa ao estado atual do jogador
 */
void reset_player(Jogo* jogo, Player* player);

/**
 * @brief Altera o estado atual do jogo com base no scan code do keyboard
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param player - struct com a informação relativa ao estado atual do jogador
 */
void move_ship(Jogo* jogo, Player* player);

/**
 * @brief Altera o estado atual do jogo com base nos parametros da struct Alien
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param alien - struct com a informação relativa ao estado atual do alien
 */
void move_alien(Jogo* jogo, Alien* alien);

/**
 * @brief Verifica se o jogador pode disparar e altera o parametro da struct Player responsável pelo disparo do jogador
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param player - struct com a informação relativa ao estado atual do jogador
 */
void check_player_fire(Jogo* jogo,Player* player);

/**
 * @brief Liberta o espaço de memória préviamnete ocupado pela struct player
 * @param player - apontador para a struct
 */
void playerDelete(Player* player);

/**
 * @brief Inicia a struct Alien alocando a memória necessária e atribuindo valores a todos os parametros
 * @param jogo - struct com a informação de todas as imagens usadas
 * @return Devolve o apontador para a struct Alien
 */
Alien* alienInit(Jogo* jogo);

/**
 * @brief Dá aos parametros da struct alien os seus valores iniciais
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param alien - struct com a informação relativa ao estado atual do alien
 * @param level - nivel atual do jogo
 * @param lives - vidas do alien para o nivel atual do jogo
 * @param speed - velocidade do alien para o nibel atual
 */
void reset_alien(Jogo* jogo, Alien* alien,int level, int lives, int speed);

/**
 * @brief Liberta o espaço de memória préviamnete ocupado pela struct alien
 * @param alien - apontador para a struct
 */
void alienDelete(Alien* alien);

/**
 * @brief Inicializa as variáveis que guardam as coordenadas do tiro disparado pelo alien
 * @param alien - struct com a informação relativa ao estado atual do alien
 * @param jogo - struct com a informação de todas as imagens usadas
 */
void alien_shot_init(Jogo* jogo, Alien* alien);

/**
 * @brief Verifica se há colisão entre o tiro do alien e a nave do jogador
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param alien - struct com a informação relativa ao estado atual do alien
 * @param player - struct com a informação relativa ao estado atual do player
 */
void alien_fire(Jogo* jogo, Alien* alien, Player* player);

/**
 * @brief Verifica se há colisão entre o tiro do jogador e o alien
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param alien - struct com a informação relativa ao estado atual do alien
 * @param player - struct com a informação relativa ao estado atual do player
 */
void player_fire(Jogo* jogo, Alien* alien, Player* player);

/**
 * @brief Toma uma determinda ação com base numa interrupção do keyboard ou do mouse
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
void kbd_read();

//bool shot_player(int x, int y, Player* player);

//void set_mouse_events();

/**
 * @brief Desenha a pontuação do jogador durante o jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param player - struct com a informação relativa ao estado atual do player
 */
void show_score(Jogo* jogo, Player* player);

/**
 * @brief Desenha o número passado como parametro
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param x - coordenada do eixo horizontal onde desenhar o número
 * @param y - coordenada do eixo horizontal onde desenhar o número
 * @param number - número a ser desenhado
 */
void display_number(Jogo* jogo,int x, int y, int number);

/**
 * @brief Desenha o número passado como parametro
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param x - coordenada do eixo horizontal onde desenhar o número
 * @param y - coordenada do eixo horizontal onde desenhar o número
 * @param number - número a ser desenhado
 */
void display_char_number(Jogo* jogo, char number, int x, int y);

/**
 * @brief Desenha o menu do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param mouse - struct com a informação relativa ao estado atual do mouse
 */
void drawMenu(Jogo* jogo, Mouse* mouse);

/**
 * @brief Toma uma determinda ação com base numa interrupção do keyboard ou do mouse podendo alternar entre difeentes estados do jogo
 */
void menu_kb_ih();

/**
 * @brief Desenha o menu de pausa do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 */
void drawPause(Jogo* jogo);

/**
 * @brief Desenha o menu de Game Over do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 */
void drawGameOver(Jogo* jogo);

/**
 * @brief Desenha o menu de You Won do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 */
void drawWon(Jogo* jogo);

/**
 * @brief Desenha o menu de instruções do jogo
 * @param jogo - struct com a informação de todas as imagens usadas
 */
void drawInstructions(Jogo* jogo);

/**
 * @brief Associa um scancode do keyboard a um caracter
 * @param name - string na qual é guardado o caracter
 * @param i - indice do array no qual é guardado o caracter
 */
void show_letter_byte(char* name, int i);

/**
 * @brief Desenha uma letra passada como argumento
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param letter - letra a ser desenhada pela função
 * @param x - coordenada do eixo horizontal onde deve ser denhada a letra
 * @param y - coordenada do eixo vertical onde deve ser denhada a letra
 */
void show_letter_file(Jogo* jogo, char letter, int x, int y);

/**
 * @brief Inicia a struct Asteroid alocando a memória necessária e atribuindo valores a todos os parametros
 * @return Devolve o apontador para a struct Asteroid
 */
Asteroid* asteroidInit(Player* player, int x);

/**
 * @brief Responsável pela transição entre niveis ajustando as posições e velocidade dos asteroides confirmando a colisão
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param alien - struct com a informação relativa ao estado atual do alien
 * @param player - struct com a informação relativa ao estado atual do player
 */
void level_transition(Jogo* jogo, Alien* alien,Player* player);

/**
 * @brief Desenha o score dos três melhores jogadores no menu de highscores
 * @param jogo - struct com a informação de todas as imagens usadas
 * @param users - lista ligada com a informação de todos os users do jogo incluindo os seus scores
 */
void display_score(Jogo* jogo, Users users);

extern int mouseX,mouseY;

/**@}*/

#endif
