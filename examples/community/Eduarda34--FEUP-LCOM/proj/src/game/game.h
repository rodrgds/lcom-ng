#pragma once

#include <lcom/lcf.h>
#include "snake.h"
#include "apple.h"
#include "tongue.h"
#include "score.h"
#include "controller/video/graphics.h"
#include "controller/mouse/mouse.h"
#include "controller/keyboard/keyboard.h"
#include "controller/video/colors.h"
#include "controller/realTimeClock/rtc.h"


extern vbe_mode_info_t mode_info;
extern uint8_t scancode;
extern int timer_interrupt_count;

extern struct packet mouse_packet;
extern uint8_t packet_bytes[3];
extern uint8_t byte_index;

typedef enum {
    GAME_RUNNING,
    GAME_OVER
} StateGame;

/**
 * @brief Initializes the game state and all game components
 */
void (game_init)();

/**
 * @brief Updates the game state on every timer tick
 */
void (update_timer_state)();

/**
 * @brief Handles keyboard input an changes snake direction accordingly
 */
void (update_keyboard_state)();

/**
 * @brief Handles mouse input and updates the snake's direction and tongue 
 * 
 */
void (update_mouse_state)();
