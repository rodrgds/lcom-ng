#include "game_utils.h"

int (create_all_sprites)() {
    game_title = create_sprite(game_title_xpm);
    if (game_title == NULL) {
        return 1;
    }

    record_title = create_sprite(record_title_xpm);
    if (record_title == NULL) {
        return 1;
    }

    start_option = create_sprite(start_option_xpm);
    if (start_option == NULL) {
        return 1;
    }

    exit_option = create_sprite(exit_option_xpm);
    if (exit_option == NULL) {
        return 1;
    }

    record_option = create_sprite(record_option_xpm);
    if (record_option == NULL) {
        return 1;
    }

    congratulations = create_sprite(congratulations_xpm);
    if (congratulations == NULL) {
        return 1;
    }

    save_option = create_sprite(save_option_xpm);
    if (save_option == NULL) {
        return 1;
    }

    x_option = create_sprite(x_option_xpm);
    if (x_option == NULL) {
        return 1;
    }

    background = create_sprite(background_xpm);
    if (x_option == NULL) {
        return 1;
    }    

    mouse_cursor = create_sprite(standard_mouse_xpm);
    if (mouse_cursor == NULL) {
        return 1;
    }

    select_mouse_cursor = create_sprite(select_mouse_xpm);
    if (select_mouse_cursor == NULL) {
        return 1;
    }

    symbol_1 = create_sprite(symbol_1_xpm);
    if (symbol_1 == NULL) {
        return 1;
    }

    symbol_2 = create_sprite(symbol_2_xpm);
    if (symbol_2 == NULL) {
        return 1;
    }

    symbol_3 = create_sprite(symbol_3_xpm);
    if (symbol_3 == NULL) {
        return 1;
    }

    symbol_4 = create_sprite(symbol_4_xpm);
    if (symbol_4 == NULL) {
        return 1;
    }

    symbol_5 = create_sprite(symbol_5_xpm);
    if (symbol_5 == NULL) {
        return 1;
    }

    symbol_6 = create_sprite(symbol_6_xpm);
    if (symbol_6 == NULL) {
        return 1;
    }

    symbol_7 = create_sprite(symbol_7_xpm);
    if (symbol_7 == NULL) {
        return 1;
    }

    symbol_8 = create_sprite(symbol_8_xpm);
    if (symbol_8 == NULL) {
        return 1;
    }

    symbol_cover = create_sprite(symbol_cover_xpm);
    if (symbol_cover == NULL) {
        return 1;
    }

    symbol_cover_selected = create_sprite(symbol_cover_selected_xpm);
    if (symbol_cover == NULL) {
        return 1;
    }

    return 0;
}

void (destroy_all_sprites)() {
    destroy_sprite(game_title);
    destroy_sprite(record_title);
    destroy_sprite(start_option);
    destroy_sprite(exit_option);
    destroy_sprite(record_option);
    destroy_sprite(congratulations);
    destroy_sprite(save_option);
    destroy_sprite(x_option);

    destroy_sprite(background);

    destroy_sprite(mouse_cursor);
    destroy_sprite(select_mouse_cursor);

    destroy_sprite(symbol_1);
    destroy_sprite(symbol_2);
    destroy_sprite(symbol_3);
    destroy_sprite(symbol_4);
    destroy_sprite(symbol_5);
    destroy_sprite(symbol_6);
    destroy_sprite(symbol_7);
    destroy_sprite(symbol_8);

    destroy_sprite(symbol_cover);
    destroy_sprite(symbol_cover_selected);
}


