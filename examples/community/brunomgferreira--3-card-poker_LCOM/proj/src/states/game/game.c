#include <lcom/lcf.h>
#include "game.h"

extern uint32_t timer_counter;

extern uint8_t *double_buffer;
extern uint8_t *game_buffer;

extern uint8_t scancode[];
extern int scancode_size;

extern struct packet mPacket;
extern struct mouse_position mPosition;

extern main_state_enum main_state;

extern bool mouse_clickable;

game_state_enum game_state = GAME_INITIALIZE;
static game_state_enum decision = GAME_PLAY_HAND;

Card deck[DECK_SIZE];

HandType dealer_hand_type;
HandType player_hand_type;

int bet = 10;
int aux_number_of_coins;
uint32_t time_left = INITIAL_COUNTDOWN_TIME_LEFT;

bool sliding = false;

extern struct Player *selected_player;

void (handle_kbd_game)() {
    switch (scancode[scancode_size -1]) {
        case 0x81: // handle ESC key
            handle_game_go_back();
            break;

        case 0x1C: // handle ENTER key
            if (game_state == GAME_NO_COINS) handle_game_go_back();
            handle_game_play_hand();
            break;

        case 0x0E: // handle Backspace key
            handle_game_fold_hand();
            break;
        
        case 0x4d: // handle right arrow key
            if (game_state == GAME_BET) increase_bet(10);
            break;

        case 0x4b: // handle left arrow keyd
            if (game_state == GAME_BET) decrease_bet(10);
            break;

        default:
            break;
    }
}

void (handle_mouse_game)() {
    mouse_clickable = false;

    handle_game_mouse_slider();

    handle_game_mouse_hover();

    if (mPacket.lb == 1) {
        handle_game_mouse_click();
    }
}

void (handle_timer_game)() {
    memcpy(double_buffer, game_buffer, XResolution * YResolution * BytesPerPixel);
    draw_sprite(deal_deck);
    draw_sprite(close_button);
    draw_player_name();
    draw_number_of_coins();

    switch (game_state) {
        case GAME_INITIALIZE:
            draw_game_initialize();
            break;

        case GAME_DEAL_CARDS:
            draw_game_deal_cards();
            break;

        case GAME_BET:
            draw_game_bet();
            break;
        
        case GAME_SHOW_PLAYER_HAND:
            draw_game_show_player_hand();
            break;

        case GAME_DECIDE:
            draw_game_decide();
            break;

        case GAME_SHOW_DEALER_HAND:
            draw_game_show_dealer_hand();
            break;

        case GAME_PLAY_HAND:
            draw_game_play_hand();
            break;

        case GAME_FOLD_HAND:
            draw_game_fold_hand();
            break;

        case GAME_WIN:
            draw_game_win();
            break;

        case GAME_LOSE:
            draw_game_lose();
            break;

        case GAME_DRAW:
            draw_game_draw();
            break;

        case GAME_REMOVE_CARDS:
            draw_game_remove_cards();
            break;

        case GAME_NO_COINS:
            draw_game_no_coins();
            break;
    }
}

void (handle_game_go_back)() {
    selected_player->coins = aux_number_of_coins;
    write_player_data_to_file();
    game_state = GAME_INITIALIZE;
    main_state = MENU;
}

void (handle_game_fold_hand)() {
    switch (game_state) {
        case GAME_DECIDE:
            decision = GAME_FOLD_HAND;
            game_state = GAME_SHOW_DEALER_HAND;
            break;
        default:
            break;
    }
}

void (handle_game_play_hand)() {
    switch (game_state) {
        case GAME_BET:
            aux_number_of_coins = selected_player->coins - bet;
            bet *= 2;
            game_state = GAME_SHOW_PLAYER_HAND;
            break;
        case GAME_DECIDE:
            decision = GAME_PLAY_HAND;
            game_state = GAME_SHOW_DEALER_HAND;
            break;
        default:
            break;
    }
}

