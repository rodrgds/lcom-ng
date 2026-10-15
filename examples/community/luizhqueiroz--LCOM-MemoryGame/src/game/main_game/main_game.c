#include "main_game.h"

int game_time = 0;
//int game_duration;

Card *board[4][4];
unsigned const int board_h_v_size = 4;
static unsigned int row = 0, col = 0;

static Card *mouse_selected_card = NULL;

static unsigned int cards_turned = 0;
static unsigned int cards_matched = 0;
static Card *cmp1 = NULL, *cmp2 = NULL;

extern bool is_in_game;
extern unsigned int mode;

int (init_game)() {
    is_in_game = true;
    game_time = 0;
    cards_turned = 0;
    cards_matched = 0;
    row = 0;
    col = 0;

    if (create_all_cards()) {
        return 1;
    }

    initialize_board(board, board_h_v_size);

    return 0;
}

int (main_game_display_page)() {
    if (display_number(game_time, 400, 20)) {
        return 1;
    }

    if (display_cards()) {
        return 1;
    }

    return 0;
}

int (main_game_control)() {
    if (cards_turned == 2) {
        if (verify_selected_cards()) {
            return 1;
        }

        cards_turned = 0;
    }

    if (cards_matched == board_h_v_size * board_h_v_size) {
        is_in_game = false;
        mode = WAITING_MODE;
    }

    return 0;
}

int (verify_selected_cards)() {
    if (cmp1 == NULL) {
        return 1;
    }

    if (cmp2 == NULL) {
        return 1;
    }

    if (cmp1->id == cmp2->id) {
        cards_matched += 2;
    } else {
        mouse_selected_card = NULL;
        mode = WAITING_MODE;
    }
    
    return 0;
}

int (unturn_selected_cards)() {
    if (cmp1 == NULL) {
        return 1;
    }

    if (cmp2 == NULL) {
        return 1;
    }

    cmp1->is_turned = false;;
    cmp2->is_turned = false;

    return 0;
}

void (main_game_timer)() {
    if (is_in_game) {
        game_time++;
    }
}

int (main_game_display_mouse)(int x, int y) {
    if (mouse_selected_card != NULL && mode == GAME_MODE) {
        if (display_sprite(select_mouse_cursor, x, y)) {
            return 1;
        }
    } else {
        if (display_sprite(mouse_cursor, x, y)) {
            return 1;
        }
    }

    return 0;
}

void (main_game_control_mouse)(struct packet *pp, int x, int y) {
    Card *card = select_card(x,y);

    if (mouse_selected_card != NULL) {
        if (card != NULL) {
            if (mouse_selected_card->id != card ->id) {
                mouse_selected_card->is_clicked = false;
            }            
        } else {
            mouse_selected_card->is_clicked = false;
            mouse_selected_card = NULL;
            return ;
        }
    }

    mouse_selected_card = card;

    if (mouse_selected_card == NULL) {
        return ;
    }

    if (mouse_selected_card->is_turned) {
        mouse_selected_card = NULL;
        return ;
    }

    if (cards_turned < 2) {
        if (!mouse_selected_card->is_clicked) {
            if (pp->lb) {
                mouse_selected_card->is_clicked = true;
            }
        }
        if (mouse_selected_card->is_clicked) {
            if (!pp->lb) {
                mouse_selected_card->is_clicked = false;
                mouse_selected_card->is_turned = true;
                cards_turned++;
                
                if (cards_turned == 1) {
                    cmp1 = mouse_selected_card;
                } else {
                    cmp2 = mouse_selected_card;
                }
            }
        }
    }
}

Card *(select_card)(int x, int y) {
    int d1 = 0, d2 = 0;

    for (unsigned int i = 0; i < board_h_v_size; i++, d1 += 135) {
        d2 = 0;
        for (unsigned int j = 0; j < board_h_v_size; j++, d2 += 205) {
            if (x >= 40 + d2 - 8 && x < 160 + d2 - 8 && y >= 60 + d1 - 8 && y < 180 + d1 - 8) {
                row = i;
                col = j;
                return board[i][j];
            }
        }
    }

    return NULL;
}

int (display_cards)() {
    unsigned int d1 = 0, d2 = 0;
    board[row][col]->is_selected = true;

    for (unsigned int i = 0; i < board_h_v_size; i++, d1 += 135) {
        d2 = 0;
        for (unsigned int j = 0; j < board_h_v_size; j++, d2 += 205) {
            Card *card = board[i][j];
            if (card->is_turned) {
                if (display_sprite(card->sprite, 60 + d2, 70 + d1)) {
                    return 1;
                }
            } else if (card->is_selected) {
                if (display_sprite(symbol_cover_selected, 40 + d2 - 8, 60 + d1 - 8)) {
                    return 1;
                }
            } else {
                if (display_sprite(symbol_cover, 40 + d2 - 8, 60 + d1 - 8)) {
                    return 1;
                }
            }
        }
    }

    board[row][col]->is_selected = false;

    return 0;
}

void (main_game_control_kbc)(uint8_t *bytes) {
    if (bytes[0] == FIRST_B_SCODE) {
        if (cards_turned < 2) {
            switch (bytes[1]) {
                case UP_BREAKCODE:
                    if (row != 0) {
                        row--;
                    } else {
                        row = 3;
                    }

                    break;
                case DOWN_BREAKCODE:
                    if (row != 3) {
                        row++;
                    } else {
                        row = 0;
                    }

                    break;
                case RIGHT_BREAKCODE:
                    if (col != 3) {
                        col++;
                    } else {
                        col = 0;
                    }

                    break;
                case LEFT_BREAKCODE:
                    if (col != 0) {
                        col--;
                    } else {
                        col = 3;
                    }

                    break;
                default:
                    break;
            }
        }
    } else {
        switch (bytes[0]) {
            case ESC_BREAKCODE:
                mode = MENU_MODE;
                is_in_game = false;
                destroy_all_cards();

                break;
            case ENTER_BREAKCODE:
                if (!board[row][col]->is_turned) {
                    board[row][col]->is_turned = true;

                    cards_turned++;
                    if (cards_turned == 1) {
                        cmp1 = board[row][col];
                    } else {
                        cmp2 = board[row][col];
                    }
                }

                break;
            default:
                break;
        }
    }
}
