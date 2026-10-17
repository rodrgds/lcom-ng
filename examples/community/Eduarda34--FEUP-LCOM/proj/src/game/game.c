#include <lcom/lcf.h>
#include "game.h"


#define CELL_SIZE (SEGMENT_SIZE + SEGMENT_SPACING)

#define SCORE_CELL_X 1 
#define SCORE_CELL_Y 1

StateGame game_st = GAME_RUNNING;

void (game_init)() {
    clear_screen();
    score_reset();
    snake_init(mode_info.XResolution, mode_info.YResolution);
    tongue_init();
    generate_apple_position();
    apple_eaten = false;
    draw_apple();
}

void update_timer_state() {
    uint32_t timer_freq = sys_hz();
    uint32_t ticks_per_frame;
    ticks_per_frame = timer_freq / 60; // 60 FPS

    timer_int_handler();
    if (timer_interrupt_count % ticks_per_frame == 0) {
        timer_interrupt_count = 0;

        static int frame_counter = 0;
        frame_counter++;

        int snake_speed = 10;

        if (frame_counter >= snake_speed) {
            frame_counter = 0;

            clear_screen();
            snake_update();

            if (snake_check_collision_with_self()) {
                game_st = GAME_OVER;
                return;
            }

            if (snake_check_collision_with_apple(apple_position)) {
                apple_eaten = true;
                snake_grow();
            }

            if (apple_eaten) {
                generate_apple_position();
                apple_eaten = false;
            }

            int score_x = SCORE_CELL_X * CELL_SIZE;
            int score_y = SCORE_CELL_Y * CELL_SIZE;

            snake_draw();
            draw_apple();
            tongue_update();
            tongue_draw(mode_info.XResolution, mode_info.YResolution);
            draw_score(get_score(), score_x, score_y);
        }
        swap_buffer();          
    }
}

void update_keyboard_state() {
    kbc_ih();

    switch (scancode) {
        case W_CODE:
        case UP_ARROW:
            if (current_direction != DOWN)
                snake_change_direction(UP);
            break;
        case S_CODE:
        case DOWN_ARROW:
            if (current_direction != UP)
                snake_change_direction(DOWN);
            break;
        case A_CODE:
        case LEFT_ARROW:
            if (current_direction != RIGHT)
                snake_change_direction(LEFT);
            break;
        case D_CODE:
        case RIGHT_ARROW:
            if (current_direction != LEFT)
                snake_change_direction(RIGHT);
            break;
        case BREAK_ESC:
            scancode = BREAK_ESC;
            game_st = GAME_OVER;
            break;
        default:
            break;
    }
}

void update_mouse_state() {
    mouse_ih();
    mouse_sync_packet();
    if (byte_index == 3) {
        mouse_packet_handler();
        snake_handle_mouse_movement(mouse_packet.delta_x, mouse_packet.delta_y);
        tongue_handle_mouse(mouse_packet.lb);
    }
}