void (handle_game_mouse_hover)() {
    switch (game_state) {
        case GAME_BET:
            if (is_on_bet_slider_thumb()) {
                mouse_clickable = true;
            }
            if (is_on_bet_slider_area()) {
                mouse_clickable = true;
            }
            if (is_on_play_button()) {
                mouse_clickable = true;
            }
            if (is_on_decrease_bet_button()) {
                mouse_clickable = true;
            }
            if (is_on_increase_bet_button()) {
                mouse_clickable = true;
            }
            break;
        case GAME_DECIDE:
            if (is_on_fold_button()) {
                mouse_clickable = true;
            }
            if (is_on_play_button()) {
                mouse_clickable = true;
            }
            break;
        default:
            break;
    }

    if (is_on_close_button()) {
        mouse_clickable = true;
    }
}

void (handle_game_mouse_click)() {
    if (!sliding && is_on_fold_button()) {
        handle_game_fold_hand();
    }
    if (!sliding && is_on_play_button()) {
        handle_game_play_hand();
    }
    if (!sliding && is_on_decrease_bet_button()) {
        decrease_bet(10);
    }
    if (!sliding && is_on_increase_bet_button()) {
        increase_bet(10);
    }
    if (!sliding && is_on_close_button()) {
        handle_game_go_back();
    }
}

void (handle_game_mouse_slider)() {
    if (game_state == GAME_BET && mPacket.lb == 1) {
        if (is_on_bet_slider_thumb()) {
            sliding = true;
        } else if (is_on_bet_slider_area()) {
            bet = (mPosition.x - SLIDER_X + (SLIDER_THUMB_SIZE / 2)) * ((selected_player->coins / 2) / SLIDER_WIDTH);
            validate_bet();
        }
    } else {
        sliding = false;
    }

    if (sliding) {
        mouse_clickable = true;
        update_bet_if_sliding();
    }
}

bool (is_on_bet_slider_thumb)() {
    int slider_thumb_x = SLIDER_X + (int)((SLIDER_WIDTH * bet) / (selected_player->coins / 2)) - SLIDER_THUMB_SIZE / 2;
    return mPosition.x >= slider_thumb_x
        && mPosition.x <= slider_thumb_x + SLIDER_THUMB_SIZE
        && mPosition.y >= SLIDER_Y - SLIDER_THUMB_SIZE / 2
        && mPosition.y <= SLIDER_Y + SLIDER_THUMB_SIZE / 2;
}

bool (is_on_bet_slider_area)() {
    return mPosition.x >= SLIDER_X - (SLIDER_THUMB_SIZE / 2)
        && mPosition.x <= SLIDER_X + SLIDER_WIDTH + (SLIDER_THUMB_SIZE / 2)
        && mPosition.y >= SLIDER_Y - (SLIDER_THUMB_SIZE / 2)
        && mPosition.y <= SLIDER_Y + SLIDER_HEIGHT + (SLIDER_THUMB_SIZE / 2);
}

bool (is_on_fold_button)() {
    return mPosition.x >= FOLD_BTN_X
        && mPosition.x <= FOLD_BTN_X + FOLD_BTN_WIDTH
        && mPosition.y >= FOLD_BTN_Y
        && mPosition.y <= FOLD_BTN_Y + FOLD_BTN_HEIGHT;
}

bool (is_on_play_button)() {
    return mPosition.x >= PLAY_BTN_X
        && mPosition.x <= PLAY_BTN_X + PLAY_BTN_WIDTH
        && mPosition.y >= PLAY_BTN_Y
        && mPosition.y <= PLAY_BTN_Y + PLAY_BTN_HEIGHT;
}

bool (is_on_decrease_bet_button)() {
    return mPosition.x >= DECREASE_BET_BTN_X
        && mPosition.x <= DECREASE_BET_BTN_X + DECREASE_BET_BTN_WIDTH
        && mPosition.y >= DECREASE_BET_BTN_Y
        && mPosition.y <= DECREASE_BET_BTN_Y + DECREASE_BET_BTN_HEIGHT;
}

bool (is_on_increase_bet_button)() {
    return mPosition.x >= INCREASE_BET_BTN_X
        && mPosition.x <= INCREASE_BET_BTN_X + INCREASE_BET_BTN_WIDTH
        && mPosition.y >= INCREASE_BET_BTN_Y
        && mPosition.y <= INCREASE_BET_BTN_Y + INCREASE_BET_BTN_HEIGHT;
}

void (update_bet_if_sliding)() {
    bet = (mPosition.x - SLIDER_X + (SLIDER_THUMB_SIZE / 2)) * ((selected_player->coins / 2) / SLIDER_WIDTH);
    validate_bet();
}

