#include "view.h"

extern Sprite* mouse;
extern Sprite* chooseColors;
extern Sprite* quitButton;
extern Sprite* startButton;
extern Sprite* numbers;
extern Sprite* letters;
extern uint16_t bytes_per_pixel;
extern Sprite* leaderboardTable;
extern Sprite* playAgainButton;
extern Sprite* leaderboardButton;
extern Sprite* initialMenuButton;
extern Sprite* victory;
extern Sprite* defeat;
extern Sprite* ldbdButtonInitialPage;
extern Sprite* coopDrawButton;
extern Sprite* dealer;

extern struct leaderboardValue leaderboard[5];
extern int x, y;
vbe_mode_info_t vmi_p;
extern MenuState menuState;
extern GameState gameState;
extern int game_counter;

extern int delayTime;
extern int number_letters;
extern uint8_t word_guess[10];

extern bool gameResult;

extern uint8_t word_solution[12];
extern int word_sol_number_letters;

extern uint8_t game_title[18];
extern int game_title_size;

extern uint8_t text[12];
extern int text_size;

void draw_new_frame() {
    switch(menuState){
        case START:
            draw_initial_menu();
            break;
        case GAME:
            draw_game_menu();
            break;
        case LEADERBOARD:
            draw_leaderboard();
            break;
        case END:
            draw_end_menu();
            break;
    }
    draw_mouse();
}

void draw_mouse() {
    draw_sprite_xpm(mouse, x, y);
}

void draw_initial_menu() {
    game_title_size = 0;
    for (int i = 0; i < 18; i++) {
        game_title[i] = -1;
    }
    convert_to_qwerty("paint with friends", game_title, &game_title_size);
    draw_word(game_title, game_title_size, 50, 50, GOLD);
    draw_sprite_xpm(startButton, 400, 200);
    draw_sprite_xpm(coopDrawButton, 400, 350);
    draw_sprite_xpm(ldbdButtonInitialPage, 400, 500);
    draw_sprite_xpm(quitButton, 400, 650);
    draw_sprite_xpm(dealer,700,500);
}

void draw_game_menu() {
    draw_bottom_bar(0,750,1152,114, GOLD,80,780,900,70);

    if (gameState == SINGLEPLAYER) {
        draw_sprite_xpm(chooseColors, 0, 0);
        if (delayTime <= 5) draw_word(word_solution,word_sol_number_letters, -1, -1, BLACK);
        else draw_word(word_guess, number_letters, -1, -1, BLACK);

    } else if (gameState == DRAW) {
        draw_sprite_xpm(chooseColors, 0, 0);
        if (delayTime <= 5) draw_word(word_solution,word_sol_number_letters, -1, -1, BLACK);
    } else if (gameState == GUESS) {
        draw_sprite_xpm(chooseColors, 0, 0);
        draw_word(word_guess, number_letters, -1, -1, BLACK);
    }

    draw_game_time(game_counter);
}

void draw_end_menu(){

    draw_sprite_xpm(gameResult ? victory : defeat, 337,0);
    text_size = 0;
    for (int i = 0; i < 18; i++) {
        text[i] = -1;
    }
    if (gameState == SINGLEPLAYER || gameState == DRAW) {
        convert_to_qwerty("the word was", text, &text_size);
        draw_word(text, text_size, 0,400, BLACK);
        draw_word(word_solution,word_sol_number_letters, 300,500, gameResult ? GREEN : RED);
    }
    
    draw_sprite_xpm(playAgainButton, 131, 600);
    draw_sprite_xpm(leaderboardButton, 462, 600);
    draw_sprite_xpm(initialMenuButton, 793, 600);
}

