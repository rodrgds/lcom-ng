#include "game.h"
#include "digits.c"
#include "../controllers/video/graphics.h"
#include "../controllers/keyboard/i8042.h"
#include "../controllers/keyboard/keyboard.h"
#include "../controllers/timer/timer.c"
#include "../controllers/timer/i8254.h"
#include "../controllers/menu/menu.h"

#define CELL_SIZE       10
#define MAX_SNAKE_LEN   100
#define INIT_SNAKE_LEN  1
#define BORDER_WIDTH    20
#define GAME_AREA_WIDTH  400
#define GAME_AREA_HEIGHT 300

static int curr_game_area_width;
static int curr_game_area_height;
int update_tick_threshold;  
static int game_level = 2;
extern uint8_t kbd_mask;

static uint16_t snake_x[MAX_SNAKE_LEN];
static uint16_t snake_y[MAX_SNAKE_LEN];
static int      snake_len;
static int      dir_x, dir_y;
static uint16_t food_x, food_y;
static int food_timer = 0; 
static int score = 0;

static uint16_t game_start_x;
static uint16_t game_start_y;
static uint16_t game_end_x;
static uint16_t game_end_y;

static bool game_running = true;
static bool score_updated = false;
/**
 * @brief Sets the current game area size and update tick threshold based on level.
 *
 * @param level The game level (1..3), adjusts area dimensions and speed.
 */
void game_set_level(int level) {
  switch(level) {
    case 1:
      curr_game_area_width = 600;
      curr_game_area_height = 450;
      update_tick_threshold = 10; 
      break;
    case 2:
      curr_game_area_width = 400;
      curr_game_area_height = 300;
      update_tick_threshold = 6;  
      break;
    case 3:
      curr_game_area_width = 300;
      curr_game_area_height = 225;
      update_tick_threshold = 4; 
      break;
    default:
      curr_game_area_width = 400;
      curr_game_area_height = 300;
      update_tick_threshold = 6;
      break;
  }
}

/**
 * @brief Initializes the snake and food positions and resets score and timer.
 */
static void init_game(void) {
    snake_len = INIT_SNAKE_LEN;
    dir_x = 1; dir_y = 0;
    
    game_start_x = (mode_info.XResolution - curr_game_area_width) / 2;
    game_start_y = (mode_info.YResolution - curr_game_area_height) / 2;
    game_end_x = game_start_x + curr_game_area_width;
    game_end_y = game_start_y + curr_game_area_height;
    
    uint16_t cols = curr_game_area_width / CELL_SIZE;
    uint16_t rows = curr_game_area_height / CELL_SIZE;
    uint16_t cx = game_start_x + (cols / 2) * CELL_SIZE;
    uint16_t cy = game_start_y + (rows / 2) * CELL_SIZE;
    
    for (int i = 0; i < snake_len; i++) {
        snake_x[i] = cx - i * CELL_SIZE;
        snake_y[i] = cy;
    }
    food_timer = 0;
    score = 0;
}
/**
 * @brief Spawns food at a random cell within the game area.
 */
static void spawn_food(void) {
    uint16_t cols = curr_game_area_width / CELL_SIZE;
    uint16_t rows = curr_game_area_height / CELL_SIZE;
    
    food_x = game_start_x + (rand() % cols) * CELL_SIZE;
    food_y = game_start_y + (rand() % rows) * CELL_SIZE;
    food_timer = 0; 
}
/**
 * @brief Draws the entire game state: background, borders, snake, food and score.
 */
void draw_game(void) {
    vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x333333);
    
    vg_draw_rectangle(game_start_x - BORDER_WIDTH, game_start_y - BORDER_WIDTH, 
                      curr_game_area_width + 2 * BORDER_WIDTH, BORDER_WIDTH, 0x666666);
    vg_draw_rectangle(game_start_x - BORDER_WIDTH, game_start_y + curr_game_area_height, 
                      curr_game_area_width + 2 * BORDER_WIDTH, BORDER_WIDTH, 0x666666);
    vg_draw_rectangle(game_start_x - BORDER_WIDTH, game_start_y, 
                      BORDER_WIDTH, curr_game_area_height, 0x666666);
    vg_draw_rectangle(game_start_x + curr_game_area_width, game_start_y, 
                      BORDER_WIDTH, curr_game_area_height, 0x666666);
    
    vg_draw_rectangle(game_start_x, game_start_y, curr_game_area_width, curr_game_area_height, 0xdddddd);
    
    int title_x = game_start_x + (curr_game_area_width / 2) - 60;
    int title_y = game_start_y - BORDER_WIDTH - 70;
    draw_text(title_x, title_y, "SNAKE GAME", 0xFFFFFF);
    
    draw_score();
    
    vg_draw_rectangle(food_x, food_y, CELL_SIZE, CELL_SIZE, 0xFFFA00);
    for (int i = 0; i < snake_len; i++)
        vg_draw_rectangle(snake_x[i], snake_y[i], CELL_SIZE, CELL_SIZE, 0x00FF00);
}