void (validate_number_of_coins)() {
    if (selected_player->coins < 2) {
        game_state = GAME_NO_COINS;
    }
}

void (validate_bet)() {
    if (bet > (selected_player->coins / 2)) {
        bet = (selected_player->coins / 2);
    }
    if (bet < 1) {
        bet = 1;
    }
}

void (decrease_bet)(int value) {
    bet -= value;
    validate_bet();
}

void (increase_bet)(int value) {
    bet += value;
    validate_bet();
}

void (countdown_timer)() {
    if (timer_counter % 60 == 0) { 
        if (time_left > 0) {
            time_left--;
        }
    }

    if (time_left == 0) {
        selected_player->coins = aux_number_of_coins;
        game_state = GAME_REMOVE_CARDS;
    }
}

void (increase_number_of_coins)() {
    int sum = bet / (60 * time_left);
    if (sum == 0 && selected_player->coins < aux_number_of_coins) sum = 1;

    selected_player->coins += sum;
    
    if (selected_player->coins > aux_number_of_coins) {
        selected_player->coins = aux_number_of_coins;
    }
}

void (decrease_number_of_coins)() {
    int sum = bet / (60 * time_left);
    if (sum == 0 && selected_player->coins > aux_number_of_coins) sum = 1;

    selected_player->coins -= sum;

    if (selected_player->coins < aux_number_of_coins) {
        selected_player->coins = aux_number_of_coins;
    }
}

void (decrease_number_of_coins_bet)() {
    int sum = bet * (1-player_card_3->xScale);
    if (sum == 0 && selected_player->coins > aux_number_of_coins) sum = 1;

    selected_player->coins -= sum;

    if (selected_player->coins < aux_number_of_coins) {
        selected_player->coins = aux_number_of_coins;
    }
}

void (draw_dealer_hand)() {
    switch (game_state) {
        case GAME_WIN:
        case GAME_LOSE:
        case GAME_DRAW:
        case GAME_FOLD_HAND:
        case GAME_PLAY_HAND:
        case GAME_SHOW_DEALER_HAND:
            set_animSprite_position(dealer_card_1, DEALER_CARD_1_X, DEALER_CARD_1_Y);
            set_animSprite_position(dealer_card_2, DEALER_CARD_2_X, DEALER_CARD_2_Y);
            set_animSprite_position(dealer_card_3, DEALER_CARD_3_X, DEALER_CARD_3_Y);
            draw_animSprite(dealer_card_1, 1);
            draw_animSprite(dealer_card_2, 1);
            draw_animSprite(dealer_card_3, 1);

            draw_dealer_hand_type();
            break;
        default:
            set_animSprite_position(dealer_card_1, DEALER_CARD_1_X, DEALER_CARD_1_Y);
            set_animSprite_position(dealer_card_2, DEALER_CARD_2_X, DEALER_CARD_2_Y);
            set_animSprite_position(dealer_card_3, DEALER_CARD_3_X, DEALER_CARD_3_Y);
            draw_animSprite(dealer_card_1, 0);
            draw_animSprite(dealer_card_2, 0);
            draw_animSprite(dealer_card_3, 0);
            break;
    }

}

void (draw_player_hand)() {
    switch (game_state) {
        case GAME_BET:
            set_animSprite_position(player_card_1, PLAYER_CARD_1_X, PLAYER_CARD_1_Y);
            set_animSprite_position(player_card_2, PLAYER_CARD_2_X, PLAYER_CARD_2_Y);
            set_animSprite_position(player_card_3, PLAYER_CARD_3_X, PLAYER_CARD_3_Y);
            draw_animSprite(player_card_1, 0);
            draw_animSprite(player_card_2, 0);
            draw_animSprite(player_card_3, 0);
            break;
        default:
            set_animSprite_position(player_card_1, PLAYER_CARD_1_X, PLAYER_CARD_1_Y);
            set_animSprite_position(player_card_2, PLAYER_CARD_2_X, PLAYER_CARD_2_Y);
            set_animSprite_position(player_card_3, PLAYER_CARD_3_X, PLAYER_CARD_3_Y);
            draw_animSprite(player_card_1, 1);
            draw_animSprite(player_card_2, 1);
            draw_animSprite(player_card_3, 1);

            draw_player_hand_type();
            break;
    }
    
}

