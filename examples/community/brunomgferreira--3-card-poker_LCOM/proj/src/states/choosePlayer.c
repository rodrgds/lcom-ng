#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include "choosePlayer.h"

extern uint8_t *double_buffer;
extern uint8_t *choose_player_buffer;

extern uint8_t scancode[];
extern int scancode_size;

extern struct packet mPacket;
extern struct mouse_position mPosition;

extern time_info rtc_info;

extern main_state_enum main_state;

extern bool mouse_clickable;

static MenuOption options[] = {
    {378, 210, 400, 59},
    {248, 340, 660, 60},
    {248, 410, 660, 60},
    {248, 480, 660, 60},
    {248, 550, 660, 60},
    {248, 620, 660, 60},
};

static uint8_t number_of_options = 6;
static uint8_t selected = 0;

static uint8_t player_shown = 0;

extern struct Player players[];
extern int num_players;
extern struct Player *selected_player;

bool scrolling = false;
int scroll_pos = 0;

void (handle_kbd_choose_player)() {
    switch (scancode[scancode_size -1]) {
        case 0x81: // handle ESC key
            player_shown = 0;
            selected = 0;
            mouse_clickable = false;
            main_state = MENU;
            break;

        case 0x1C: // handle ENTER key
            switch (selected) {
                case 0:
                    mouse_clickable = false;
                    main_state = CREATE_PLAYER;
                    break;
                default:
                    select_player(&players[player_shown + selected - 1]);
                    break;
            }
            break;
        
        case 0x48: // handle UP arrow key
            if (selected > 0) {
                selected--;
                if (selected < 1 && player_shown > 0) {
                    player_shown--;
                    selected = 1;
                }
                scroll_pos = (player_shown * SCROLLER_HEIGHT) / (num_players - 5);
            }
            break;

        case 0x50: // handle DOWN arrow key
            if (selected < number_of_options && selected <= num_players) {
                selected++;
                if (selected > 5 && player_shown <= num_players - 5) {
                    player_shown++;
                    selected--;
                }
                if ((num_players >= 5 && player_shown > num_players - 5) || 
                    (num_players < 5 && selected > num_players)) {
                    player_shown--;
                }
                scroll_pos = (player_shown * SCROLLER_HEIGHT) / (num_players - 5);
            }
            break;

        default:
            break;
    }
}

void (handle_mouse_choose_player)() {
    mouse_clickable = false;

    if (mPacket.lb == 1) {
        if (is_on_scroller_thumb() && num_players > 5) {
            scrolling = true;
            mouse_clickable = true;
        } else if (is_on_scroller() && num_players > 5) {
            mouse_clickable = true;
            update_scroller_position();
        }
    } else {
        scrolling = false;
    }

    if (scrolling) {
        mouse_clickable = true;
    }

    if (is_on_scroller_thumb() && num_players > 5) {
        mouse_clickable = true;
    }

    if (is_on_scroller() && num_players > 5) {
        mouse_clickable = true;
    }

    if (is_on_close_button()) {
        mouse_clickable = true;
    }

    handle_choose_player_mouse_movement();

    if (mPacket.lb == 1) {
        handle_choose_player_mouse_click();
    }
}

void (handle_timer_choose_player)() {
    memcpy(double_buffer, choose_player_buffer, XResolution * YResolution * BytesPerPixel);

    draw_sprite(close_button);

    // Draw each player as a menu option
    draw_players();

    // Draw the selector rectangle
    MenuOption selected_option = options[selected];
    if (selected == 0) {
        draw_menu_option_selector(selected_option.x, selected_option.y, selected_option.width, selected_option.height, false);
    } else {
        draw_menu_option_selector(selected_option.x, selected_option.y, selected_option.width, selected_option.height, true);
    }
    

    wordDecoder("LCOM2024", 500, 755);

    if (num_players > 5) draw_scroller();
}

bool (is_on_scroller)() {
    return mPosition.x >= SCROLLER_X - 10 && mPosition.x <= SCROLLER_X + SCROLLER_WIDTH + 10 &&
           mPosition.y >= SCROLLER_Y - 10 && mPosition.y <= SCROLLER_Y + SCROLLER_HEIGHT + 10;
}

bool (is_on_scroller_thumb)() {
    int thumb_y = SCROLLER_Y + (SCROLLER_HEIGHT * player_shown) / (num_players - 5);
    return mPosition.x >= SCROLLER_X - (SCROLLER_THUMB_SIZE / 2)
        && mPosition.x <= SCROLLER_X + SCROLLER_WIDTH + (SCROLLER_THUMB_SIZE / 2)
        && mPosition.y >= thumb_y - (SCROLLER_THUMB_SIZE / 2)
        && mPosition.y <= thumb_y + (SCROLLER_THUMB_SIZE * 3 / 2);
}

