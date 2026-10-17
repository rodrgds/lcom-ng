#ifndef GAME_H
#define GAME_H

#define STARTBOARDY 10
#define STARTBOARDX 200
#define LENGTH 25
#define WIDTH 25
#define CUBE_SIZE 20
#define BACKGROUNDCOLOR 16
#define BLOCKCOLOR 20
#define ENDCOLOR 4

// ASCII CODE PARA PECAS

// I, J, L, O, S, T, Z
/*
// peca que faz um L
#define BLOCK_L 'L'
// 4 cubos em linha reta
#define BLOCK_I 'I'
// peca que faz uma especie de S
#define BLOCK_S 'S'
// cubo
#define BLOCK_O 'O'
// peca tipo T
#define BLOCK_T 'T'
*/


//void initBoard(char board[WIDTH][LENGTH]);
void show(char board[WIDTH][LENGTH]);
void game();

#endif