void (flip_dealer_cards)() {
    animSprite_horizontal_flip(dealer_card_1, 0, 1, 0.1);
    animSprite_horizontal_flip(dealer_card_2, 0, 1, 0.1);
    animSprite_horizontal_flip(dealer_card_3, 0, 1, 0.1);
}

void (flip_player_cards)() {
    animSprite_horizontal_flip(player_card_1, 0, 1, 0.1);
    animSprite_horizontal_flip(player_card_2, 0, 1, 0.1);
    animSprite_horizontal_flip(player_card_3, 0, 1, 0.1);
}

void (draw_show_dealer_cards)() {
    flip_dealer_cards();
    if (dealer_card_3->xScale == 1 && dealer_card_3->curr_sprite == 0) {
        game_state = decision;
    }
}

void (draw_show_player_cards)() {
    flip_player_cards();
    if (player_card_3->xScale == 1 && player_card_3->curr_sprite == 0) {
        game_state = GAME_DECIDE;
        handle_game_mouse_hover();
    }
}

void (draw_player_hand_type)() {
    draw_text_container(PLAYER_HAND_TYPE_X,
                        PLAYER_HAND_TYPE_Y,
                        PLAYER_HAND_TYPE_WIDTH,
                        PLAYER_HAND_TYPE_HEIGHT,
                        0xffa96a);

    wordDecoder(handtype_to_string(player_hand_type), 
                PLAYER_HAND_TYPE_X + 15,
                PLAYER_HAND_TYPE_Y + 12);
}

void (draw_dealer_hand_type)() {
    draw_text_container(DEALER_HAND_TYPE_X,
                        DEALER_HAND_TYPE_Y,
                        DEALER_HAND_TYPE_WIDTH,
                        DEALER_HAND_TYPE_HEIGHT,
                        0xffa96a);

    wordDecoder(handtype_to_string(dealer_hand_type),
                DEALER_HAND_TYPE_X + 15,
                DEALER_HAND_TYPE_Y + 12);
}

void (draw_player_name)() {
    draw_text_container(PLAYER_NAME_X,
                        PLAYER_NAME_Y,
                        PLAYER_NAME_WIDTH,
                        PLAYER_NAME_HEIGHT,
                        0xf3884b);

    wordDecoder(selected_player->name,
                PLAYER_NAME_X + 15,
                PLAYER_NAME_Y + 12);
}

void (draw_number_of_coins)() {
    char coins_str[12];
    snprintf(coins_str, sizeof(coins_str), "%u", selected_player->coins);

    draw_text_container(NUMBER_OF_COINS_X,
                        NUMBER_OF_COINS_Y,
                        NUMBER_OF_COINS_WIDTH,
                        NUMBER_OF_COINS_HEIGHT,
                        0xf3884b);

    wordDecoder(coins_str,
                NUMBER_OF_COINS_X + 15,
                NUMBER_OF_COINS_Y + 12);
}

void (draw_bet)() {
    char coins_str[12];
    snprintf(coins_str, sizeof(coins_str), "%u", bet);

    draw_text_container(BET_X,
                        BET_Y,
                        BET_WIDTH,
                        BET_HEIGHT,
                        0xffa96a);

    wordDecoder(coins_str, 
                BET_X + 15,
                BET_Y + 12);
}

void (draw_message)(char message[]) {
    draw_text_container(MESSAGE_X,
                        MESSAGE_Y,
                        MESSAGE_WIDTH,
                        MESSAGE_HEIGHT,
                        0xfee4ad);

    wordDecoder(message,
                MESSAGE_X + 15,
                MESSAGE_Y + 12);
}