void (update_scroller_position)() {
    player_shown = (mPosition.y - SCROLLER_Y + (SCROLLER_HEIGHT / ((num_players - 5) * 2))) * ((num_players - 5) / SCROLLER_HEIGHT);
    scroll_pos = mPosition.y - SCROLLER_Y;
    if (player_shown < 0) player_shown = 0;
    if (player_shown > num_players - 5) player_shown = num_players - 5;
    if (scroll_pos < 0) scroll_pos = 0;
    if (scroll_pos > SCROLLER_HEIGHT) scroll_pos = SCROLLER_HEIGHT;
}

void (handle_choose_player_mouse_click)() {
    if (!scrolling && is_on_close_button()) {
        player_shown = 0;
        selected = 0;
        mouse_clickable = false;
        main_state = MENU;
    }

    if (!scrolling && is_point_on_menu_option(options[0], mPosition.x, mPosition.y)) {
        mouse_clickable = false;
        main_state = CREATE_PLAYER;
    }

    for (uint8_t i = 1; i < number_of_options && i <= num_players; i++) {
        if (!scrolling && is_point_on_menu_option(options[i], mPosition.x, mPosition.y)) {
            select_player(&players[player_shown + i - 1]);
        }
    }
}

void (handle_choose_player_mouse_movement)() {
    if (scrolling) {
        mouse_clickable = true;
        update_scroller_position();
    } else {
        for (uint8_t i = 0; i < number_of_options && i <= num_players; i++) {
            if (is_point_on_menu_option(options[i], mPosition.x, mPosition.y)) {
                mouse_clickable = true;
                selected = i;
            }
        }
    }
}

void (select_player)(struct Player *player) {
    selected_player = player;
    if (strcmp(time_left_to_target_time(selected_player->nextRewardDataTime), "Get reward") == 0) {
        selected_player->coins += REWARD;
        selected_player->nextRewardDataTime = rtc_info;
        increment_time_by_one_day(&selected_player->nextRewardDataTime);
    }
    if (selected_player->coins < 2) {
        return;
    }
    player_shown = 0;
    selected = 0;
    mouse_clickable = false;
    main_state = GAME;
}

void (draw_players)() {
    // Draw each player as a menu option
    for (int i = 1; i < number_of_options && i <= num_players; i++) {
        char coins_str[12];
        snprintf(coins_str, sizeof(coins_str), "%u", players[player_shown + i - 1].coins);

        draw_text_container(options[i].x + 4,
                            options[i].y + 4,
                            options[i].width - 7,
                            options[i].height - 7,
                            0xfee4ad);

        wordDecoder(players[player_shown + i - 1].name,
                    options[i].x + 15,
                    options[i].y + 22);
        wordDecoder(coins_str,
                    options[i].x + 225,
                    options[i].y + 22);
        wordDecoder(time_left_to_target_time(players[player_shown + i - 1].nextRewardDataTime),
                    options[i].x + 445,
                    options[i].y + 22);
    }
}

void (draw_scroller)() {
    int scroller_thumb_y = SCROLLER_Y + scroll_pos - (SCROLLER_THUMB_SIZE / 2);

    // Draw scroller bar
    vg_draw_rectangle(SCROLLER_X,
                    SCROLLER_Y,
                    SCROLLER_WIDTH,
                    SCROLLER_HEIGHT,
                    0x136b51);

    vg_draw_rectangle(SCROLLER_X - 4,
                    SCROLLER_Y - 4,
                    SCROLLER_WIDTH + 8,
                    4,
                    0x000000);
    vg_draw_rectangle(SCROLLER_X - 4,
                    SCROLLER_Y + SCROLLER_HEIGHT,
                    SCROLLER_WIDTH + 8,
                    4,
                    0x000000);
    vg_draw_rectangle(SCROLLER_X - 4,
                    SCROLLER_Y - 4,
                    4,
                    SCROLLER_HEIGHT + 8,
                    0x000000);
    vg_draw_rectangle(SCROLLER_X + SCROLLER_WIDTH,
                    SCROLLER_Y - 4,
                    4,
                    SCROLLER_HEIGHT + 8,
                    0x000000);

    // Draw scroller thumb
    vg_draw_rectangle(SCROLLER_THUMB_X,
                    scroller_thumb_y,
                    SCROLLER_THUMB_SIZE,
                    SCROLLER_THUMB_SIZE,
                    0xa73b26);
    vg_draw_rectangle(SCROLLER_THUMB_X,
                    scroller_thumb_y,
                    SCROLLER_THUMB_SIZE,
                    4,
                    0x000000);
    vg_draw_rectangle(SCROLLER_THUMB_X,
                    scroller_thumb_y + SCROLLER_THUMB_SIZE - 4,
                    SCROLLER_THUMB_SIZE,
                    4,
                    0x000000);
    vg_draw_rectangle(SCROLLER_THUMB_X,
                    scroller_thumb_y,
                    4,
                    SCROLLER_THUMB_SIZE,
                    0x000000);
    vg_draw_rectangle(SCROLLER_THUMB_X + SCROLLER_THUMB_SIZE - 4,
                    scroller_thumb_y,
                    4,
                    SCROLLER_THUMB_SIZE,
                    0x000000);
}
