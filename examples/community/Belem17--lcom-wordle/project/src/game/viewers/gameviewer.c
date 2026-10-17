#include "gameviewer.h"
#include <lcom/lcf.h>
#include "../../drivers/graphics.h"
#include "../../drivers/i8042.h"
#include "../../utils/structs.h"
#include "../../utils/frames.h"
#include "../../utils/objects.h"
#include "../controllers/keyboardLogic.h"
#include <string.h>
extern Resources resources;
#define spacing 50
#define xpos (x_axis/2)-123

/**
 * @brief Desenha um quadrado colorido de 48x48 pixels na tela.
 * 
 * @param x Coordenada x do canto superior esquerdo.
 * @param y Coordenada y do canto superior esquerdo.
 * @param color Cor do quadrado (formato 0xRRGGBB).
 */
static void draw_square(int x, int y, uint32_t color) {
  for (int i = 0; i < 48; i++) {
    for (int j = 0; j < 48; j++) {
      vg_draw_pixel(x + i, y + j, color);
    }
  }
}

/**
 * @brief Escreve uma mensagem na tela usando sprites das letras do alfabeto.
 * 
 * @param msg Mensagem a ser exibida (apenas letras maiúsculas e espaços).
 * @param x Coordenada x inicial.
 * @param y Coordenada y inicial.
 */
static void draw_message(const char *msg, int x, int y) {
  for (size_t i = 0; i < strlen(msg); ++i) {
    char c = msg[i];
    if (c == ' ') continue;
    if (c >= 'A' && c <= 'Z')
      drawframe(x + i * spacing, y, resources.Alphabet[c - 'A']);
  }
}

/**
 * @brief Mostra a tela de vitória, incluindo a palavra correta.
 * 
 * @param game Ponteiro para a estrutura do jogo.
 */
void showWin(struct Game *game) {
  drawframe(0, 0, resources.winscreen);
  draw_message(game->target, xpos, 400);
}

/**
 * @brief Mostra a tela de derrota, incluindo a palavra correta.
 * 
 * @param game Ponteiro para a estrutura do jogo.
 */
void showLose(struct Game *game) {
  drawframe(0, 0, resources.losescreen);
  draw_message(game->target, xpos, 400);
}

/**
 * @brief Desenha um quadrado colorido de acordo com o resultado do palpite.
 * 
 * @param x Coordenada x do quadrado.
 * @param y Coordenada y do quadrado.
 * @param trace Valor do resultado (1: verde, 0: amarelo, -1: preto).
 */
static void draw_colored_square(int x, int y, int trace) {
  uint32_t color;
  if (trace == 1) color = 0x00FF00;      // Verde
  else if (trace == 0) color = 0xFFFF00; // Amarelo
  else color = 0x000000;                 // Preto
  draw_square(x, y, color);
}

/**
 * @brief Mostra uma letra na tela usando o sprite correspondente.
 * 
 * @param c Letra a ser exibida (maiúscula).
 * @param x Coordenada x.
 * @param y Coordenada y.
 */
void showLetter(char c, int x, int y) {
  drawframe(x, y, resources.Alphabet[c - 'A']);
}

/**
 * @brief Mostra a palavra atual que o jogador está digitando.
 * 
 * @param game Ponteiro para a estrutura do jogo.
 */
void showCurrentWord(struct Game *game){
  int x = xpos, y = 100;
  size_t len = strlen(game->word);
  for (size_t i = 0; i < MAX_WORD_LEN - 1; ++i) {
    if (i < len && game->word[i] != '\0') {
      showLetter(game->word[i], x + i * spacing, y);
    } 
    else {
      draw_square(x + i * spacing, y, 0x000000); 
    }
  }
}

/**
 * @brief Mostra todas as tentativas feitas pelo jogador, colorindo conforme o resultado.
 * 
 * @param game Ponteiro para a estrutura do jogo.
 */
void showAllAttempts(struct Game *game){
  int x = xpos, y = 100;
  int line_spacing = 60;
  for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
    char *word = (attempt < game->current_attempt) ? game->attempts[attempt] : (attempt == game->current_attempt ? game->word : NULL);
    for (int i = 0; i < MAX_WORD_LEN - 1; ++i) {
      if (word && word[i] != '\0') {
        if (attempt < game->current_attempt) {
          draw_colored_square(x + i * spacing, y + attempt * line_spacing, game->traces[attempt][i]);
        } else {
          draw_square(x + i * spacing, y + attempt * line_spacing, 0x000000);
        }
        showLetter(word[i], x + i * spacing, y + attempt * line_spacing);
      } else {
        draw_square(x + i * spacing, y + attempt * line_spacing, 0x000000); // Quadrado preto para letras vazias
      }
    }
  }
}

/**
 * @brief Mostra o estado atual do jogo na tela.
 * 
 * @param game Ponteiro para a estrutura do jogo.
 */
void showGame(struct Game *game){
  if (game->state == STATE_WIN) {
    showWin(game);
  }
  else if (game->state == STATE_LOSE) {
    showLose(game);
  }
  else if (game->state == STATE_GAME) {
    draw_message("WORDLE", xpos-24, 10);
    showAllAttempts(game);
  }
}