void (draw_slider)() {
    int slider_thumb_x = SLIDER_X + (int)((SLIDER_WIDTH * bet) / (selected_player->coins / 2)) - SLIDER_THUMB_SIZE / 2;
    int slider_thumb_y = SLIDER_Y - SLIDER_HEIGHT;

    // Draw slider bar
    vg_draw_rectangle(SLIDER_X, SLIDER_Y, (int) SLIDER_WIDTH, SLIDER_HEIGHT, 0xfee4ad);

    // Draw slider thumb
    vg_draw_rectangle(slider_thumb_x,
                    slider_thumb_y,
                    SLIDER_THUMB_SIZE,
                    SLIDER_THUMB_SIZE,
                    0xa73b26);
    vg_draw_rectangle(slider_thumb_x,
                    slider_thumb_y,
                    SLIDER_THUMB_SIZE,
                    4,
                    0x000000);
    vg_draw_rectangle(slider_thumb_x,
                    slider_thumb_y + SLIDER_THUMB_SIZE - 4,
                    SLIDER_THUMB_SIZE,
                    4,
                    0x000000);
    vg_draw_rectangle(slider_thumb_x,
                    slider_thumb_y,
                    4,
                    SLIDER_THUMB_SIZE,
                    0x000000);
    vg_draw_rectangle(slider_thumb_x + SLIDER_THUMB_SIZE - 4,
                    slider_thumb_y,
                    4,
                    SLIDER_THUMB_SIZE,
                    0x000000);
    
    // Draw decrease & increase bet buttons
    draw_decrease_bet_button();
    draw_increase_bet_button();
    
}

void (draw_decrease_bet_button)() {
    draw_text_container(DECREASE_BET_BTN_X,
                        DECREASE_BET_BTN_Y,
                        DECREASE_BET_BTN_WIDTH,
                        DECREASE_BET_BTN_HEIGHT,
                        0xa73b26);

    vg_draw_rectangle(DECREASE_BET_BTN_X + 7, 
                    DECREASE_BET_BTN_Y + 18,
                    26,
                    4,
                    0xfee4ad);
}

void (draw_increase_bet_button)() {
    draw_text_container(INCREASE_BET_BTN_X,
                        INCREASE_BET_BTN_Y,
                        INCREASE_BET_BTN_WIDTH,
                        INCREASE_BET_BTN_HEIGHT,
                        0xa73b26);

    vg_draw_rectangle(INCREASE_BET_BTN_X + 7,
                    DECREASE_BET_BTN_Y + 18,
                    26,
                    4,
                    0xfee4ad);

    vg_draw_rectangle(INCREASE_BET_BTN_X + 18,
                    INCREASE_BET_BTN_Y + 7,
                    4,
                    26,
                    0xfee4ad);
}

void (draw_fold_button)() {
    draw_text_container(FOLD_BTN_X,
                        FOLD_BTN_Y,
                        FOLD_BTN_WIDTH,
                        FOLD_BTN_HEIGHT,
                        0xfee4ad);

    wordDecoder("Fold",
                FOLD_BTN_X + 12,
                FOLD_BTN_Y + 12);
}

void (draw_bet_button)() {
    draw_text_container(PLAY_BTN_X,
                        PLAY_BTN_Y,
                        PLAY_BTN_WIDTH,
                        PLAY_BTN_HEIGHT,
                        0xfee4ad);

    wordDecoder("Bet",
                PLAY_BTN_X + 18,
                PLAY_BTN_Y + 12);
}

void (draw_play_button)() {
    draw_text_container(PLAY_BTN_X,
                        PLAY_BTN_Y,
                        PLAY_BTN_WIDTH,
                        PLAY_BTN_HEIGHT,
                        0xfee4ad);

    wordDecoder("Play",
                PLAY_BTN_X + 12,
                PLAY_BTN_Y + 12);
}