/**
 * @brief Updates snake position, checks collisions and handles food eating.
 */
void update_game(void) {
    for (int i = snake_len - 1; i > 0; i--) {
        snake_x[i] = snake_x[i-1];
        snake_y[i] = snake_y[i-1];
    }
    snake_x[0] += dir_x * CELL_SIZE;
    snake_y[0] += dir_y * CELL_SIZE;
    
    if (snake_x[0] < game_start_x || snake_x[0] >= game_end_x ||
        snake_y[0] < game_start_y || snake_y[0] >= game_end_y) {
        draw_game_over();
        return;
    }
    
    for (int i = 1; i < snake_len; i++) {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
            draw_game_over();
            return;
        }
    }
    
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
        if (snake_len < MAX_SNAKE_LEN) snake_len++;
        spawn_food();
        score++;
        score_updated = true;
    }
}

/**
 * @brief Handles key press events to change snake direction or exit game.
 * @param scancode The PS/2 scancode of the pressed key.
 */
void handle_key(uint8_t scancode) {
    switch (scancode) {
        case W_KEY: if (dir_y != 1) { dir_x = 0;  dir_y = -1; } break;
        case S_KEY: if (dir_y != -1){ dir_x = 0;  dir_y =  1; } break;
        case A_KEY: if (dir_x != 1) { dir_x = -1; dir_y =  0; } break;
        case D_KEY: if (dir_x != -1){ dir_x =  1; dir_y =  0; } break;
        case ESC_BREAK_CODE:
            game_exit(); 
            break;
        default:
            break;
    }
}
/**
 * @brief Draws the score label and current value at the top of the play area.
 */
void draw_score(void) {
    int score_x = game_start_x + (curr_game_area_width / 2) - 50; 
    int score_y = game_start_y - BORDER_WIDTH - 30;
    draw_text(score_x, score_y, "SCORE", 0xFFFFFF);
    draw_number(score_x + 200, score_y, score, 0xFFFFFF);
}
/**
 * @brief Starts or restarts the game: initializes state and flags.
 */
void game_start(void) {

    init_game();
    spawn_food();
    score_updated = true;
    game_running = true;
}
/**
 * @brief Exits the current game and returns control to the menu.
 */
void game_exit(void) {
    game_running = false;
}
/**
 * @brief Returns whether the game is still running.
 * @return true if the game loop should continue, false otherwise.
 */
bool game_is_running(void) {
    return game_running;
}
/**
 * @brief Draws the game over screen.
 */
void draw_game_over(void) {
    vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x000000); 

    int x = mode_info.XResolution / 2 - 100;
    int y = mode_info.YResolution / 2 - 10;
    vg_draw_rectangle(x, y, 200, 20, 0xF65AA0); 
    draw_text(x + 50, y + 3, "GAME OVER", 0xFFFFFF); 
    
    int score_x = x + 60;
    int score_y = y + 30;
    draw_text(score_x, score_y, "SCORE", 0xFFFFFF);
    draw_number(score_x + 70, score_y, score, 0xFFFFFF);
    
    tickdelay(micros_to_ticks(2000000)); 

    menu_set_active(true);
}
/**
 * @brief Draws only the static part of the score (label) before dynamic updates.
 */
void draw_score_static(void) {
    int score_x = game_start_x + (curr_game_area_width / 2) - 50; 
    int score_y = game_start_y - BORDER_WIDTH - 30;
    draw_text(score_x, score_y, "SCORE", 0xFFFFFF);
}
/**
 * @brief Draws only the numeric score dynamically during gameplay.
 */
