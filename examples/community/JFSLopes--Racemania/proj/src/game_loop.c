#include "game_loop.h"
#include "video_card_macros.h"
#include "assets/pixmaps.h"
#include "controller.h"
#include "i8042.h"
#include "rtc.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

extern struct packet pp;
extern bool packet_ready;
extern uint32_t counter;
extern uint8_t scancodes[2];
extern uint8_t position;
extern bool scancode_ready;
extern bool alarm_out;

static Game game;
static PauseMenu pause_menu;
/**
 * -1 accelerating backwards
 * 0 no acceletation
 * 1 accelerating forwards
 */
static bool pause_menu_loaded = false;
static Move move = NO_MOVEMENT;
static StickMove stickMove = STICK_NO_MOVEMENT; 
static CurrentState state = BEGIN;
static uint8_t race_number;
static uint8_t friction = 3;
static uint16_t stick_init_x;
static uint16_t stick_init_y;
static uint16_t car_x;
static uint16_t car_y;
static xpm_image_t numbers[10];
uint16_t seconds;
uint16_t num_collisions;
static uint16_t initial_angle;
static uint8_t delay = 3;

void (clean_game) (){
    clean_screen(); /// Erases every xpm from the screen
}

int (initialize_game) (mouse_sprite* cursor, uint8_t race_id){
    race_number = race_id;
    ///Load Background 
    sprite backgound_xpm;
	load_sprite(0,0,0,0,&backgound_xpm,background);
	game.background = backgound_xpm;


    /// Load Car and race
	sprite car_xpm;
    sprite race_xpm;

    switch (race_id){
    case 1:
        load_sprite(270,53,0,90,&car_xpm,car1);
        car_x = car_xpm.x;
        car_y = car_xpm.y;
        load_sprite(SCREEN_CAR_POS_X,SCREEN_CAR_POS_Y,0,0,&race_xpm,race_circuit1);
        break;
    
    case 2:
         load_sprite(300,676,0,270,&car_xpm,car1);
        car_x = car_xpm.x;
        car_y = car_xpm.y;
        load_sprite(SCREEN_CAR_POS_X,SCREEN_CAR_POS_Y,0,0,&race_xpm,race_circuit2);
        break;
    }

    initial_angle = car_xpm.angle;
	game.car = car_xpm;
    game.race = race_xpm;

    /// Load stick
    sprite stick_xpm;
    load_sprite(800,650,0,0,&stick_xpm,mouse_stick_xpm);
    stick_init_x = stick_xpm.x;
    stick_init_y = stick_xpm.y;
	game.stick = stick_xpm;

    /// Load stick margin
    sprite margin_xpm;
	load_sprite(779,629,0,0,&margin_xpm,mouse_stick_big_xpm);
	game.stick_margin = margin_xpm;

    ///Load numbers 
    fill_numbers_array();

    ///Load clock
    sprite clock_xpm;
    load_sprite(750,100,0,0,&clock_xpm,clock1);
	game.clock = clock_xpm;

    
    /// Load the pause menu

    game.mouse = cursor;
    return 0;
}

void fill_numbers_array(){
    
    xpm_image_t number;

    xpm_load(number0,XPM_8_8_8_8,&number);
    numbers[0] = number;

    xpm_load(number1,XPM_8_8_8_8,&number);
    numbers[1] = number;

    xpm_load(number2,XPM_8_8_8_8,&number);
    numbers[2] = number;

    xpm_load(number3,XPM_8_8_8_8,&number);
    numbers[3] = number;

    xpm_load(number4,XPM_8_8_8_8,&number);
    numbers[4] = number;

    xpm_load(number5,XPM_8_8_8_8,&number);
    numbers[5] = number;

    xpm_load(number6,XPM_8_8_8_8,&number);
    numbers[6] = number;

    xpm_load(number7,XPM_8_8_8_8,&number);
    numbers[7] = number;

    xpm_load(number8,XPM_8_8_8_8,&number);
    numbers[8] = number;

    xpm_load(number9,XPM_8_8_8_8,&number);
    numbers[9] = number;

    return;
}

void (reset_all_variables) (){
    /// Reset all variables
    state = BEGIN;
    pause_menu_loaded = false;
    stickMove = STICK_NO_MOVEMENT;
    move = NO_MOVEMENT;
    seconds = 0;
    num_collisions = 0;
    game.car.x = car_x;
    game.car.y = car_y;
    game.car.angle = initial_angle;
    game.car.speed = 0;
    delay = 3;
}