void (draw_game_initialize)() {
    set_animSprite_position(dealer_card_1, CARD_INITIAL_X, CARD_INITIAL_Y);
    set_animSprite_position(dealer_card_2, CARD_INITIAL_X, CARD_INITIAL_Y);
    set_animSprite_position(dealer_card_3, CARD_INITIAL_X, CARD_INITIAL_Y);

    set_animSprite_position(player_card_1, CARD_INITIAL_X, CARD_INITIAL_Y);
    set_animSprite_position(player_card_2, CARD_INITIAL_X, CARD_INITIAL_Y);
    set_animSprite_position(player_card_3, CARD_INITIAL_X, CARD_INITIAL_Y);

    set_animSprite_speed(dealer_card_1, -CARD_SPEED_X, CARD_SPEED_Y);
    set_animSprite_speed(dealer_card_2, -CARD_SPEED_X, CARD_SPEED_Y);
    set_animSprite_speed(dealer_card_3, -CARD_SPEED_X, CARD_SPEED_Y);

    set_animSprite_speed(player_card_1, -CARD_SPEED_X, CARD_SPEED_Y);
    set_animSprite_speed(player_card_2, -CARD_SPEED_X, CARD_SPEED_Y);
    set_animSprite_speed(player_card_3, -CARD_SPEED_X, CARD_SPEED_Y);

    create_deck(deck);
    shuffle_deck(deck);

    Card dealerCards[3] = {deck[0], deck[1], deck[2]};
    determine_hand(dealerCards, &dealer_hand_type);

    Card playerCards[3] = {deck[3], deck[4], deck[5]};
    determine_hand(playerCards, &player_hand_type);

    dealer_card_1->map[1] = find_card_sprite(&deck[0]);
    dealer_card_2->map[1] = find_card_sprite(&deck[1]);
    dealer_card_3->map[1] = find_card_sprite(&deck[2]);

    player_card_1->map[1] = find_card_sprite(&deck[3]);
    player_card_2->map[1] = find_card_sprite(&deck[4]);
    player_card_3->map[1] = find_card_sprite(&deck[5]);

    time_left = INITIAL_COUNTDOWN_TIME_LEFT;
    aux_number_of_coins = selected_player->coins;
    bet /= 2;
    validate_bet();
    
    game_state = GAME_DEAL_CARDS;
}

void (draw_game_deal_cards)() {
    draw_animSprite(dealer_card_3, 0);
    draw_animSprite(dealer_card_2, 0);
    draw_animSprite(dealer_card_1, 0);

    move_animSprite(dealer_card_1, DEALER_CARD_1_X, DEALER_CARD_1_Y);
    move_animSprite(dealer_card_2, DEALER_CARD_2_X, DEALER_CARD_2_Y);
    move_animSprite(dealer_card_3, DEALER_CARD_3_X, DEALER_CARD_3_Y);

    draw_animSprite(player_card_1, 0);
    draw_animSprite(player_card_2, 0);
    draw_animSprite(player_card_3, 0);
    
    move_animSprite(player_card_1, PLAYER_CARD_1_X, PLAYER_CARD_1_Y);
    move_animSprite(player_card_2, PLAYER_CARD_2_X, PLAYER_CARD_2_Y);
    move_animSprite(player_card_3, PLAYER_CARD_3_X, PLAYER_CARD_3_Y);

    if (dealer_card_1->x == DEALER_CARD_1_X &&
        dealer_card_1->y == DEALER_CARD_1_Y &&
        dealer_card_2->x == DEALER_CARD_2_X &&
        dealer_card_2->y == DEALER_CARD_2_Y &&
        dealer_card_3->x == DEALER_CARD_3_X &&
        dealer_card_3->y == DEALER_CARD_3_Y &&
        player_card_1->x == PLAYER_CARD_1_X &&
        player_card_1->y == PLAYER_CARD_1_Y &&
        player_card_2->x == PLAYER_CARD_2_X &&
        player_card_2->y == PLAYER_CARD_2_Y &&
        player_card_3->x == PLAYER_CARD_3_X &&
        player_card_3->y == PLAYER_CARD_3_Y) {
        game_state = GAME_BET;
        handle_game_mouse_hover();
    }
}

void (draw_game_bet)() {
    draw_dealer_hand();
    draw_player_hand();
    draw_bet();
    draw_bet_button();
    draw_slider();
}

void (draw_game_show_player_hand)() {
    decrease_number_of_coins_bet();
    draw_dealer_hand();
    draw_show_player_cards();
    draw_bet();
}

void (draw_game_decide)() {
    draw_dealer_hand();
    draw_player_hand();

    // Initialize draw_remove_cards()
    set_animSprite_speed(dealer_card_1, CARD_SPEED_X, -CARD_SPEED_Y);
    set_animSprite_speed(dealer_card_2, CARD_SPEED_X, -CARD_SPEED_Y);
    set_animSprite_speed(dealer_card_3, -CARD_SPEED_X, -CARD_SPEED_Y);

    set_animSprite_speed(player_card_1, CARD_SPEED_X, -CARD_SPEED_Y*2);
    set_animSprite_speed(player_card_2, CARD_SPEED_X, -CARD_SPEED_Y*2);
    set_animSprite_speed(player_card_3, -CARD_SPEED_X, -CARD_SPEED_Y*2);
    
    draw_bet();
    draw_fold_button();
    draw_play_button();
}

