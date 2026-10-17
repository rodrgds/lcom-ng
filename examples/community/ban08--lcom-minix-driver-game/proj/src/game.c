/**
 * @file game.c
 * @brief Contains core game logic, including game loop, rendering, enemy handling, and menu interface.
 */

#include "game.h"
#include "video_card.h"
#include "utils.c"

/// @brief Global X coordinate of the player.
extern int x_position;

/// @brief Global Y coordinate of the player.
extern int y_position;

/// @brief Game running flag.
extern bool running;

/// @brief Whether the player is currently going up in a jump.
extern int is_going_up;

/// @brief Represents the current movement state.

extern int menu;

/// @brief Flag indicating whether the game has started.
int has_started = 0;



int janela_min = 0;
int janela_max = 800;
extern int first_screen_appearance;

int rotations = 0;

/// @brief X-coordinates for enemy objects
int enemy_x_coordinates[] = {700, 1200, 1400, 1900, 2900, 3100, 3500, 3700};


/**
 * @brief Starts and runs one frame of the game loop.
 *
 * This function handles:
 * - Drawing menu or game screen based on current state.
 * - Initializing the environment and background.
 * - Managing vertical movement (jumping/falling).
 * - Drawing the player and enemies.
 * - Scrolling window logic.
 * - Collision detection.
 */
void game_start(){

    if (menu == 1){

        if (first_screen_appearance == 1){
            vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x00000000);
            draw_menu_screen();
            first_screen_appearance = 0;

        }

    } else {
        
    if (first_screen_appearance == 1){
        vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x00000000);
        has_started = 0;
        first_screen_appearance = 0;

    }


    if (has_started == 0){
        has_started = 1;
        vg_draw_rectangle(0,0, mode_info.XResolution, 500, 0xFF7DC7C4);
        
        //Sol
        vg_draw_rectangle(620,70, 80, 80, 0xFFE0C25C);
        vg_draw_rectangle(630,80, 60, 60, 0xFFD1A513);
        
        //Nuvem
        vg_draw_rectangle(80,80, 450, 140, 0XFF969696);
        vg_draw_rectangle(100,100, 220, 100, 0XFFBABABA);
        vg_draw_rectangle(350,110, 110, 90, 0XFFBABABA);
        
        // Chao
        vg_draw_rectangle(0,500, mode_info.XResolution, 25, 0xFFBD9773);
        vg_draw_rectangle(0,525, mode_info.XResolution, 25, 0xFF9E7E60);
        vg_draw_rectangle(0,550, mode_info.XResolution, 25, 0xFF735C46);
        vg_draw_rectangle(0,575, mode_info.XResolution, 25, 0xFF45372A);

        //icons


        //Letra P
        vg_draw_rectangle(700, 250, 30, 30, 0xFF00FF00);
        vg_draw_rectangle(705, 255, 5, 20, 0xFF000000);
        vg_draw_rectangle(710, 255, 10, 5, 0xFF000000);
        vg_draw_rectangle(710, 265, 10, 5, 0xFF000000);
        vg_draw_rectangle(720, 260, 5, 5, 0xFF000000);


    }
    
    game_jump();

    for (int y = 325; y < 500; y++) {
        for (int x = 40; x < 70; x++) {
            vg_draw_pixel(x, y, 0xFF7DC7C4);
        }
    }

    for (int y = 488; y < 500; y++) {
        for (int x = 0; x < mode_info.XResolution; x++) {
            vg_draw_pixel(x, y, 0xFF7DC7C4);
        }
    }

    vg_draw_rectangle(x_position, y_position, 30, 75, 0XFF105902);

    draw_enemies();

    janela_min = janela_min + 5;
    janela_max = janela_max + 5;

    if (janela_min > 3700){
    janela_min = -200;
    janela_max = 600;
    rotations = rotations + 1;
}
    }
    usleep(16000); // aprox 60 FPS --> parece me ser o sweet spot. + frames, + tearing



}

/**
 * @brief Draws all visible enemies within the current scrolling window.
 *
 * Also handles collision detection between enemies and the player.
 * If a collision is detected, the game returns to the menu screen.
 */
void draw_enemies(){
    for (unsigned int i = 0; i < 8; i++){
        if ((enemy_x_coordinates[i] > janela_min) && (enemy_x_coordinates[i] < janela_max)){
            vg_draw_rectangle(enemy_x_coordinates[i] - janela_min, 488, 4, 12, 0xFFE37610);
            if ((enemy_x_coordinates[i] - janela_min > 40) && (enemy_x_coordinates[i] - janela_min < 70) && (y_position > 425 - 12)){
                rotations = 0;
                janela_max = 800;
                janela_min = 0;
                menu = 1;
                first_screen_appearance = 1;
                has_started = 1;

                printf("You scored %d points!", enemy_x_coordinates[i] + rotations * 3700);
            }
        }
    }

}

/**
 * @brief Draws the menu screen, including "MENU", "DINO" title, and options for play and exit.
 *
 * This function uses pixel/rectangle primitives to render letters and buttons manually.
 */
