#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>

#define KEYBOARD_ID 'k'
#define ROPE_ID 'r'
#define CHANGE_ID 'x'
#define CONNECT_ID 'c'
#define CONNECT_ACK 0xFA
#define CONNECT_START 0xF9
#define CONNECT_READY 0xFB
#define GAME_OVER_ID 0x10
#define RESUME_GAME 0x11
#define RESTART_GAME 0x12
#define EXIT_GAME 0x13
#define NEXT_LEVEL 0x14
#define PAUSE_GAME 0x15

void send_keyboard_key(keyboard_key key);
void send_destroyed_rope(uint8_t x, uint8_t y);
void send_start_connection();
void send_accept_connection();
void send_ready_connection();
void send_game_over();
void send_resume_game();
void send_restart_game();
void send_exit_game();
void send_next_level();
void send_pause_game();
bool check_connection();
bool check_ready();

#endif
