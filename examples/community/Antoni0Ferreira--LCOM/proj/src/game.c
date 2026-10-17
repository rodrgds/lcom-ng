#include <lcom/lcf.h>
#include "game.h"

extern uint8_t scanCode;
extern int counter;
uint8_t seconds_boost = 0x00;
int counter_boost = 0;
int draw_b = 0;
int player_boosted = 0;

extern uint8_t irq_kbd_set;
extern uint8_t irq_timer_set;
extern uint8_t irq_mouse_set;
extern uint8_t irq_rtc_set;

static struct packet pp;
static struct cursor cursor;

static struct keyboard_state state;

struct astro astro;

struct shot shot;

static struct shot_animation shot_animation;

static struct player player;
static struct boost boost;
static int points = 0;



int menu(uint8_t *menubg, xpm_image_t imgmenuBG){

    menu_option_sprite(0, menubg);

    initialize_cursor(&cursor);

    bool start_game = true;
    bool left_menu = false;
    message msg;
    int r, ipc_status;
    uint8_t curr_option = 0;
    int exit_menu_counter = 60;
    scanCode = 0x00;

    while ((!left_menu) && exit_menu_counter > 0 && scanCode != ESC) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                if (msg.m_notify.interrupts & irq_timer_set) {
                    timer_int_handler();
                    if ((counter % 1) == 0) {
                        if(cursor.x >= 290 && cursor.x <= 510 && cursor.y >= 270 && cursor.y <= 370){
                            curr_option = 0;
                            if(cursor.LBpressed){
                                if (curr_option == 1) {
                                    start_game = false;
                                }
                                left_menu = true;
                            }
                        }
                        else if(cursor.x >= 315 && cursor.x <= 485 && cursor.y >= 420 && cursor.y <= 520){
                            curr_option = 1;
                            if(cursor.LBpressed){
                                if (curr_option == 1) {
                                    start_game = false;
                                }
                                left_menu = true;
                            }
                        }
                        vg_draw_sprite(menubg,0,0,imgmenuBG);
                        menu_option_sprite(curr_option, menubg);
                        vg_draw_sprite(cursor.sprite, cursor.x, cursor.y, cursor.img);
                        copy_buffer();
                    }
                    if ((counter % 60) == 0) {
                        exit_menu_counter--;
                    }
                }

                if (msg.m_notify.interrupts & irq_kbd_set) {
                    kbc_ih();
                    switch (scanCode) {
                        case W_BREAK_CODE:
                            curr_option = (curr_option + 1) % 2;
                            menu_option_sprite(curr_option, menubg);
                            break;

                        case S_BREAK_CODE:
                            curr_option = (curr_option + 1) % 2;
                            menu_option_sprite(curr_option, menubg);
                            break;

                        case SPACE_MAKE_CODE:
                            if (curr_option == 1) {
                                start_game = false;
                            }
                            left_menu = true;
                            break;

                        default:
                            break;
                    }
                }
                if (msg.m_notify.interrupts & irq_mouse_set) {
                    mouse_ih();

                    if (counter_mouse_ih == 3) {
                        mouse_get_packet(packet_mouse_ih, &pp);
                        cursor.x += pp.delta_x;
                        if(cursor.x < 0){
                            cursor.x = 0;
                        }
                        if(cursor.x > 800 - cursor.img.width){
                            cursor.x = 800 - cursor.img.width;
                        }
                        cursor.y -= pp.delta_y;
                        if(cursor.y < 0){
                            cursor.y = 0;
                        }
                        if(cursor.y > 600 - cursor.img.height){
                            cursor.y =  600 - cursor.img.height;
                        }

                        if(pp.lb != 0){
                            cursor.LBpressed = true;
                        }
                        else{
                            cursor.LBpressed = false;
                        }
                        counter_mouse_ih = 0;
                    }

                    
                }
                break;

                default: break;
            }
        }
    }

    if(!start_game){
        if (mouse_unsubscribe_int())
            return 1;
        if (mouse_disable_data())
            return 1;
        if (keyboard_unsubscribe_int() != 0)
            return 1;
        if (timer_unsubscribe_int() != 0)
            return 1;
        if (rtc_unsubscribe_int() != 0)
            return 1;
        if (vg_exit() != 0)
            return 1;
        return 2;
    }
    
    return 0;
}

