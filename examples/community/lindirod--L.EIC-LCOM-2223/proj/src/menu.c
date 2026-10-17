// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

//header files created by us
#include "menu.h"
#include "keys.h"

#define TETRIS_LOGO_XPM_X_START 250
#define TETRIS_LOGO_XPM_Y_START 25

#define PLAY_XPM_X_START 250
#define PLAY_XPM_Y_START 250
#define PLAY_XPM_WIDTH 300
#define PLAY_XPM_HEIGHT 50

#define INSTRUCTIONS_XPM_X_START 250
#define INSTRUCTIONS_XPM_Y_START 325
#define INSTRUCTIONS_XPM_WIDTH 300
#define INSTRUCTIONS_XPM_HEIGHT 50

#define QUIT_XPM_X_START 250
#define QUIT_XPM_Y_START 400
#define QUIT_XPM_WIDTH 300
#define QUIT_XPM_HEIGHT 50

#include "menu.h"
#include "vbe_graphics.h"
#include "game_state.h"

#include "xpm/menu/tetris_logo.xpm"

#include "xpm/menu/tetris_mouse_pointer.xpm"

#include "xpm/menu/txt_play.xpm"
#include "xpm/menu/txt_leaderboard.xpm"
#include "xpm/menu/txt_instructions.xpm"
#include "xpm/menu/txt_quit.xpm"

#include "xpm/menu/txt_play_selected.xpm"
#include "xpm/menu/txt_leaderboard_selected.xpm"
#include "xpm/menu/txt_instructions_selected.xpm"
#include "xpm/menu/txt_quit_selected.xpm"

#include "xpm/instructions/instructions.xpm"

GameState game_state = MENU;

uint8_t menu_counter;
#include "mouse.h"

int16_t x_pos = 250;
int16_t y_pos = 350;

uint8_t menu_counter;
extern struct packet pp;

xpm_image_t tetris_logo, tetris_mouse_pointer,
            txt_play, txt_play_selected, 
            txt_instructions, txt_instructions_selected, 
            txt_quit, txt_quit_selected;

xpm_image_t *txt_play_draw = NULL;
xpm_image_t *txt_instructions_draw = NULL;
xpm_image_t *txt_quit_draw = NULL;

xpm_image_t instructions;

void (instruction_file_init)(){
    instructions.bytes = xpm_load(instructions_draw_xpm, XPM_8_8_8, &instructions);
}

void (draw_instructions)(){
    draw_xpm(&instructions, 0, 0);
}

void (clear_instructions)(){
    erase_xpm(0, 0, &instructions);
}



void (menu_files_init)() {
    menu_counter = 0;

    tetris_logo.bytes = xpm_load(tetris_logo_xpm,XPM_8_8_8, &tetris_logo);

    tetris_mouse_pointer.bytes = xpm_load(tetris_mouse_pointer_xpm, XPM_8_8_8, &tetris_mouse_pointer);

    txt_play.bytes = xpm_load(txt_play_xpm, XPM_8_8_8, &txt_play);
    txt_instructions.bytes = xpm_load(txt_instructions_xpm, XPM_8_8_8, &txt_instructions);
    txt_quit.bytes = xpm_load(txt_quit_xpm, XPM_8_8_8, &txt_quit);

    txt_play_selected.bytes = xpm_load(txt_play_selected_xpm, XPM_8_8_8, &txt_play_selected);
    txt_instructions_selected.bytes = xpm_load(txt_instructions_selected_xpm, XPM_8_8_8, &txt_instructions_selected);
    txt_quit_selected.bytes = xpm_load(txt_quit_selected_xpm, XPM_8_8_8, &txt_quit_selected);
}

void (draw_menu)() {
    
    draw_xpm(&tetris_logo, TETRIS_LOGO_XPM_X_START, TETRIS_LOGO_XPM_Y_START);
    
    switch (menu_counter)
    {
    case 0:
        txt_play_draw = &txt_play_selected;
        txt_instructions_draw = &txt_instructions;
        txt_quit_draw = &txt_quit;
        break;
    case 1:
        txt_play_draw = &txt_play;
        txt_instructions_draw = &txt_instructions_selected;
        txt_quit_draw = &txt_quit;
        break;
    case 2:
        txt_play_draw = &txt_play;
        txt_instructions_draw = &txt_instructions;
        txt_quit_draw = &txt_quit_selected;
        break;
    default:
        break;
    }

    draw_xpm(txt_play_draw, PLAY_XPM_X_START, PLAY_XPM_Y_START);
    draw_xpm(txt_instructions_draw, INSTRUCTIONS_XPM_X_START, INSTRUCTIONS_XPM_Y_START);
    draw_xpm(txt_quit_draw, QUIT_XPM_X_START,QUIT_XPM_Y_START);
    
}