int (create_all_cards)() {
    card_1 = create_card(1, symbol_1);
    if (card_1 == NULL) {
        return 1;
    }

    card_2 = create_card(2, symbol_2);
    if (card_2 == NULL) {
        return 1;
    }

    card_3 = create_card(3, symbol_3);
    if (card_3 == NULL) {
        return 1;
    }

    card_4 = create_card(4, symbol_4);
    if (card_4 == NULL) {
        return 1;
    }

    card_5 = create_card(5, symbol_5);
    if (card_5 == NULL) {
        return 1;
    }

    card_6 = create_card(6, symbol_6);
    if (card_6 == NULL) {
        return 1;
    }
    
    card_7 = create_card(7, symbol_7);
    if (card_7 == NULL) {
        return 1;
    }

    card_8 = create_card(8, symbol_8);
    if (card_8 == NULL) {
        return 1;
    }

    card_9 = create_card(1, symbol_1);
    if (card_9 == NULL) {
        return 1;
    }

    card_10 = create_card(2, symbol_2);
    if (card_10 == NULL) {
        return 1;
    }

    card_11 = create_card(3, symbol_3);
    if (card_11 == NULL) {
        return 1;
    }

    card_12 = create_card(4, symbol_4);
    if (card_12 == NULL) {
        return 1;
    }

    card_13 = create_card(5, symbol_5);
    if (card_13 == NULL) {
        return 1;
    }

    card_14 = create_card(6, symbol_6);
    if (card_14 == NULL) {
        return 1;
    }
    
    card_15 = create_card(7, symbol_7);
    if (card_15 == NULL) {
        return 1;
    }

    card_16 = create_card(8, symbol_8);
    if (card_16 == NULL) {
        return 1;
    }

    return 0;
}

void (destroy_all_cards)() {
    destroy_card(card_1);
    destroy_card(card_2);
    destroy_card(card_3);
    destroy_card(card_4);
    destroy_card(card_5);
    destroy_card(card_6);
    destroy_card(card_7);
    destroy_card(card_8);
    destroy_card(card_9);
    destroy_card(card_10);
    destroy_card(card_11);
    destroy_card(card_12);
    destroy_card(card_13);
    destroy_card(card_14);
    destroy_card(card_15);
    destroy_card(card_16);
}

void (initialize_board)(Card *board[4][4], unsigned int board_h_v_size) {
    unsigned int num_cards = board_h_v_size * board_h_v_size;
    Card *cards[16] = {card_1, card_2, card_3, card_4, card_5, card_6, card_7, card_8, card_9, card_10, card_11, card_12, card_13, card_14, card_15, card_16};
    unsigned int card_counts[16] = {0}; 

    for (unsigned int i = 0; i < board_h_v_size; i++) {
        for (unsigned int j = 0; j < board_h_v_size; j++) {
            board[i][j] = NULL;
        }
    }

    for (unsigned int i = 0; i < num_cards; i++) {
        unsigned int card_index;

        do {
            card_index = rand() % num_cards;
        } while (card_counts[card_index] == 1);
        card_counts[card_index]++;

        unsigned int row, col;

        do {
            row = rand() % board_h_v_size;
            col = rand() % board_h_v_size;
        } while (board[row][col] != NULL);

        board[row][col] = cards[card_index];
    }
}

int (display_number)(int number, int x, int y) {
    if (number == 0) {
        if (display_xpm(number_0_xpm, x, y)) {
            return 1;
        }
    }

    while (number > 0) {
        switch (number % 10) {
            case 0:
                if (display_xpm(number_0_xpm, x, y)) {
                    return 1;
                }
                break;
            case 1:
                if (display_xpm(number_1_xpm, x, y)) {
                    return 1;
                }
                break;
            case 2:
                if (display_xpm(number_2_xpm, x, y)) {
                    return 1;
                }
                break;
            case 3:
                if (display_xpm(number_3_xpm, x, y)) {
                    return 1;
                }
                break;
            case 4:
                if (display_xpm(number_4_xpm, x, y)) {
                    return 1;
                }
                break;        
            case 5:
                if (display_xpm(number_5_xpm, x, y)) {
                    return 1;
                }
                break;        
            case 6:
                if (display_xpm(number_6_xpm, x, y)) {
                    return 1;
                }
                break;        
            case 7:
                if (display_xpm(number_7_xpm, x, y)) {
                    return 1;
                }
                break;
            case 8:
                if (display_xpm(number_8_xpm, x, y)) {
                    return 1;
                }
                break;        
            case 9:
                if (display_xpm(number_9_xpm, x, y)) {
                    return 1;
                }
                break;        
            default:
                return 1;
        }
        
        number /= 10;
        x -= 17;
    }
    
    return 0;
}