int (play)( uint8_t *bg, xpm_image_t imgBG){

    struct astro astros[] = {astro, astro, astro, astro, astro, astro, astro, astro, astro};

    struct shot shots[] = {shot, shot, shot, shot, shot, shot, shot, shot, shot, shot,
                        shot, shot, shot, shot, shot, shot, shot, shot, shot, shot};

    struct shot_animation shot_animations[] = {shot_animation, shot_animation, shot_animation, shot_animation,
                                            shot_animation, shot_animation, shot_animation, shot_animation,
                                            shot_animation, shot_animation, shot_animation, shot_animation};

    message msg;
    int r, ipc_status;
    int game_over_counter = 40;
    int bar_counter = 800;  
    bool shoot = false;

    state.A_DOWN = false;
    state.D_DOWN = false;
    state.S_DOWN = false;
    state.W_DOWN = false;

    initialize_game_sprites(astros, shots, shot_animations, &player, &boost);
    counter = 0;

    while (scanCode != ESC && game_over_counter > 0) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                if (msg.m_notify.interrupts & irq_timer_set) {
                    timer_int_handler();
                    if((counter % 3) == 0 && bar_counter > 0){
                        bar_counter--; 
                    }

                    if ((counter % 1) == 0) {

                        vg_draw_sprite(bg, 0, 0, imgBG);
                        
                        vg_draw_rectangle(0,590,bar_counter,10, 0xf71b39);

                        vg_draw_frame(astros,shots,shot_animations,&player,&boost,&state,&cursor,bg,&points);

                        copy_buffer();
                        
                    }
                    
                    if ((counter % 60) == 0 && counter != 0) {
                        game_over_counter--;
                    }
                    if((counter % 12) == 0){
                        shoot= true;
                    }
                                        
                }

                if (msg.m_notify.interrupts & irq_kbd_set) {
                    kbc_ih();
                    state_scancode(scanCode,&state);

                }
                
                if (msg.m_notify.interrupts & irq_mouse_set) {
                        mouse_ih();

                        if (counter_mouse_ih == 3) {
                            mouse_get_packet(packet_mouse_ih, &pp);

                            if (pp.bytes[0] & BIT(0) && shoot) {
                                for (int i = 0; i < 20; i++) {
                                    if (shots[i].animation == -1 && player.y - 71 > 0) {
                                        shots[i].animation = 0;
                                        shots[i].x = player.x + 2;
                                        shots[i].y = player.y - 50;
                                        vg_draw_sprite(shot_animations[0].sprite, shots[i].x, shots[i].y, shot_animations[0].img);
                                        shoot = false;
                                        break;
                                    }
                                }
                                
                            }
                            cursor.x += pp.delta_x;
                            if(cursor.x < 0){
                                cursor.x = 0;
                            }
                            if(cursor.x > 800 - cursor.img.width){
                                cursor.x = 800 - cursor.img.width;
                            }
                            cursor.y -= pp.delta_y;
                            if(cursor.y < 0){
                                cursor.y = 0;
                            }
                            if(cursor.y > 600 - cursor.img.height){
                                cursor.y =  600 - cursor.img.height;
                            }
                            
                            if(pp.rb != 0){
                                cursor.RBpressed = true;
                            }
                            else{
                                cursor.RBpressed = false;
                            }
                            counter_mouse_ih = 0;
                        }
                    
                }
                
                if(msg.m_notify.interrupts & irq_rtc_set){
                    rtc_ih();
                    printf("seconds - %u\n", seconds_boost);
                    if((seconds_boost % 10) == 0){
                        draw_b = 1;
                    }
                }          

                break;

                default: break;
            }
        }
    }

    return 0;
}

int (end)(){

    message msg;
    int r, ipc_status;

    int highscore = 0;
    read_highscore(&highscore);

    if(points > highscore){
        highscore = points;
        write_highscore(highscore);

        xpm_image_t img_go;
        xpm_map_t map_go = game_over_hs;
        uint8_t *go_sprite = xpm_load(map_go,XPM_8_8_8,&img_go);
        vg_draw_sprite(go_sprite,0,0,img_go);
    }
    else{
        xpm_image_t img_go;
        xpm_map_t map_go = game_over;
        uint8_t *go_sprite = xpm_load(map_go,XPM_8_8_8,&img_go);
        vg_draw_sprite(go_sprite,0,0,img_go);
    }

    draw_scores(points, highscore);

    copy_buffer();


    while(scanCode != ENTER_BREAK_CODE && scanCode != ESC){
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_kbd_set) {
                        printf("%x", scanCode);
                        kbc_ih();
                    }
                    if (msg.m_notify.interrupts & irq_mouse_set) {
                        mouse_ih();
                    }
                    break;
                default:
                    break;
            }
        }
    }
    if(scanCode == ESC){
        return 2;
    }
    points = 0;

    return 0;
}