void (draw_game_show_dealer_hand)() {
    draw_show_dealer_cards(); // Show dealer cards
    draw_player_hand();

    if (decision == GAME_PLAY_HAND) {
        draw_bet();
    }
}

void (draw_game_play_hand)() {
    draw_dealer_hand();
    draw_player_hand();

    draw_bet();
    
    switch (winning_hand(player_hand_type, dealer_hand_type)) {
        case WIN:
            aux_number_of_coins = selected_player->coins + (int)(bet * 1.5);
            game_state = GAME_WIN;
            break;
        case LOSE:
            aux_number_of_coins = selected_player->coins - (int)(bet / 2.0);
            game_state = GAME_LOSE;
            break;
        case DRAW:
            aux_number_of_coins = selected_player->coins + (int)(bet / 2.0);
            game_state = GAME_DRAW;
            break;
    };
}

void (draw_game_fold_hand)() {
    draw_dealer_hand();
    draw_player_hand();

    switch (winning_hand(player_hand_type, dealer_hand_type)) {
        case WIN:
            draw_message("Would have won!");
            break;
        case LOSE:
            draw_message("Would have lost!");
            break;
        case DRAW:
            draw_message("Would have drawn!");
            break;
    };

    countdown_timer();
}

void (draw_game_win)() {
    draw_dealer_hand();
    draw_player_hand();

    draw_bet();
    draw_message("You won!");

    increase_number_of_coins();
    countdown_timer();
}

void (draw_game_lose)() {
    draw_dealer_hand();
    draw_player_hand();
    
    draw_bet();
    draw_message("You lost!");

    decrease_number_of_coins();
    countdown_timer();
}

void (draw_game_draw)() {
    draw_dealer_hand();
    draw_player_hand();

    draw_bet();
    draw_message("Its a draw!");

    countdown_timer();
}

void (draw_game_remove_cards)() {
    draw_animSprite(dealer_card_3, 1);
    draw_animSprite(dealer_card_2, 1);
    draw_animSprite(dealer_card_1, 1);
    
    draw_animSprite(player_card_1, 1);
    draw_animSprite(player_card_2, 1);
    draw_animSprite(player_card_3, 1);

    if (dealer_card_1->x == CARD_FINAL_X &&
        dealer_card_1->y == CARD_FINAL_Y &&
        dealer_card_2->x == CARD_FINAL_X &&
        dealer_card_2->y == CARD_FINAL_Y &&
        dealer_card_3->x == CARD_FINAL_X &&
        dealer_card_3->y == CARD_FINAL_Y &&
        player_card_1->x == CARD_FINAL_X &&
        player_card_1->y == CARD_FINAL_Y &&
        player_card_2->x == CARD_FINAL_X &&
        player_card_2->y == CARD_FINAL_Y &&
        player_card_3->x == CARD_FINAL_X &&
        player_card_3->y == CARD_FINAL_Y) {
        validate_number_of_coins();
        if (game_state != GAME_NO_COINS) {
            game_state = GAME_INITIALIZE;
        }
    } else {
        move_animSprite(dealer_card_1, CARD_FINAL_X, CARD_FINAL_Y);
        move_animSprite(dealer_card_2, CARD_FINAL_X, CARD_FINAL_Y);
        move_animSprite(dealer_card_3, CARD_FINAL_X, CARD_FINAL_Y);
        
        move_animSprite(player_card_1, CARD_FINAL_X, CARD_FINAL_Y);
        move_animSprite(player_card_2, CARD_FINAL_X, CARD_FINAL_Y);
        move_animSprite(player_card_3, CARD_FINAL_X, CARD_FINAL_Y);
    }
}

void (draw_game_no_coins)() {
    draw_text_container(NO_COINS_X,
                        NO_COINS_Y,
                        NO_COINS_WIDTH,
                        NO_COINS_HEIGHT,
                        0xfee4ad);

    wordDecoder("Not enough funds",
                NO_COINS_X + 15,
                NO_COINS_Y + 12);
}
