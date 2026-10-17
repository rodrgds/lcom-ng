#pragma once

enum program_state { MAIN_MENU, LEADERBOARDS, PICK_USER_NAME, WAIT_PLAYER_TWO, PLAYING, PAUSE_GAME, WAIT_WINNER_TWO, END_GAME, QUIT_PROGRAM};
enum online_state { OFFLINE = 0, ONLINE = 1, ON_WAIT = 2, ON_PLAY = 3 };
enum size_print_text { SMALL = 1, MEDIUM = 2, LARGE = 3};
enum direction { MOVING_UP, MOVING_DOWN, MOVING_LEFT, MOVING_RIGHT, NOT_MOVING };
typedef enum { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT } Alignment;

#define BIT(n) (0x01<<(n))
