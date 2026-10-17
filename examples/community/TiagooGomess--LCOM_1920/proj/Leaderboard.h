#pragma once

#include <lcom/lcf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * @brief - Pergunta o nome do jogador, se este fizer uma pontuação rocord.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 * @param topPlayerName - Nome do jogador atual.
 * @return - 0 se houver sucesso, 1 caso contrário.
 */
void askForName(uint8_t* initial_address, char* topPlayerName);

/**
 * @brief - Jogador.
 */
typedef struct {
    const char* name;   /*!< Nome do jogador. */
    uint8_t score;      /*!< Pontuação do jogador. */
} Player;

/**
 * @brief - Lê o ficheiro da leaderboard (Leaderboard.txt).
 */
void read_leaderboard_file();

/**
 * @brief - Desenha a leaderboard.
 * @param initial_address - Zona da memória gráfica onde queremos desenhar.
 */
void display_leaderboard(uint8_t* initial_address);

/**
 * @brief - Mete os jogadores da leaderboard na posição correta, de acordo com as suas pontuações.
 * @param score - Pontuação do jogador atual.
 * @param name - Nome do jogador atual.
 * @return - true se houver um record, false caso contrário.
 */
bool new_record_handler(int score, char* name);

/**
 * @brief - Verifica se o jogador atual obteve uma pontuação record.
 * @param score - Pontuação do jogador atual.
 * @return - true se houver um record, false caso contrário.
 */
bool is_new_record(int score);

/**
 * @brief - Escreve o ficheiro da leaderboard (Leaderboard.txt).
 */
void write_leaderboard_file();

