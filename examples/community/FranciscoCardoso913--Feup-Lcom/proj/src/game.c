#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "game.h"
#include "maps/map.h"
#include "count_down/count_down.h"
#include "drivers/rtc/rtc.h"
#include "drivers/serial_port/ser.h"
#include "characters/character.h"
#include "communication/communication.h"
#include "drivers/serial_port/uart.h"

int frames_per_second = 60;
int current_frame = 0;
extern Map* current_map;
extern Map* map1;
uint8_t irq_mouse, irq_timer, irq_keyboard, irq_ser;
static bool control_fireboy = true, control_watergirl = true, multiplayer = false;
static int player;

int game_init(int player_number) {
    player = player_number;

    if (video_mode()) {
        printf("Error setting video mode.\n");
        return 1;
    }

    if (timer_subscribe_int(&irq_timer)) {
        printf("Error subscribing timer interrupts.\n");
        return 1;
    }

    timer_set_frequency(0, 60);


    if (kbc_init(&irq_keyboard, &irq_mouse)) {
        printf("Error initializing kbc.\n");
        return 1;
    }

    if (load_sprites()) {
        printf("Error loading cursor.\n");
        return 1;
    }
    if (load_maps()) {
        printf("Error loading maps.\n");
        return 1;
    }

    if (create_characters()) {
        printf("Error loading characters.\n");
        return 1;
    }

    if (rtc_init()) return 1;

    if (ser_init()) {
        printf("Error initializing serial port\n");
        return 1;
    }

    if (ser_subscribe_int(&irq_ser)) {
        printf("Error subscribing serial port interrupts.\n");
        return 1;
    }

    return 0;
}

int game_exit() {
    if (kbc_disable() != OK) {
        printf("Error disabling kbc.\n");
        return 1;
    }

    if (timer_unsubscribe_int()) {
        printf("Error unsubscribing timer interrupts.\n");
        return 1;
    }

    if (text_mode() != OK) {
        printf("Error exiting video mode.\n");
        return 1;
    }

    if (ser_unsubscribe_int()) {
        printf("Error unsubscribing serial port interrupts.\n");
        return 1;
    }

    return 0;
}

game_state state = MAIN_MENU;

