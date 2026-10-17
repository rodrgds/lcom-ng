#include "design.h"

uint8_t *main_frame_buffer, *secondary_frame_buffer, *drawing_frame_buffer;
uint32_t frame_buffer_size;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
extern MenuState menuState;
extern Game game;

extern Sprite *typo_racer, *play_button, *play, *cursor, *quit, *exit_button, *esc, *you_win, *play_again, *play_again_button, *accuracy;
extern Sprite *letters[26];
extern Sprite *numbers[10];


/**
 * @brief Sets the frame buffers to the given mode.
 * @param mode the mode to set the frame buffers to
 * @return 0 if successful, 1 otherwise
*/
int (design_set_frame_buffers)(uint16_t mode) {
    if (graphics_set_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
    drawing_frame_buffer = secondary_frame_buffer;
    return 0;
}

/**
 * @brief Swaps the frame buffers
*/
void (design_swap_buffers)() {
    memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
}

/**
 * @brief Draws a new frame according to the current menu state
*/
void (design_draw_new_frame)() {
    switch (menuState) {
        case START:
            design_draw_initial_menu();
            break;
        case GAME:
            design_draw_game_menu();
            break;
        case END:
            design_draw_finish_menu();
            break;
    }
}

/**
 * @brief Draws the initial menu of the game
*/
void (design_draw_initial_menu)() { 
    graphics_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, DARKBLUE, drawing_frame_buffer);
    design_draw_sprite_xpm(typo_racer, mode_info.XResolution/4 + 18, mode_info.YResolution/4);
    design_draw_sprite_button(play_button, mode_info.XResolution/2 - 65, mode_info.YResolution/2);
    design_draw_sprite_xpm(play, mode_info.XResolution/2 - 58 + 5, mode_info.YResolution/2 - 10);
    design_draw_sprite_button(exit_button, mode_info.XResolution/2 - 65, mode_info.YResolution/2 + 70);
    design_draw_sprite_xpm(quit, mode_info.XResolution/2 - 58 + 10, mode_info.YResolution/2 + 60);
    return;
}

/**
 * @brief Draws the menu of the game while a user is playing
*/
void (design_draw_game_menu)() {
    graphics_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, DARKBLUE, drawing_frame_buffer);
    design_draw_sprite_xpm(esc, mode_info.XResolution - 365, mode_info.YResolution - 45);
    phrase_writer( game.phrase , 170);
    if(game.pos_player >=1) {
        int elapsed = rtc_get_elapsed_time();
        game.wpm = elapsed > 0 ? (int)(((double)(game.pos_player) / 5) / ((double)elapsed / 60)) : 0;
        if(wpm_writer(game.wpm))return;
    } 
    else {
        if(wpm_writer(0))return;
    }
    return;
} 

/**
 * @brief Draws the menu of the game when a user has finished playing
*/
void (design_draw_finish_menu)() {
    graphics_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, DARKBLUE, drawing_frame_buffer);
    design_draw_sprite_xpm(you_win, mode_info.XResolution/2 - 100, mode_info.YResolution/2 - 100);
    design_draw_sprite_button(play_again_button, mode_info.XResolution/2 - 132, mode_info.YResolution/2);
    design_draw_sprite_xpm(play_again, mode_info.XResolution/2 - 125 + 5, mode_info.YResolution/2 - 10);
    design_draw_sprite_xpm(esc, mode_info.XResolution - 365, mode_info.YResolution - 45);
    if (game.pos_player >=1) {
        game.wpm = game.elapsed_time > 0 ? (int)(((double)(game.pos_player) / 5) / ((double)(game.elapsed_time) / 60)) : 0;
        if(wpm_writer(game.wpm)) return;
        if(accuracy_writer((int)(((double)(game.phrase_size) * 100) / ((double)(game.phrase_size + game.typo_overall_count)))))return;
    }
    else {
        if(wpm_writer(0))return;
    }

    return;
}

/**
 * @brief Draws the mouse cursor
*/
void (design_draw_mouse)() {
    design_draw_sprite_xpm(cursor, mouse_info.x, mouse_info.y);
}

/**
 * @brief Draws a sprite
 * @param sprite sprite to be drawn
 * @param x x coordinate
 * @param y y coordinate
 * @return 0 if successful, 1 otherwise
*/
int (design_draw_sprite_xpm)(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == TRANSPARENT) continue;
        if (graphics_draw_pixel(x + w, y + h, current_color, drawing_frame_buffer)) return 1;
      }
    }
    return 0; 
}

/**
 * @brief Draws a sprite in green
 * @param sprite sprite to be drawn
 * @param x x coordinate
 * @param y y coordinate
 * @return 0 if successful, 1 otherwise
*/
int (design_draw_sprite_xpm_green)(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == TRANSPARENT) continue;
        if(current_color == GOLD) current_color = GREEN;
        if (graphics_draw_pixel(x + w, y + h, current_color, drawing_frame_buffer)) return 1;
      }
    }
    return 0; 
}

/**
 * @brief Draws a sprite in red
 * @param sprite sprite to be drawn
 * @param x x coordinate
 * @param y y coordinate
 * @return 0 if successful, 1 otherwise
*/
int (design_draw_sprite_xpm_red)(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == TRANSPARENT) continue;
        if(current_color == GOLD) current_color = RED;
        if (graphics_draw_pixel(x + w, y + h, current_color, drawing_frame_buffer)) return 1;
      }
    }
    return 0; 
}