int (display_string)(char *text, int x, int y) {

    if (text == NULL) {
        return 0;
    }    

    for (int i = 0; i < 20; i++) {

        if (text[i] == '\0') {
            break;
        }

        switch(text[i]) {
            case 'a': 
            case 'A':
                if (display_xpm(letter_a_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'b': 
            case 'B':
                if (display_xpm(letter_b_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'c': 
            case 'C':
                if (display_xpm(letter_c_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'd': 
            case 'D':
                if (display_xpm(letter_d_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'e': 
            case 'E':
                if (display_xpm(letter_e_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'f': 
            case 'F':
                if (display_xpm(letter_f_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'g': 
            case 'G':
                if (display_xpm(letter_g_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'h': 
            case 'H':
                if (display_xpm(letter_h_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'i': 
            case 'I':
                if (display_xpm(letter_i_xpm, x, y)) {
                    return 1;
                }

                break;
            case 'j': 
            case 'J':
                if (display_xpm(letter_j_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'k': 
            case 'K':
                if (display_xpm(letter_k_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'l': 
            case 'L':
                if (display_xpm(letter_l_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'm': 
            case 'M':
                if (display_xpm(letter_m_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'n': 
            case 'N':
                if (display_xpm(letter_n_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'o': 
            case 'O':
                if (display_xpm(letter_o_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'p': 
            case 'P':
                if (display_xpm(letter_p_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'q': 
            case 'Q':
                if (display_xpm(letter_q_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'r': 
            case 'R':
                if (display_xpm(letter_r_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 's': 
            case 'S':
                if (display_xpm(letter_s_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 't': 
            case 'T':
                if (display_xpm(letter_t_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'u': 
            case 'U':
                if (display_xpm(letter_u_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'v': 
            case 'V':
                if (display_xpm(letter_v_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'x': 
            case 'X':
                if (display_xpm(letter_x_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'w': 
            case 'W':
                if (display_xpm(letter_w_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case 'y': 
            case 'Y':
                if (display_xpm(letter_y_xpm, x, y)) {
                    return 1;
                } 
                
                break;
            case 'z': 
            case 'Z':
                if (display_xpm(letter_z_xpm, x, y)) {
                    return 1;
                }
                
                break;
            case ' ':
                
                break;
            case '/':
                if (display_xpm(f_slash_xpm, x, y)) {
                    return 1;
                }

                break;
            case ':':
                if (display_xpm(colon_xpm, x, y)) {
                    return 1;
                }

                break;   
            case '0':
                if (display_xpm(number_0_xpm, x, y)) {
                    return 1;
                }

                break;   
            case '1':
                if (display_xpm(number_1_xpm, x, y)) {
                    return 1;
                }

                break;             
            case '2':
                if (display_xpm(number_2_xpm, x, y)) {
                    return 1;
                }

                break;
            case '3':
                if (display_xpm(number_3_xpm, x, y)) {
                    return 1;
                }

                break;
            case '4':
                if (display_xpm(number_4_xpm, x, y)) {
                    return 1;
                }  

                break;
            case '5':
                if (display_xpm(number_5_xpm, x, y)) {
                    return 1;
                }

                break;
            case '6':
                if (display_xpm(number_6_xpm, x, y)) {
                    return 1;
                }      

                break;
            case '7':
                if (display_xpm(number_7_xpm, x, y)) {
                    return 1;
                }   

                break;
            case '8':
                if (display_xpm(number_8_xpm, x, y)) {
                    return 1;
                }                
                break;
            case '9':
                if (display_xpm(number_9_xpm, x, y)) {
                    return 1;
                }                
                break;                    

            default:
                break;
        }

        x += 20;
    }

    return 0;
}