int game_loop() {
    int ipc_status, r;
    message msg;


    do {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("Error receiving message: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_mouse) {
                    mouse_ih();
                    if (!mouse_packet.complete) continue;

                    cursor->x = mouse_packet.x;
                    cursor->y = mouse_packet.y;
                }

                if (msg.m_notify.interrupts & irq_timer) {
                    timer_ih();
                    if(state==GAME){
                        if ((current_frame++) == frames_per_second) {
                            if(  decrement_counter()){
                                state=GAME_OVER;
                                send_game_over();
                                if (check_transmitter()) return 1;
                                if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
                            }
                            current_frame = 0;
                        }
                        if((on_fire(watergirl) || on_water(fireboy)) || on_poison(fireboy) || on_poison(watergirl) ){ 
                            state=GAME_OVER;
                            stop_moving(fireboy);
                            stop_moving(watergirl);
                            send_game_over();
                            if (check_transmitter()) return 1;
                            if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
                        }
                        if(door_fire(fireboy) && door_water(watergirl)){
                            send_next_level();
                            if (check_transmitter()) return 1;
                            if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
                            reset_falling_blocks();
                            clear_background();
                            if (nextLevel()) {
                                exit_multiplayer();
                                state = MAIN_MENU;
                                control_fireboy = control_watergirl = true;
                            }
                            else{
                                start_counter(120);
                                draw_map(current_map);
                                if( current_map==map1 || current_map==map3){
                                    fireboy->sprite->x=100;
                                    fireboy->sprite->y=750;
                                    watergirl->sprite->x= 100;
                                    watergirl->sprite->y=750;
                                }
                                if( current_map== map2){
                                    fireboy->sprite->x=60;
                                    fireboy->sprite->y=750;
                                    watergirl->sprite->x= 1000;
                                    watergirl->sprite->y=750;
                                }
                            
                            }
                        }
                        update_character(fireboy);
                        update_character(watergirl);
                    }

                    if (state == WAITING_PLAYER && !multiplayer) {
                        if (check_connection()) {
                            if (check_transmitter()) return 1;
                            if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
                            send_ready_connection();
                            start_multiplayer();
                            clear_queues();
                            clear_fifos();
                            start_counter(120);
                            clear_background();
                            current_map=map1;
                            fireboy->sprite->x=100;
                            fireboy->sprite->y=750;
                            watergirl->sprite->x= 100;
                            watergirl->sprite->y=750;
                            draw_map(current_map);
                            state = GAME;
                        }
                    }

                    if (multiplayer && (state == GAME || state == PAUSE_MENU || state == GAME_OVER)) handle_remote_player();

                    if (check_transmitter()) return 1;

                    if (transmitter_ready() && !send_queue_is_empty()) {
                        send_bytes();
                    }

                    while (true) {
                        uint8_t lsr, data;

                        read_lsr(&lsr);

                        if (!(lsr & LSR_RECEIVED_DATA))
                            break;
                        
                        read_byte(&data);
                        
                        receive_queue_push(data);
                    }

                    update_character(fireboy);
                    update_character(watergirl);
                    draw_screen();
                }

                if (msg.m_notify.interrupts & irq_keyboard) {
                    keyboard_ih();
                    if (!keyboard_packet.complete) continue;
                    keyboard_key k = keyboard_get_key();

                    if (k == KEY_ESC) {
                        if (state == MAIN_MENU) {
                            state = EXIT; 
                            send_exit_game();
                            if (check_transmitter()) return 1;
                            if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
                        }
                        else if (state == GAME) {
                            state = PAUSE_MENU; 
                            send_pause_game();
                            if (check_transmitter()) return 1;
                            if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
                        }
                        else if (state == WAITING_PLAYER) state = MAIN_MENU;
                        else {
                            exit_multiplayer();
                            state = MAIN_MENU;
                            control_fireboy = control_watergirl = true;
                            clear_background();
                        }
                        break;
                    }

                    if (state == GAME && control_fireboy) fireboy_move(k);
                    if (state == GAME && control_watergirl) watergirl_move(k);

                    if (multiplayer) send_keyboard_key(k);
                }

                if (msg.m_notify.interrupts & irq_ser) {
                    ser_ih();

                    if (check_ih_err()) {
                        printf("Error?\n");
                        break;
                    }
                }
            default:
                break;
            }
        }
    } while (state != EXIT);

    return 0;
}

int draw_screen() {
    switch (state) {
    case MAIN_MENU:
        draw_main_menu();
        break;
    case WAITING_PLAYER:
        draw_waiting_player_background();
        break;
    case GAME:
        draw_game();
        break;
    case PAUSE_MENU:
        draw_pause_menu();
        break;
    case GAME_OVER:
        draw_game_over();
        break;
    default:
        break;
    }

    return 0;
}

int draw_main_menu() {
    draw_sprite(background);
    draw_sprite(logo);

    Sprite* single_player = mouse_over_sprite(single_player_text) ? single_player_texth : single_player_text;
    Sprite* coop = mouse_over_sprite(coop_text) ? coop_texth : coop_text;
    Sprite* exit = mouse_over_sprite(exit_text) ? exit_texth : exit_text;

    if (mouse_lclick_sprite(single_player)) {
        
        state = GAME;
        current_map=map1;
        fireboy->sprite->x=100;
        fireboy->sprite->y=750;
        watergirl->sprite->x= 100;
        watergirl->sprite->y=750;
        
        start_counter(120);
        clear_background();
       
        draw_map(current_map);
     
        reset_falling_blocks();
     
        return 0;
    }
    if (mouse_lclick_sprite(coop)) {
        state = WAITING_PLAYER;
        send_start_connection();
        if (check_transmitter()) return 1;
        if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
        
        return 0;
    }
    if (mouse_lclick_sprite(exit)) {
        state = EXIT;
        return 0;
    }

    draw_sprite(single_player);
    draw_sprite(coop);
    draw_sprite(exit);
    draw_sprite(cursor);

    draw_buffer();

    erase_sprite(logo);
    erase_sprite(single_player);
    erase_sprite(coop);
    erase_sprite(exit);
    erase_sprite(cursor);

    return 0;
}