void draw_leaderboard() {
    draw_sprite_xpm(leaderboardTable, 0 , 0);
    int y_gap = 0;

    for (int i = 0; i < 5; i++) {
        int x_pos = 20;
        int y_pos = 270 + y_gap;

        //draw month
        if(leaderboard[i].month < 9){
            draw_number(numbers, x_pos, y_pos, leaderboard[i].month % 10);
            x_pos += 140;
        } 
        else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].month / 10);
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].month % 10);  
            x_pos += 70;
        }

        //draw day
        if(leaderboard[i].day < 9){
            draw_number(numbers, x_pos, y_pos, leaderboard[i].day % 10);  
            x_pos += 190;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].day / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].day % 10);  
            x_pos += 120;
        }

        //draw hour
        if(leaderboard[i].hour < 9){
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].hour % 10);  
            x_pos += 70;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].hour / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].hour % 10);  
            x_pos += 70;
        }

        //draw minute
        if(leaderboard[i].minute < 9){
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].minute % 10);  
            x_pos += 70;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].minute / 10); 
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].minute % 10);  
            x_pos += 70;
        }

        //draw second
        if(leaderboard[i].second < 9){
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].second % 10);  
            x_pos += 200;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].second / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].second % 10);  
            x_pos += 200;
        }

        //draw score
        if(leaderboard[i].score < 9){
            draw_number(numbers, x_pos, y_pos, leaderboard[i].score % 10);  
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].score / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].score % 10);  
        }

        y_gap += 125;  
    }
}


int draw_sprite_xpm(Sprite *sprite, int x, int y) {
    uint16_t width;
    uint16_t height;
    width = sprite->width;
    height = sprite->height;
    uint32_t current_color;
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = sprite->colors[w + h*width];
            if (current_color != TRANSPARENT) {
                vg_draw_pixel(x + w, y + h, current_color);
            }
        }
    }
    return 0;
}

int draw_bar(int x, int y, int width, int height, uint32_t color){
    for (uint16_t h = 0; h < height; h++){
        for (uint16_t w = 0; w < width; w++){
            vg_draw_pixel(x + w, y + h, color);
        }
    }
    return 0;
}

int draw_bottom_bar(int x, int y, int width, int height, uint32_t color, int square_vertex_x , int square_vertex_y , int square_width , int square_height){
    
    for (uint16_t h = 0; h < height; h++){
        for (uint16_t w = 0; w < width; w++){
            // (x+w),(y+h) -> position to draw 
            if ( (x + w >= square_vertex_x && x + w <= square_vertex_x + square_width) && (y + h >= square_vertex_y && y + h <=  square_vertex_y + square_height)) vg_draw_pixel(x + w, y + h, WHITE);
            else {
                vg_draw_pixel(x + w, y + h, color);
            }
        }
    }
    return 0;
}

int draw_word(uint8_t word[], int word_size, int x, int y, uint32_t color){
    int letter_pos = 0;
    for(int i = 0; i < word_size; i++){
        if(word[i] == 26){letter_pos++; continue;}
        if (x != -1 && y != -1) draw_letter(x + letter_pos * 60, y, word[i], color);
        else{
            draw_letter(80 + letter_pos * 60, 780, word[i], color);
        }
        letter_pos++;
    }
    return 0;
}

int draw_letter(int x, int y, uint8_t letter_index, uint32_t color){

    uint16_t width = 70;
    uint16_t height = 70;
    uint16_t img_width = letters->width;

    uint32_t current_color;
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = letters->colors[(70 * letter_index  + w) + (img_width * h)];
            if(current_color != TRANSPARENT) {
                current_color = color == BLACK ? current_color : color;
                vg_draw_pixel(x + w, y + h, current_color);
            }
        }
    }
    return 0;
}

int draw_number(Sprite* sprite, int x, int y, int index){
    uint16_t width = 70;
    uint16_t height = 70;
    uint16_t img_width = sprite->width;

    uint32_t current_color;
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = sprite->colors[(70 * index  + w) + (img_width * h)];
            if(current_color != TRANSPARENT) {
                vg_draw_pixel(x + w, y + h, current_color);
            }
        }
    }
    return 0;
}

int draw_game_time(int num)
{   
    int arr[2];
    int i = 0;
    int j, r;
  
    while (num != 0) {
        r = num % 10;
        arr[i] = r;
        i++;
        num = num / 10;
    }
    
    int length = 0;
    for (j = i - 1; j > -1; j--) {
        draw_number(numbers, 1000 + length * 60, 780, arr[j]);
        length++;
    }
    return 0;
}