Game_State (game_controller) (int timer_irq, int mouse_irq, int kbd_irq, int rtc_irq, mouse_sprite* cursor, uint8_t race_track){
    if (initialize_game(cursor, race_track) != 0){
        return MENU;
    }

    reset_all_variables();
    bool stay = true;
    uint32_t prevCounter = counter;

    uint32_t counter_before_pause = 0;

    while (stay){
        controller(timer_irq, mouse_irq, kbd_irq, rtc_irq);
        if (alarm_out) return CLOSE;

        if (state == DELAY){
            if (counter % FPS == 0) delay--;
            if (delay == 0) stay = false;
        }

        if (state == BEGIN){
            if (scancode_ready || stickMove == STICK_MOVING){
                state = PLAYING;
                counter = 0;
                seconds = 0;
            }
        }
        if (scancode_ready){
            if (scancodes[position] == ESC_BREAKCODE && state != DELAY){ /// Open pause menu
                state = PAUSE_MENU;
            }
            else if (scancodes[position] == W_MAKECODE && state != DELAY){ 
                move = FORWARD;
            }
            else if (scancodes[position] == W_BREAKCODE && state != DELAY){
                move = NO_MOVEMENT;
            }
            else if (scancodes[position] == S_MAKECODE && state != DELAY){
                move = BACKWARD;
            }
            else if (scancodes[position] == S_BREAKCODE && state != DELAY){
                move = NO_MOVEMENT;
            }
            reset_scancodes();
        }
        if (packet_ready){
            organize_packet(&pp);
            move_mouse_cursor(game.mouse, &pp);
            reset_packets();
        }
        if (prevCounter != counter && state != DELAY){
            move_car();
            control_stick();
            clean_screen();
            game_viewer_draw();
            prevCounter = counter;
        }

        if (state == PAUSE_MENU){ /// Check what user entered in the pause menu
            stickMove = STICK_NO_MOVEMENT;
            if (counter_before_pause == 0) counter_before_pause = counter;
            
            state = pause_menu_controller();

            if (state == PLAYING){
                counter = counter_before_pause;
                prevCounter = counter;
                counter_before_pause = 0;
            }
            else if (state == EXIT){
                stay = false;
            }
            else if (state == BEGIN){
                reset_all_variables();
            }
        }

        if (state != BEGIN && state != PAUSE_MENU && state != DELAY){ /// Make sure timer only starts after the user starts playing
            if (counter == FPS){
                seconds++;
                counter = 0;
                prevCounter = 0;
            }
        }

        if (state == DELAY) continue;

        int ret = car_collision(&game.car);
        switch (ret){
        case NO_COLLISION:
            break;

        case CRASH:
            num_collisions++;
            game.car.x = car_x;
            game.car.y = car_y;
            game.car.angle = initial_angle;
            game.car.speed = 0;
            break;

        case FINISH_LINE:
            store_performance();
            break;
        }
    }
    clean_game();
    return MENU;
}

int (game_viewer_draw) (){
    ///Draw background
    load_background(&game.background);
    /// Draw race. Only a part of it
    show_race(&game.race, game.car.x, game.car.y);
    /// Draw stick margin
    show_xpm(&game.stick_margin);
    /// Draw stick
    show_xpm(&game.stick);
    /// Draw car
    show_xpm_at_position(&game.car, SCREEN_CAR_POS_X, SCREEN_CAR_POS_Y);
    //Draw time
    
    uint16_t pos_init_x = 800;
    uint16_t pos_init_y = 100;
    uint16_t index = 100;
    uint16_t time_aux = seconds;
    while(index){
        show_xpm1(&numbers[time_aux/index],pos_init_x,pos_init_y);
        time_aux = time_aux % index;
        index = index / 10;
        pos_init_x += 50;
    }

    ///Draw clock
    show_xpm(&game.clock);
    
    /// Show pause menu
    if (state == PAUSE_MENU){
        pause_menu_draw();
    }

    /// Draw mouse
    if (stickMove != STICK_MOVING) show_mouse_cursor(game.mouse);

    copy_to_video_mem();
    return 0;
}

void (move_car) (){
    if (state == PAUSE_MENU) return;
    /// Ajust the x and y coordinates of the car
    double angle_rad = game.car.angle * (M_PI / 180.0);

    double dx = game.car.speed * sin(angle_rad);
    double dy = game.car.speed * cos(angle_rad);

    // Update coordinates
    game.car.x += (int16_t) dx;
    game.car.y -= (int16_t) dy;

    switch (move){
    case NO_MOVEMENT:
        friction--;
        if (game.car.speed < 0 && !friction){
            game.car.speed++;
        }
        else if (game.car.speed > 0 && !friction){
            game.car.speed--;
        }
        if (friction == 0) friction = 3;
        break;

    case FORWARD:
        if (game.car.speed < 7){
            game.car.speed++;
        }
        break;

    case BACKWARD:
        if (game.car.speed > -7){
            game.car.speed--;
        }
        break;

    default:
        break;
    }
}