int draw_waiting_player_background() {
    /*if (mouse_inside(420, 540, 240, 80) && mouse_packet.lb) {
       state=MAIN_MENU;
       clear_background();
       printf("State: %d\n", state);
    }*/

    draw_sprite(waiting);
    draw_sprite(cursor);
    draw_buffer();

    erase_sprite(waiting);
    erase_sprite(cursor);
    clear_background();
    
    return 0;
}

void draw_ropes() {
  
    for (int i = 0; i < current_map->n_blocks; i++) {
        
        if(!current_map->blocks[i] ) continue;
       
        if(current_map->blocks[i]->is_cut) continue;
        for (int j = 0; j < current_map->blocks[i]->n_rope; j++) {
           
            if (!current_map->blocks[i]->rope[j] ) continue;
           
            draw_sprite(current_map->blocks[i]->rope[j]);
            
            int x_distf = abs(current_map->blocks[i]->rope[j]->x - fireboy->sprite->x);
            int y_distf = abs(current_map->blocks[i]->rope[j]->y - fireboy->sprite->y);
            int x_distw = abs(current_map->blocks[i]->rope[j]->x - watergirl->sprite->x);
            int y_distw = abs(current_map->blocks[i]->rope[j]->y - watergirl->sprite->y);
            
            if ((x_distf > 150 || y_distf > 150) &&
                (x_distw > 150 || y_distw > 150)) {
                continue;
            }
          
            if (mouse_lclick_sprite(current_map->blocks[i]->rope[j])) {
                current_map->blocks[i]->is_cut=true;
                for(int z=0; z<current_map->blocks[i]->n_rope;z++){
                    erase_sprite( current_map->blocks[i]->rope[z]);
                }
            }
        }
    }
}


int draw_game() {
    draw_sprite(cursor);
    move(fireboy);
    move(watergirl);
    draw_character(fireboy);
    draw_character(watergirl);
    
    draw_ropes();
   
    draw_falling_blocks();
  
    draw_sprite(cursor);

    draw_buffer();
    erase_sprite(cursor);
    erase_sprite(fireboy->sprite);
    erase_sprite(watergirl->sprite);
  
    erase_blocks();
    

    
    draw_timer();
    draw_time();
    draw_date();

    return 0;
}

int draw_pause_menu() {
     if (mouse_inside(310, 452, 240, 80) && mouse_packet.lb) {
        exit_game();
        if (check_transmitter()) return 1;
        if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
    }

    if (mouse_inside(663, 452, 240, 80) && mouse_packet.lb) {
        restart_game();
        if (check_transmitter()) return 1;
        if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
    }
    if (mouse_inside(468, 579, 240, 80) && mouse_packet.lb) {
        state = GAME;
        send_resume_game();
        if (check_transmitter()) return 1;
        if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
        return 0;
    }

    draw_sprite(pause_menu);
    draw_sprite(cursor);
    draw_buffer();

    erase_sprite(pause_menu);
    erase_sprite(cursor);

    return 0;
}

int draw_game_over() {

    if (mouse_inside(190, 540, 240, 80) && mouse_packet.lb) {
        exit_game();
        if (check_transmitter()) return 1;
        if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
    }

    if (mouse_inside(770, 540, 240, 80) && mouse_packet.lb) {
        restart_game();
        if (check_transmitter()) return 1;
        if (transmitter_ready() && !send_queue_is_empty()) send_bytes();
    }

    draw_sprite(game_over);
    draw_sprite(cursor);
    draw_buffer();

    erase_sprite(game_over);
    erase_sprite(cursor);

    return 0;
}

