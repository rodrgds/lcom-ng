#include "../view/draw_game.h"
#include "../view/draw_functions.h"




void draw_game_start(GameState *game_state) {
    if (game_state == NULL) return;

    // Clear the screen
    clearScreen();
    refresh_frame_buffer();


}

void draw_input(GameState *game_state) {
    if (game_state == NULL || game_state->input == NULL ) return;

    // Calculate the width of the input word
    int total_width = 0;
    for (size_t i = 0; game_state->input->word[i] != '\0'; i++) {
        const xpm_row_t* sprite = get_letter_sprite_red(game_state->input->word[i]);
        if (sprite) total_width += get_xpm_width(sprite);
    }


    // Calculate centered position
    int rect_x = game_state->writting_rectangle_position.position.x;
    int rect_y = game_state->writting_rectangle_position.position.y;
    int rect_w = game_state->writting_rectangle_position.width;
    int rect_h = game_state->writting_rectangle_position.height;

    int x = rect_x + (rect_w - total_width) / 2;
    int y = rect_y + (rect_h + get_xpm_height(get_letter_sprite_red('A'))) / 2;

    clear_input(game_state); 
    draw_sentence(game_state->input->word, x, y, "black");
    
}

void clear_input(GameState *game_state) {

    // Clear the writing rectangle
    vg_draw_rectangle(game_state->writting_rectangle_position.position.x,
                      game_state->writting_rectangle_position.position.y,
                      game_state->writting_rectangle_position.width,
                      game_state->writting_rectangle_position.height,
                      game_state->writting_rectangle_position.color); 
}


void draw_answer(GameState *game_state , bool correct) {

    char* correct_word = game_state->word_positions[game_state->current_word_index].word;
    draw_sentence(correct_word, 
                    game_state->word_positions[game_state->current_word_index].coordinates.x, 
                    game_state->word_positions[game_state->current_word_index].coordinates.y, 
                  correct ? "green" : "red");
}

void draw_game_stats(GameState *game_state, int secondsPerRound) {
    clearScreen();
    
    char buffer[64];

    int time_remaining = secondsPerRound - game_state->time_taken;
    if (time_remaining < 0) time_remaining = 0;

    int score = game_state->correct_words * time_remaining;

    if (game_state->correct_words + game_state->incorrect_words > 35) {
      score *= 5;
    } // Se o modo de jogo for difícil, o score é multiplicado por 5

    draw_sentence("Game Completed !", 300, 100, "white");

    sprintf(buffer, "Score : %d points !", score);
    draw_sentence(buffer, 100, 300, "white");

    sprintf(buffer, "Words correct : %d", game_state->correct_words);
    draw_sentence(buffer, 100, 400, "white");

    sprintf(buffer, "Words incorrect : %d", game_state->incorrect_words);
    draw_sentence(buffer, 100, 500, "white");

    sprintf(buffer, "Time taken : %d seconds", game_state->time_taken);
    draw_sentence(buffer, 100, 600, "white");

    sprintf(buffer, "Accuracy : %.2f%%", 
        (game_state->correct_words + game_state->incorrect_words > 0) ? 
        (float)game_state->correct_words / (game_state->correct_words + game_state->incorrect_words) * 100 : 0);
    draw_sentence(buffer, 100, 700, "white");

    refresh_frame_buffer();
}

void draw_game_lost() {
    clearScreen();
    draw_sentence("You Lost! Time is up!", 200, get_vertical_resolution() / 2, "white");
    refresh_frame_buffer();
}





