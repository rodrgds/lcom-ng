#ifndef _LCOM_CONTROLLER_H_
#define _LCOM_CONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct { //Se precisar depois...
    int16_t x;  // Posição x de 16 bits
    int16_t y;  // Posição y de 16 bits
} Position;

typedef struct {
    uint16_t x;  // Posição x de 16 bits
    uint16_t y;  // Posição y de 16 bits
    bool exploded;  // Estado do bomb (explodido ou não)
    bool disappear;  // Se a bomba desaparece após explodir
    int explosionRadius;  // Raio de explosão
    //Position* canBombExplode;  // Vetor dinâmico de posições as quais a bomba pode explodir para
    //size_t canBombExplodeSize;  // Tamanho do vetor de posições
} Bomb;
typedef struct {
    uint16_t x;  // Posição x de 16 bits
    uint16_t y;  // Posição y de 16 bits
} BombUpgrade;

typedef struct {
    uint16_t x;  // Posição x de 16 bits
    uint16_t y;  // Posição y de 16 bits
} BreakableWall;

typedef struct {
    uint16_t x;  // Posição x de 16 bits
    uint16_t y;  // Posição y de 16 bits
} Life;

typedef struct {
    uint16_t x;  // Posição x de 16 bits
    uint16_t y;  // Posição y de 16 bits
    uint16_t lifes;  // Vidas do herói
    bool bombUp;
    uint16_t bombUpTime;
} Hero;



#endif