int fireboy_move(keyboard_key key) {
    switch (key) {
    case KEY_A:
        move_left(fireboy);
        break;
    case KEY_W:
        jump(fireboy);
        break;
    case KEY_D:
        move_right(fireboy);
        break;
    case KEY_A_BREAK: case KEY_D_BREAK:
        stop_moving(fireboy);
        break;
    default:
        break;
    }

    return 0;
}

int watergirl_move(keyboard_key key) {
    switch (key) {
    case KEY_LEFT:
        move_left(watergirl);
        break;
    case KEY_UP:
        jump(watergirl);
        break;
    case KEY_RIGHT:
        move_right(watergirl);
        break;
    case KEY_LEFT_BREAK: case KEY_RIGHT_BREAK:
        stop_moving(watergirl);
        break;
    default:
        break;
    }

    return 0;
}

void exit_multiplayer() {
    clear_queues();
    control_fireboy = control_watergirl = true;
}

void handle_remote_player() {
    micro_delay(500);
    while (!receive_queue_is_empty()) {
        //printf("Queue front: %x\n", receive_queue_front());
        switch(receive_queue_front()) {
            case KEYBOARD_ID: {
                uint8_t key;
                if (receive_queue_size() < 2) {
                    printf("Key hasn't been received\n");
                    return;
                }

                receive_queue_pop();

                key = receive_queue_pop();

                if (player == 1){
                    watergirl_move(key);
                }
                else {
                    fireboy_move(key);
                }

                break;
            }

            case CHANGE_ID: {
                uint8_t change;
                if (receive_queue_size() < 2) {
                    printf("Change hasn't been received\n");
                    return;
                }

                receive_queue_pop();

                change = receive_queue_pop();

                if (change == GAME_OVER_ID){
                    printf("Got here\n");
                    state = GAME_OVER;
                    stop_moving(fireboy);
                    stop_moving(watergirl);
                }
                else if (change == RESTART_GAME) {
                    restart_game();
                }
                else if (change == RESUME_GAME) {
                    state = GAME;
                }
                else if (change == EXIT_GAME) {
                    exit_game();
                }
                else if (change == NEXT_LEVEL) {
                    reset_falling_blocks();
                    clear_background();
                    if (nextLevel()) {
                        exit_multiplayer();
                        state = MAIN_MENU;
                        control_fireboy = control_watergirl = true;
                    }
                    else{
                        start_counter(120);
                        draw_map(current_map);
                        if( current_map==map1 || current_map==map3){
                            fireboy->sprite->x=100;
                            fireboy->sprite->y=750;
                            watergirl->sprite->x= 100;
                            watergirl->sprite->y=750;
                        }
                        if( current_map== map2){
                            fireboy->sprite->x=60;
                            fireboy->sprite->y=750;
                            watergirl->sprite->x= 1000;
                            watergirl->sprite->y=750;
                        }
                    
                    }
                }
                else if (change == PAUSE_GAME) {
                    state = PAUSE_MENU;
                }

                break;
            }

            default:
                receive_queue_pop();
                break;
        }
    }
}

void start_multiplayer() {
    if (player == 1)
        control_watergirl = false;
    else
        control_fireboy = false;
    multiplayer = true;
}

int exit_game() {
    if (multiplayer) exit_multiplayer();
    
    state = MAIN_MENU;
    reset_falling_blocks();
    clear_background();
    send_exit_game();
    return 0;
}

int restart_game() {
    state = GAME;
    reset_falling_blocks();
    clear_background();
    start_counter(120);
    if(current_map==map1 || current_map==map3){
        
        fireboy->sprite->x=100;
        fireboy->sprite->y=750;
        watergirl->sprite->x= 100;
        watergirl->sprite->y=750;
        
    }
    else if (current_map == map2) {
        fireboy->sprite->x = 60;
        fireboy->sprite->y = 750;
        watergirl->sprite->x = 1000;
        watergirl->sprite->y = 750;
    }
    clear_background();
    draw_map(current_map);
    send_restart_game();
    return 0;
}
