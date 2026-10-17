#include "menu.h"
#include "assets/pixmaps.h"
#include "controller.h"
#include "sprites.h"
#include "rtc.h"

extern struct packet pp;
extern bool packet_ready;
extern uint32_t counter;
extern bool scancode_ready;
extern bool alarm_out;
static Menu menu;
static Races races;
static bool already_loaded_races = false;

uint8_t race_id;

void (clean_menu) (){
    clean_screen(); /// Erases every xpm from the screen
}

int (initialize_menu) (mouse_sprite* cursor){
    ///Load background
    sprite background;
	load_sprite(0,0,0,0,&background,main_menu_backgound);
	menu.menu_background = background;


    /// Load the play
	sprite play_xpm;
	load_sprite(498,350,0,0,&play_xpm,play_big);
	menu.images[0] = play_xpm;

	/// Load the records
	sprite record_xpm;
	load_sprite(458,400,0,0,&record_xpm,record_big);
	menu.images[1] = record_xpm;

	/// Load the exit
	sprite exit_xpm;
	load_sprite(508,450,0,0,&exit_xpm,exit_big);
	menu.images[2] = exit_xpm;

    /// Load the logotype
	sprite logo;
	load_sprite(410,300,0,0,&logo,logo_xpm);
	menu.images[3] = logo;

    /// Load the car1
	sprite car_xpm;
	load_sprite(542,500,0,90,&car_xpm,car1);
	menu.images[4] = car_xpm;


    /// Store the cursor
    menu.mouse = cursor;

    return 0;
}

void (load_races) (){
    if (already_loaded_races) return;
    already_loaded_races = true;

    /// Load Circuit 1
	sprite race1_xpm;
	load_sprite(426,160,0,0,&race1_xpm,mini_circuit1);
	races.races[0] = race1_xpm;

	/// Load Circuit 2
	sprite race2_xpm;
	load_sprite(426,564,0,0,&race2_xpm,mini_circuit2);
	races.races[1] = race2_xpm;
}

int (menu_viewer_draw) (){
    load_background(&menu.menu_background);
    for (uint8_t k = 0; k < 5; k++){
        show_xpm(&menu.images[k]);
    }
    show_mouse_cursor(menu.mouse);
    copy_to_video_mem();
    return 0;
}

int (choose_race_draw) (){
    load_races();
    for (uint8_t k = 0; k < 2; k++){
        show_xpm(&races.races[k]);
    }
    show_mouse_cursor(menu.mouse);
    copy_to_video_mem();
    return 0;
}

bool (check_collision) (mouse_sprite* mouse, sprite* object){ 
    if (((mouse->x >= object->x) && (mouse->x <= (object->x + object->info.width))) && ((mouse->y >= object->y) && (mouse->y <= (object->y + object->info.height)))){
        menu.mouse->index = 1;
        return true;
    }
    else {
        menu.mouse->index = 0;
        return false; 
    }
}

Game_State (menu_controller) (int timer_irq, int mouse_irq, int kbd_irq, int rtc_irq, mouse_sprite* cursor){
    if (initialize_menu(cursor) != 0){
        clean_menu();
        return MENU;
    }
    Game_State gs = MENU;
    bool stay = true;
    uint32_t prevCounter = counter;
    while (stay){
        controller(timer_irq, mouse_irq, kbd_irq, rtc_irq);
        if(alarm_out) return CLOSE;
        if (packet_ready){
            organize_packet(&pp);
            move_mouse_cursor(menu.mouse, &pp);
            reset_packets();
        }

        if (scancode_ready){
            reset_scancodes();
        }

        if (prevCounter != counter){
            clean_screen();
            if (gs == SINGLE_PLAYER){
                for (uint8_t k = 0; k < 2; k++){
                    if (check_collision(menu.mouse, &races.races[k])){
                        if (pp.lb == 1){
                            race_id = k + 1;
                            stay = false;
                        }
                        break;
                    }
                }
                choose_race_draw();
            } else {
                for (uint8_t k = 0; k < 3; k++){
                    if (check_collision(menu.mouse, &menu.images[k])){
                        if (pp.lb == 1){
                            pp.lb = 0;
                            switch (k){
                                case 0:
                                    gs = SINGLE_PLAYER;
                                    break;
                                case 1:
                                    gs = RECORDS;
                                    break;
                                case 2:
                                    gs = CLOSE;
                                    break;
                            }
                            if (gs != SINGLE_PLAYER) stay = false;
                        }
                        break; // lb is not pressed so no collision
                    }
                }
                menu_viewer_draw(); 
            }
            prevCounter = counter;
        }
    }
    clean_menu();
    return gs;
}
