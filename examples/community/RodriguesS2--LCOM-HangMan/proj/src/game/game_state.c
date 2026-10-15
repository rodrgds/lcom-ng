#include "../peripherals/graphics/graphics.h"  
#include "../peripherals/mouse/mouse.h"
#include "../assets/font.h"                  
#include "../assets/game_pixmap.h"

#include "words.h"           
#include "hangman.h"          
#include "menu.h"             
#include "instructions.h" 
#include "categories.h"    
#include "game.h"    
#include "game_state.h"


int current_stage = 0;
int remaining_time = TIME_LIMIT
int menu_selected_option = 0;
int categories_selected_option = 0;
bool redraw_needed = true;
bool is_setup = true;


void handle_game_state() {
    switch (state) {
        case MENU:
            draw_options(menu_selected_option); 
            break;

        case CATEGORIES:
            draw_categories(categories_selected_option);
            break;

        case PLAY:
            if(is_setup) {
               is_setup = false;
            }

            if (remaining_time == 0) {

                if(current_stage < 6) {

                    //add (+1) to the current state of the hangman and if it is not in the "final form"
                    current_stage++;
                    remaining_time = TIME_LIMIT
                }

                else {

                    state = EXIT;
                }

                redraw_needed = true;
            }

            if (current_stage > 6) {
                state = EXIT;
                redraw_needed = true;
            }
    
            draw_hangman(5, 150, current_stage);
            draw_word_guesses();
            draw_used_letters();
            
            gameCountdown(remaining_time);
            
            if(check_win()) {
                state = EXIT;
                redraw_needed = true;
            }

            break;


        case INSTRUCTIONS:
                draw_instructions_screen();
            break;
            
        case EXIT:
            draw_game_over_screen();

            if(timer_counter % 180 == 0){
                reset_game_state();
            }
            break;

    }
    draw_mouse_pointer_to_back_buffer();
    swap_buffers();
    redraw_needed = false;
}


void reset_game_state() {

    state = MENU;
    remaining_time = TIME_LIMIT; //we can create a new var
    current_stage = 0;
    menu_selected_option = 0;
    memset(guessed_letters, 0, sizeof(guessed_letters)); //clear the gessed_letters (all false again)
    is_setup = true;
    redraw_needed = true;
}
