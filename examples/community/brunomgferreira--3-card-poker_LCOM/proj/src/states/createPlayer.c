#include <lcom/lcf.h>
#include "createPlayer.h"

extern uint8_t *double_buffer;
extern uint8_t *create_player_buffer;

extern uint8_t scancode[];
extern int scancode_size;

extern struct packet mPacket;
extern struct mouse_position mPosition;

extern time_info rtc_info;

extern main_state_enum main_state;

extern bool mouse_clickable;

#define MAX_STRING_LENGTH 10
static char input_string[MAX_STRING_LENGTH + 1];
static uint32_t string_length = 0;

extern struct Player players[];
extern int num_players;
extern struct Player *selected_player;

struct Player newPlayer;

void (handle_create_button)() {
    if (string_length > 0) {
        create_new_player();
        main_state = GAME;
    }
}

void (handle_go_back_button)() {
    input_string[0] = '\0';
    string_length = 0;
    main_state = CHOOSE_PLAYER;
}

void (handle_kbd_create_player)() {
    read_kbd_input_str(scancode[scancode_size - 1], input_string, &string_length, MAX_STRING_LENGTH);

    switch (scancode[scancode_size - 1]) {
        case 0x81: // handle ESC key
            handle_go_back_button();
            break;
        case 0x1C: // handle ENTER key
            handle_create_button();
            break;

        default:
            break;
    }
}

bool (is_on_create_player_btn)() {
    return(mPosition.x >= CREATE_PLAYER_BTN_X
        && mPosition.x <= CREATE_PLAYER_BTN_X + CREATE_PLAYER_BTN_WIDTH
        && mPosition.y >= CREATE_PLAYER_BTN_Y
        && mPosition.y <= CREATE_PLAYER_BTN_Y + CREATE_PLAYER_BTN_HEIGHT);
}

void (handle_mouse_create_player)() {
    mouse_clickable = false;
    if (mPacket.lb == 1) {
        if (is_on_close_button()) {
            handle_go_back_button();
        }
        else if (is_on_create_player_btn()) {
            handle_create_button();
        }
    }

    if (is_on_close_button()) {
        mouse_clickable = true;
    }
    if (is_on_create_player_btn()) {
        mouse_clickable = true;
    }
}

void (handle_timer_create_player)() {
    memcpy(double_buffer, create_player_buffer, XResolution * YResolution * BytesPerPixel);

    draw_sprite(close_button);

    wordDecoder(input_string, CREATE_PLAYER_INPUT_X, CREATE_PLAYER_INPUT_Y);
}

void (create_new_player)() {
    strcpy(newPlayer.name, input_string);
    newPlayer.coins = 200;
    newPlayer.nextRewardDataTime = rtc_info;
    
    increment_time_by_one_day(&newPlayer.nextRewardDataTime);
    
    input_string[0] = '\0';
    string_length = 0;

    if (num_players < MAX_PLAYERS) {
        players[num_players] = newPlayer;
        num_players++;
    }
    selected_player = &players[num_players - 1];
}
