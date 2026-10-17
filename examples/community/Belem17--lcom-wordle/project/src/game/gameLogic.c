#include "gameLogic.h"
#include "../drivers/i8042.h"
#include "controllers/keyboardLogic.h"
#include "viewers/stateviewer.h"
#include <lcom/lcf.h>
#include <string.h>
#include "../database/words.h"
#include <ctype.h>

int chooseWord(struct Game *game){
  size_t count = sizeof(words) / sizeof(words[0]);
  size_t start = (size_t)rand() % count;
  for (size_t i = 0; i < count; i++) {
    const char *word = words[(start + i) % count];
    if (word && strlen(word) == MAX_WORD_LEN - 1) {
      strncpy(game->target, word, MAX_WORD_LEN - 1);
      game->target[MAX_WORD_LEN - 1] = '\0';
      return 0;
    }
  }
  return 1;
}
void wordle_trace(char target[5], char guess[5], int trace[5]) {
    int used_in_target[5] = {0}; 

    for (int i = 0; i < 5; i++) {
        if (guess[i] == target[i]) {
            trace[i] = 1;
            used_in_target[i] = 1;
        } else {
            trace[i] = -1;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (trace[i] == -1) {
            for (int j = 0; j < 5; j++) {
                if (!used_in_target[j] && guess[i] == target[j]) {
                    trace[i] = 0;
                    used_in_target[j] = 1;
                    break;
                }
            }
        }
    }
}
void stateChanger(struct Game *game) {
  if (game->signal.key_pressed) {

    if (game->state == STATE_MENU) {
      if (game->signal.key_code == ENTER || game->signal.key_code == SPACE) {
        game->state = STATE_GAME;
      } else if (game->signal.key_code == ESC) {
        game->state = STATE_END;
        clean_screen();
      }
    } 
    else if (game->state == STATE_GAME) {
      if (game->signal.key_code == ESC) {
        game->state = STATE_MENU;
        clean_screen();
        game->current_attempt = 0;
        memset(game->word, 0, sizeof(game->word));
        memset(game->attempts, 0, sizeof(game->attempts));
        memset(game->target, 0, sizeof(game->target));
      }
    } 
    else if (game->state == STATE_WIN || game->state == STATE_LOSE) {
      if (game->signal.key_code == ENTER || game->signal.key_code == SPACE || game->signal.key_code == ESC) {
        game->state = STATE_MENU;
        clean_screen();
        game->current_attempt = 0;
        memset(game->word, 0, sizeof(game->word));
        memset(game->attempts, 0, sizeof(game->attempts));
        memset(game->target, 0, sizeof(game->target));
      }
    }
  }

  if (game->previous_state != STATE_GAME && game->state == STATE_GAME) {
    game->game_needs_init = true;
  }
  
  game->previous_state = game->state;

  if (game->state == STATE_GAME && game->game_needs_init) {
    chooseWord(game);
    normalize_target(game->target);
    clean_screen();
    printf("The target word is: %s\n", game->target);

    game->current_attempt = 0;
    memset(game->word, 0, sizeof(game->word));
    memset(game->attempts, 0, sizeof(game->attempts));
    game->game_needs_init = false;
  }

  game->previous_state = game->state;
}

void gameHandler(struct Game *game) {
  stateChanger(game);
  handleKeyboardInput(game);
  recieveLetter(game);
  showsState(game);
  game->signal.key_pressed = 0;
}

// Função para remover acentos e converter para maiúsculas
void normalize_target(char *target) {
  for (int i = 0; target[i] != '\0'; i++) {
    unsigned char c = (unsigned char)target[i];
    // Troca manualmente os códigos ISO-8859-1 (Latin1) mais comuns
    if (c == 0xe1 || c == 0xe0 || c == 0xe3 || c == 0xe2 || c == 0xe4) target[i] = 'a'; // á à ã â ä
    else if (c == 0xc1 || c == 0xc0 || c == 0xc3 || c == 0xc2 || c == 0xc4) target[i] = 'A'; // Á À Ã Â Ä
    else if (c == 0xe9 || c == 0xe8 || c == 0xea || c == 0xeb) target[i] = 'e'; // é è ê ë
    else if (c == 0xc9 || c == 0xc8 || c == 0xca || c == 0xcb) target[i] = 'E'; // É È Ê Ë
    else if (c == 0xed || c == 0xec || c == 0xee || c == 0xef) target[i] = 'i'; // í ì î ï
    else if (c == 0xcd || c == 0xcc || c == 0xce || c == 0xcf) target[i] = 'I'; // Í Ì Î Ï
    else if (c == 0xf3 || c == 0xf2 || c == 0xf5 || c == 0xf4 || c == 0xf6) target[i] = 'o'; // ó ò õ ô ö
    else if (c == 0xd3 || c == 0xd2 || c == 0xd5 || c == 0xd4 || c == 0xd6) target[i] = 'O'; // Ó Ò Õ Ô Ö
    else if (c == 0xfa || c == 0xf9 || c == 0xfb || c == 0xfc) target[i] = 'u'; // ú ù û ü
    else if (c == 0xda || c == 0xd9 || c == 0xdb || c == 0xdc) target[i] = 'U'; // Ú Ù Û Ü
    else if (c == 0xe7) target[i] = 'c'; // ç
    else if (c == 0xc7) target[i] = 'C'; // Ç
    // Converte para maiúscula
    target[i] = toupper((unsigned char)target[i]);
  }
}
