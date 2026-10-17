#ifndef __JOGO_H
#define __JOGO_H

#include "bitmap.h"
#define HEIGHT_EXPERT 16
#define WIDTH_EXPERT 30
#define NUM_MINES_EXPERT 100

typedef struct{
	int carregado; ///0 se nao estiver, 1 se carregado,  2 se flag
	int valor; ///-1 se mina, 0 se vazia, outro valor
}Mine;

Bitmap* Cron_0;
Bitmap* Cron_1;
Bitmap* Cron_2;
Bitmap* Cron_3;
Bitmap* Cron_4;
Bitmap* Cron_5;
Bitmap* Cron_6;
Bitmap* Cron_7;
Bitmap* Cron_8;
Bitmap* Cron_9;
Bitmap* Player1_table;
Bitmap* Player2_table;

///@brief prints games table
///@param table mine bidimensional array
void print_table(Mine** table);

///@brief creates game table
///@param difficulty
///@return table
Mine** create_table(int difficulty);

///@brief generates interrupts from the timer keyboard and mouse
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
void create_interrupts(int *irq_set_timer,int *irq_set_keyboard,int *irq_set_mouse);

///@brief stops interrupts from the timer keyboard and mouse
void stop_interrupts();

///@brief
///@param difficulty
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
///@param jogador
///@return success
int post_game_state_multi(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse,int jogador);

///@brief games multiplayer game mode
///@param difficulty
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
///@return success
int jogo_multi_player(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse);

///@brief actions taken when right button of mouse clicks on the table
///@param table
///@param x
///@param y
///@param difficulty
///@return success
int right_click_screen(Mine*** table, int x, int y, int difficulty);

///@brief actions taken when the mouse clicks outside the tabke limits
///@param table
///@param k horizontal square k - 1
///@param j vertical square j - 1
///@param difficulty
///@param por_carregar
///@param single
void click_vazio(Mine*** table, int k, int j, int difficulty,int *por_carregar,int single);

///@brief actions taken when left button of mouse clicks on the table
///@param table
///@param x
///@param y
///@param difficulty
///@param filled 1 if yes 0 if not
///@param por_carregar 1 if yes 0 if not
///@param single
///@return success
int click_screen(Mine*** table, int x, int y, int difficulty, int *filled,int *por_carregar,int single);

///@brief fills game table with bombs and numbers
///@param table bidimensional array
///@param difficulty
///@param k horizontal square k - 1
///@param j vertical square j - 1
///@param single
///@param seed
///@param use
///@return table
Mine** fill_table(Mine** table,int difficulty,int k, int j,int single,char *seed,int use); /// use é para usar a seed para criar

///@brief
///@param difficulty
///@param win 1 if yes 0 if not
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
///@param table
///@return success
int post_game_state(int difficulty,int win,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse,Mine** table);


///@brief games single player mode
///@param difficulty 0 for easy 1 for medium 2 for expert
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
///@return success
int jogo_single_player(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse);

///@brief updates the game timer and draws it
///@param tempo
void update_cronometro(int tempo);

//@brief loads the bitmaps necessary for the timer
void load_cronometro();

///@brief deletes the bitmaps used for the timer
void delete_cronometro();

///@brief updates the timer on the multi player game and draws it
///@param tempo
///@param jogador
///@param jogador1
///@param jogador2
void update_multi_cronometro(int tempo, int jogador,int jogador1,int jogador2);

///@brief draws the bitmap presented when the game ends
///@param turn
///@param jogador
void draw_end_scree_multi_porta(int turn,int jogador);

///@brief screen where it shows if a player won or lost
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
///@param turn
///@param jogador
void end_scree_multi_porta(int irq_set_timer,int irq_set_keyboard,int irq_set_mouse,int turn,int jogador);

#endif
