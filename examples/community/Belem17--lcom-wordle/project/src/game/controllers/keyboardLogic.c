#include <lcom/lcf.h>
#include "keyboardLogic.h"
#include "../../utils/structs.h"
#include "../../utils/frames.h"
#include "../../utils/objects.h"
#include "../../drivers/i8042.h"
#include "../gameLogic.h"
extern Resources resources;

void handleKeyboardInput(struct Game *game) {

    if (game->state == STATE_GAME) {
        if (game->signal.key_pressed) {
            if (game->signal.key_code == ENTER) {
                if (strlen(game->word) == MAX_WORD_LEN - 1 && game->current_attempt < MAX_ATTEMPTS) {
                    strcpy(game->attempts[game->current_attempt], game->word);
                    wordle_trace(game->target, game->word, game->traces[game->current_attempt]);

                    bool all_correct = true;
                    for (int i = 0; i < MAX_WORD_LEN - 1; ++i) {
                        if (game->traces[game->current_attempt][i] != 1) {
                            all_correct = false;
                            break;
                        }
                    }
                    if (all_correct) {
                        game->state = STATE_WIN;
                    } else {
                        game->current_attempt++;
                        memset(game->word, 0, sizeof(game->word));
                        if (game->current_attempt >= MAX_ATTEMPTS) {
                            game->state = STATE_LOSE;
                        }
                    }
                }
            }
        }
    }
}

char codeToChar(int code) {
    switch (code) {
        case A: return 'A';
        case B: return 'B';
        case C: return 'C';
        case D: return 'D';
        case E: return 'E';
        case F: return 'F';
        case G: return 'G';
        case H: return 'H';
        case I: return 'I';
        case J: return 'J';
        case K: return 'K';
        case L: return 'L';
        case M: return 'M';
        case N: return 'N';
        case O: return 'O';
        case P: return 'P';
        case Q: return 'Q';
        case R: return 'R';
        case S: return 'S';
        case T: return 'T';
        case U: return 'U';
        case V: return 'V';
        case W: return 'W';
        case X: return 'X';
        case Y: return 'Y';
        case Z: return 'Z';
        default: return '\0';  
    }
}

void recieveLetter(struct Game *game){
    char c = '\0';
    if (game->state == STATE_GAME){
        if (game->signal.key_pressed){
            if (game->signal.key_code == BACKSPACE) { 
                int len = strlen(game->word);
                if (len > 0) {
                    game->word[len-1] = '\0';
                }
            } else {
                c = codeToChar(game->signal.key_code);
                int len = strlen(game->word);
                if (c != '\0' && len < MAX_WORD_LEN - 1) {
                    game->word[len] = c;
                    game->word[len+1] = '\0';
                }
            }
            printf("%s\n", game->word);
        }
    }
}