/**
 * @brief Draws a sprite button
 * @param sprite sprite to be drawn
 * @param x x coordinate
 * @param y y coordinate
 * @return 0 if successful, 1 otherwise
*/
int (design_draw_sprite_button)(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t color = sprite->pressed ? PRESSED : sprite->color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (graphics_draw_pixel(x + w, y + h, color, drawing_frame_buffer)) return 1;
      }
    }
    return 0; 
}

/**
 * @brief Function that controls the phrase writing
 * @param word phrase to be written
 * @param y_line y coordinate
 * @return 0 if successful, 1 otherwise
*/
int (phrase_writer)(char* word, int y_line){
    int x_pos = 60;
    int safe_x = x_pos;
    size_t size = strlen(word);
    for(int i=0; i<(int)size; i++){
        char letter = *(word+i);
        if((safe_x >= mode_info.XResolution - 190 && letter == ' ') || safe_x >= mode_info.XResolution - 50){
            y_line += 30;
            x_pos = 40;
            safe_x = x_pos;
        }
        if(letter == '\0') break;
        else if(is_lower(letter)){
            if(i < game.pos_player) {
                if(design_draw_sprite_xpm_green(letters[letter - 'a'], x_pos, y_line)) return 1;
            }
            else if( i < game.pos_player + game.misses_after_hit) {
                if(design_draw_sprite_xpm_red(letters[letter - 'a'], x_pos, y_line)) return 1;
            }
            else {
                if(design_draw_sprite_xpm(letters[letter - 'a'], x_pos, y_line)) return 1;
            }
        }
        else if(is_upper(letter)){
            if(i <= game.pos_player) {
                if(design_draw_sprite_xpm_green(letters[letter - 'A'], x_pos, y_line)) return 1;
            }
            else if(i < game.pos_player + game.misses_after_hit) {
                if(design_draw_sprite_xpm_red(letters[letter - 'A'], x_pos, y_line)) return 1;
            }
            else {
                if(design_draw_sprite_xpm(letters[letter - 'A'], x_pos, y_line)) return 1;
            }
        }
        else if(is_number(letter)){
            if(i <= game.pos_player) {
                if(design_draw_sprite_xpm_green(letters[letter - '0'], x_pos, y_line)) return 1;
            }
            else if(i < game.pos_player + game.misses_after_hit) {
                if(design_draw_sprite_xpm_red(letters[letter - '0'], x_pos, y_line)) return 1;
            }
            else {
                if(design_draw_sprite_xpm(letters[letter - '0'], x_pos, y_line)) return 1;
            }
        }
        else if(letter == ' '){
            safe_x += 1;
            x_pos += 1;
        }
        else return 1;
        safe_x += 18;
        x_pos += 18;
    }

return 0;
}

/**
 * @brief Checks if a letter is lower case
 * @param letter letter to be checked
 * @return 1 if the letter is lower case, 0 otherwise
*/
int (is_lower)(char letter){
    if(letter >= 'a' && letter <= 'z') return 1;
    return 0;
}

/**
 * @brief Checks if a letter is upper case
 * @param letter letter to be checked
 * @return 1 if the letter is upper case, 0 otherwise
*/
int (is_upper)(char letter){
    if(letter >= 'A' && letter <= 'Z') return 1;
    return 0;
}

/**
 * @brief Checks if a letter is a number
 * @param letter letter to be checked
 * @return 1 if the letter is a number, 0 otherwise
*/
int (is_number)(char letter){
    if(letter >= '0' && letter <= '9') return 1;
    return 0;
}

/**
 * @brief Writes the wpm
 * @param wpm wpm to be written
 * @return 0 if successful, 1 otherwise
*/
int (wpm_writer)(int wpm){
    int digit1, digit2;

    if(wpm >= 10) {
        digit1 = wpm % 10;  // Extract the rightmost digit
        wpm /= 10;         // Remove the rightmost digit
        digit2 = wpm % 10;
    }
    else {
        digit1 = wpm;
        digit2 = 0;
    }

    if(design_draw_sprite_xpm(numbers[digit2], 60, 70)) return 1;
    if(design_draw_sprite_xpm(numbers[digit1], 75, 70)) return 1;
    if(design_draw_sprite_xpm(letters['w' -'a'], 100, 70)) return 1;
    if(design_draw_sprite_xpm(letters['p' -'a'], 120, 70)) return 1;
    if(design_draw_sprite_xpm(letters['m' -'a'], 135, 70)) return 1;
    return 0;
}

/**
 * @brief Writes the accuracy
 * @param acc accuracy to be written
 * @return 0 if successful, 1 otherwise
*/
int (accuracy_writer)(int acc){
    int digit1, digit2;

    if(acc >= 10) {
        digit1 = acc % 10;  // Extract the rightmost digit
        acc /= 10;         // Remove the rightmost digit
        digit2 = acc % 10;
    }
    else {
        digit1 = acc;
        digit2 = 0;
    }
    
    if(design_draw_sprite_xpm(numbers[digit2], 550, 70)) return 1;
    if(design_draw_sprite_xpm(numbers[digit1], 570, 70)) return 1;
    if(design_draw_sprite_xpm(accuracy, 600, 70)) return 1;
    return 0;
}