void draw_menu_screen() {

    // Letra M
    vg_draw_rectangle(490, 100, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(480, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(500, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(510, 100, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(520, 100, 10, 50, 0xFFFFFF00);
    
    // Letra E
    vg_draw_rectangle(540, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(550, 100, 20, 10, 0xFFFFFF00);
    vg_draw_rectangle(550, 125, 15, 10, 0xFFFFFF00);
    vg_draw_rectangle(550, 140, 20, 10, 0xFFFFFF00);
    
    // Letra N
    vg_draw_rectangle(580, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(590, 110, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(600, 120, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(610, 130, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(620, 100, 10, 50, 0xFFFFFF00);
    
    // Letra U
    vg_draw_rectangle(640, 100, 10, 40, 0xFFFFFF00);
    vg_draw_rectangle(670, 100, 10, 40, 0xFFFFFF00);
    vg_draw_rectangle(650, 140, 20, 10, 0xFFFFFF00);
    


    //texto a dizer DINO
    int start_x = 100;

    int start_y = 100;

    int letter_w = 20;

    int letter_h = 50;

    int gap = 15;

    // Letra D
    vg_draw_rectangle(start_x, start_y, 5, letter_h, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 5, start_y, 10, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 15, start_y + 5, 5, 40, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 5, start_y + 45, 10, 5, 0xFFFFFF00);

    // Letra I
    start_x += letter_w + gap;
    vg_draw_rectangle(start_x + 5, start_y, 10, letter_h, 0xFFFFFF00);

    // Letra N
    start_x += letter_w + gap;
    vg_draw_rectangle(start_x, start_y, 5, letter_h, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 5, start_y + 10, 5, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 10, start_y + 20, 5, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 15, start_y + 30, 5, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 20, start_y, 5, letter_h, 0xFFFFFF00);

    // Letra O
    start_x += letter_w + gap;
    vg_draw_rectangle(start_x, start_y, 5, letter_h, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 20, start_y, 5, letter_h, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 5, start_y, 15, 5, 0xFFFFFF00); 
    vg_draw_rectangle(start_x + 5, start_y + 45, 15, 5, 0xFFFFFF00); 




    //palavra play
    int square_x = 100;
    int square_y = 400;
    int square_size = 30;

    // quadrado verde
    vg_draw_rectangle(square_x, square_y, square_size, square_size, 0xFF00FF00);

    //S dentro do quadrado
    vg_draw_rectangle(square_x + 5, square_y + 5, 20, 5, 0xFF000000);
    vg_draw_rectangle(square_x + 5, square_y + 5, 5, 10, 0xFF000000);
    vg_draw_rectangle(square_x + 5, square_y + 15, 20, 5, 0xFF000000);
    vg_draw_rectangle(square_x + 20, square_y + 15, 5, 10, 0xFF000000);
    vg_draw_rectangle(square_x + 5, square_y + 25, 20, 5, 0xFF000000);



    int text_x = square_x + square_size + 10;
    int text_y = square_y + 5;
    vg_draw_rectangle(text_x, text_y, 2, 10, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 4, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 6, text_y + 2, 2, 2, 0xFFFFFFFF);
    text_x += 12;
    vg_draw_rectangle(text_x, text_y, 2, 10, 0xFFFFFFFF);
    vg_draw_rectangle(text_x, text_y + 8, 6, 2, 0xFFFFFFFF);

    // Letra A
    text_x += 10;
    vg_draw_rectangle(text_x, text_y + 2, 2, 8, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 6, text_y + 2, 2, 8, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 4, 4, 2, 0xFFFFFFFF);

    // Letra Y
    text_x += 12;
    vg_draw_rectangle(text_x, text_y, 2, 4, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 6, text_y, 2, 4, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 3, text_y + 4, 2, 6, 0xFFFFFFFF);



    //exit

    square_x = 100;
    square_y = 440;
    int square_width = 40;
    int square_height = 20;

    // retangulo vermelho (nao cabe num quadrado, a menos q fosse demasiado alto)
    vg_draw_rectangle(square_x, square_y, square_width, square_height, 0xFFFF0000);

    int esc_x = square_x + 4;
    int esc_y = square_y + 4;

    // E
    vg_draw_rectangle(esc_x, esc_y, 2, 10, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y, 6, 2, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y + 4, 4, 2, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y + 8, 6, 2, 0xFF000000);

    // S
    esc_x += 10;
    vg_draw_rectangle(esc_x, esc_y, 8, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 4, 6, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 8, 8, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 2, 2, 2, 0xFF000000);
    vg_draw_rectangle(esc_x + 6, esc_y + 6, 2, 2, 0xFF000000);

    // C
    esc_x += 10;
    vg_draw_rectangle(esc_x + 2, esc_y, 6, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 2, 2, 6, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y + 8, 6, 2, 0xFF000000);

    // exit
    text_x = square_x + square_width + 10;
    text_y = square_y + 4;

    // E
    vg_draw_rectangle(text_x, text_y, 2, 8, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 3, 3, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 6, 4, 2, 0xFFFFFFFF);

    // X
    text_x += 8;
    vg_draw_rectangle(text_x, text_y, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 4, text_y, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 2, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x, text_y + 4, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 4, text_y + 4, 2, 2, 0xFFFFFFFF);

    // I
    text_x += 8;
    vg_draw_rectangle(text_x + 2, text_y, 2, 8, 0xFFFFFFFF);

    // T
    text_x += 6;
    vg_draw_rectangle(text_x, text_y, 6, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 2, 2, 6, 0xFFFFFFFF);

}


/**
 * @brief Handles player vertical movement during jumping or falling.
 *
 * This function modifies the `y_position` based on whether the player is going up or falling down.
 * - Jumping stops at a maximum height.
 * - Falling stops when reaching the ground level.
 */
void game_jump(){
    if (is_going_up && y_position > 325){
        y_position = y_position - 5;
    }
    else if (is_going_up && y_position <= 325){
        is_going_up = 0;
    }
    else if (!is_going_up && y_position < 425){
        y_position = y_position + 5;
    }
}