void draw_score_dynamic(void) {
    int score_x = game_start_x + (curr_game_area_width/ 2) - 50; 
    int score_y = game_start_y - BORDER_WIDTH - 30;
    
    int num_digits = 0;
    int temp = score;
    if (temp == 0) num_digits = 1;
    else {
        while (temp) {
            num_digits++;
            temp /= 10;
        }
    }
    
    int clear_width = num_digits * 12;
    vg_draw_rectangle(score_x + 80, score_y, clear_width, 20,0x333333);
    draw_number(score_x + 80, score_y, score, 0xFFFFFF);
}
/**
 * @brief Draws the static elements of the playfield (background, borders, title).
 */
void draw_game_static(void) {
    vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x333333); 
    vg_draw_rectangle(game_start_x - BORDER_WIDTH, game_start_y - BORDER_WIDTH,  
                      curr_game_area_width + 2 * BORDER_WIDTH, BORDER_WIDTH, 0x666666);
    vg_draw_rectangle(game_start_x - BORDER_WIDTH, game_start_y + curr_game_area_height, 
                      curr_game_area_width + 2 * BORDER_WIDTH, BORDER_WIDTH, 0x666666);
    vg_draw_rectangle(game_start_x - BORDER_WIDTH, game_start_y,  
                      BORDER_WIDTH, curr_game_area_height, 0x666666);
    vg_draw_rectangle(game_start_x + curr_game_area_width, game_start_y,  
                      BORDER_WIDTH, curr_game_area_height, 0x666666);
    
    vg_draw_rectangle(game_start_x, game_start_y, curr_game_area_width, curr_game_area_height, 0xdddddd);
    
    int title_x = game_start_x + (curr_game_area_width / 2) - 60;
    int title_y = game_start_y - BORDER_WIDTH - 70;
    draw_text(title_x, title_y, "SNAKE GAME", 0xFFFFFF);
    
    draw_score_static();
}
/**
 * @brief Draws the dynamic elements of the playfield (food, snake segments).
 */
void draw_game_dynamic(void) {
    vg_draw_rectangle(game_start_x, game_start_y, curr_game_area_width, curr_game_area_height, 0xdddddd);
    if (score_updated) {
        draw_score_dynamic();
        score_updated = false;
    }
    
    vg_draw_rectangle(food_x, food_y, CELL_SIZE, CELL_SIZE, 0xFFFA00);
    
    for (int i = 0; i < snake_len; i++) {
        vg_draw_rectangle(snake_x[i], snake_y[i], CELL_SIZE, CELL_SIZE, 0x00FF00);
    }
}

static int draw_help() {
    // Clear the screen (using your background color)
    vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x333333);

    // Draw the help text
    draw_text(50, 50, "Snake Game Help:", 0xFFFFFF);
    draw_text(50, 70, "Objective:", 0xFFFFFF);
    draw_text(70, 90, "Guide your snake to eat food, grow longer, and avoid collisions with walls or yourself to earn points.", 0xFFFFFF);
    draw_text(50, 110, "Controls:", 0xFFFFFF);
    draw_text(70, 130, "W: Up", 0xFFFFFF);
    draw_text(70, 150, "S: Down", 0xFFFFFF);
    draw_text(70, 170, "A: Left", 0xFFFFFF);
    draw_text(70, 190, "D: Right", 0xFFFFFF);
    draw_text(70, 210, "ESC: Exit", 0xFFFFFF);
    draw_text(50, mode_info.YResolution - 40, "Developed by: Duarte Marques, Maria Vieira, Marta Cruz", 0xFFFFFF);

    return 0;
}

static void help_screen(void) {
    message msg;
    int ipc_status;
    
    // Draw the static help screen once
    draw_help();
    
    // Loop until the ESC break code is received
    while (1) {
        if (driver_receive(ANY, &msg, &ipc_status) != OK)
            continue;
        
        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & BIT(kbd_mask)) {
                kbc_ih(); // Process the keyboard interrupt
                // Check for key release of ESC (break code)
                if (scancode == ESC_BREAK_CODE) {
                    scancode = 0;  // Reset scancode so main loop doesn't exit
                    break;
                }
            }
        }
    }
    
    // Reactivate menu after exiting the help screen
    menu_set_active(true);
}