void clear_menu() {
    erase_xpm(TETRIS_LOGO_XPM_X_START, TETRIS_LOGO_XPM_Y_START, &tetris_logo);
    erase_xpm(PLAY_XPM_X_START, PLAY_XPM_Y_START, txt_play_draw);
    erase_xpm(INSTRUCTIONS_XPM_X_START, INSTRUCTIONS_XPM_Y_START, txt_instructions_draw);
    erase_xpm(QUIT_XPM_X_START, QUIT_XPM_Y_START, txt_quit_draw);
}

  
bool (navigate_menu)(uint8_t *scancode) {
    if(*scancode == KBD_UP) {
        switch(menu_counter) {
            case 0: menu_counter = 2; break;
            default: menu_counter--; break;
        }
        return true;
    } else if(*scancode == KBD_DOWN) {
        switch(menu_counter) {
            case 2: menu_counter = 0; break;
            default: menu_counter++; break;
        }
        return true;
    } else if(*scancode == KBD_RETURN) {
        switch (menu_counter)
        {
        case 0:
            clear_menu();
            game_state = GAME;
            return true;
            break;
        case 1:
            clear_menu();
            game_state = INSTRUCTIONS;
            return true;
            break;
        case 2:
            *scancode = KBD_ESC;
            return true;
            break;
        default:
            break;
        }
    }

    return false;
}

void (update_mouse_coordinates)(bool draw) {
    
    vg_draw_pixel(x_pos, y_pos, 0x000000);
    
  if(x_pos+pp.delta_x < 0 && !pp.x_ov){
    x_pos = 0;
  } else if(x_pos+pp.delta_x > 800 && !pp.x_ov){
    x_pos = 800;
  } else if(!pp.x_ov){
    x_pos+=pp.delta_x;
  }

  if(y_pos-pp.delta_y < 0 && !pp.y_ov){
    y_pos = 0;
  } else if(y_pos-pp.delta_y > 600 && !pp.y_ov){
    y_pos = 600;
  } else if(!pp.y_ov){
    y_pos-=pp.delta_y;
  }

  if(x_pos > 800 ){
    x_pos = 800;
  }
  
  if(y_pos > 600) {
    y_pos = 600;
  }

  if(draw) {
    vg_draw_pixel(x_pos, y_pos, 0xFFFFFF);
  }
}



bool menu_mouse_handler(uint8_t *scancode, struct mouse_ev *event) {
    static int prev_option = -1; // initialize to an invalid value
    static int option_timer = 10; // timer for option stability
    bool draw = true;

    if (event->type == MOUSE_MOV) {
        int new_option = -1; // initialize to an invalid value

        if ((x_pos > PLAY_XPM_X_START) && (x_pos < (PLAY_XPM_X_START + PLAY_XPM_WIDTH)) && (y_pos > PLAY_XPM_Y_START) && (y_pos < (PLAY_XPM_Y_START + PLAY_XPM_HEIGHT))) {
            new_option = 0;
            draw = false;
        } else if ((x_pos > INSTRUCTIONS_XPM_X_START) && (x_pos < (INSTRUCTIONS_XPM_X_START + INSTRUCTIONS_XPM_WIDTH)) && (y_pos > INSTRUCTIONS_XPM_Y_START) && (y_pos < (INSTRUCTIONS_XPM_Y_START + INSTRUCTIONS_XPM_HEIGHT))) {
            new_option = 1;
            draw = false;
        } else if ((x_pos > QUIT_XPM_X_START) && (x_pos < (QUIT_XPM_X_START + QUIT_XPM_WIDTH)) && (y_pos > QUIT_XPM_Y_START) && (y_pos < (QUIT_XPM_Y_START + QUIT_XPM_HEIGHT))) {
            new_option = 2;
            draw = false;
        } 

        update_mouse_coordinates(draw);

        // Update selected option and redraw menu if necessary
        if (new_option != -1 && new_option != prev_option) {
            if (new_option == option_timer) {
                menu_counter = new_option;
                prev_option = new_option;
                draw_menu();
                option_timer = 0; // reset timer
            } else {
                option_timer = new_option;
            }
        }
    } else if (event->type == LB_PRESSED) {
        // Handle left button click
        switch (menu_counter) {
            case 0: // PLAY
            clear_menu();
            game_state = GAME;
                break;
            case 1: // INSTRUCTIONS
            clear_menu();
            game_state = INSTRUCTIONS;
                break;
            case 2: // QUIT
                *scancode = KBD_ESC;
                return true;
                break;
            default:
                break;
        }
    }

    return false;
}