void (control_stick) (){
    if (state == PAUSE_MENU) return;
    double dist = 0;
    switch (stickMove){
        case STICK_NO_MOVEMENT:
            if (check_colision(game.mouse, &game.stick)){
                if (pp.lb == 1){
                    stickMove = STICK_MOVING;
                }
            }
        
            break;

        case STICK_MOVING:
            if (pp.lb == 0) stickMove = RESET;
            dist = distance_coordinates(stick_init_x + game.stick.info.width / 2, stick_init_y + game.stick.info.height / 2, game.mouse->x, game.mouse->y);
            if (dist > (game.stick_margin.info.width * 1.0 / 2 - game.stick.info.width * 1.0 / 2)){
                dist = (game.stick_margin.info.width * 1.0 / 2 - game.stick.info.width * 1.0 / 2);

                // Calculate the stick position inside the margin
                update_stick_coordinates(stick_init_x, stick_init_y, game.mouse->x, game.mouse->y, dist);
            }
            else{ // Move the mouse stick to the cursor position
                game.stick.x = game.mouse->x - game.stick.info.width / 2;
                game.stick.y = game.mouse->y - game.stick.info.height / 2;
            }
            break;

        case RESET:
            game.stick.x = stick_init_x;
            game.stick.y = stick_init_y;
            stickMove = STICK_NO_MOVEMENT;
            break;

        default:
            break;
    }
}

bool (check_colision) (mouse_sprite* mouse, sprite* object){
    if (((mouse->x >= object->x) && (mouse->x <= (object->x + object->info.width))) && ((mouse->y >= object->y) && (mouse->y <= (object->y + object->info.height)))){
        game.mouse->index = 1;
        return true;
    }
    else {
        game.mouse->index = 0;
        return false; 
    }
}

double (distance_coordinates) (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

void (update_stick_coordinates) (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, double dist){
    double angle_radians = atan2(y2 - (y1 + game.stick.info.height / 2), x2 - (x1 + game.stick.info.width / 2));
    
    game.stick.x = x1 + dist * cos(angle_radians);
    game.stick.y = y1 + dist * sin(angle_radians);

    
    // Convert to degrees
    double angle_degrees = angle_radians * (180.0 / M_PI);

    // Update the car angle
    game.car.angle = (int16_t) angle_degrees + 90;
}

void (store_performance) () {
    date_time dt;

    if (rtc_get_datetime(&dt) != 0) {
        printf("Error: Failed to get date and time\n");
        return;
    }

    // Open the file for appending
    FILE *file = fopen("/home/lcom/labs/g6/proj/src/assets/records.txt", "a");
    if (file == NULL) {
        printf("Error: Failed to open file for writing\n");
        return;
    }

    fprintf(file, "Race Number: %u\n", race_number);
    fprintf(file, "Time Taken: %u seconds\n", seconds);
    fprintf(file, "Date: %02u-%02u-%02u\n", dt.day, dt.month, dt.year);
    fprintf(file, "Time: %02u:%02u:%02u\n", dt.hours, dt.minutes, dt.seconds);
    fprintf(file, "Number of collisions: %u\n", num_collisions);
    fprintf(file, "-------------------------\n");

    // Close the file
    fclose(file);

    // Show a image
    sprite aux;
    load_sprite(300,200,0,0,&aux,won_xpm);
    clean_screen();
    show_xpm(&aux);
    copy_to_video_mem();
    
    state = DELAY;
}

void (load_pause_menu) (){
    if (pause_menu_loaded) return;
    pause_menu_loaded = true;
    vg_draw_rectangle(376, 132, 400, 600, 0);

    /// Load the resume
	sprite resume_xpm;
	load_sprite(482,261,0,0,&resume_xpm,resume);
	pause_menu.options[0] = resume_xpm;

    /// Load the restart
	sprite restart_xpm;
	load_sprite(470,390,0,0,&restart_xpm,restart);
	pause_menu.options[1] = restart_xpm;

    /// Load the exit
	sprite exit_xpm;
    load_sprite(508,519,0,0,&exit_xpm,exit_big);  
	pause_menu.options[2] = exit_xpm;

}

void (pause_menu_draw) (){
    load_pause_menu();
    vg_draw_rectangle(376, 132, 400, 600, 0);
    for (uint8_t i = 0; i < 3; i++){
        show_xpm(&pause_menu.options[i]);
    }
}

CurrentState (pause_menu_controller) (){
    if (pause_menu_loaded){
        for (uint8_t i = 0; i < 3; i++){
            if (check_colision(game.mouse, &pause_menu.options[i])){
                if (pp.lb == 1){
                    switch (i){
                        case 0:
                            return PLAYING;
                        case 1:
                            return BEGIN;
                        case 2:
                            return EXIT;
                    }
                }
                return PAUSE_MENU;
            }
        }
    }
    return PAUSE_MENU;
}
