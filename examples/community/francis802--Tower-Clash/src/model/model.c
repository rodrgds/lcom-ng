#include "model.h"

extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
MenuState menuState = START;
extern MouseInfo mouse_info;
extern vbe_mode_info_t mode_info;
extern real_time_info time_info;

Sprite *mouse;
Sprite *mouse_click;
Sprite *mountain1;
Sprite *mountain2;
Sprite *mountain3;
Sprite *mountain4;
Sprite *towerO0;
Sprite *towerO1;
Sprite *towerO2;
Sprite *towerO3;
Sprite *towerO4;
Sprite *towerO5;
Sprite *towerO6;
Sprite *towerP0;
Sprite *towerP1;
Sprite *towerP2;
Sprite *towerP3;
Sprite *towerP4;
Sprite *towerP5;
Sprite *towerP6;
Sprite *cloud1;
Sprite *cloud2;
Sprite *cloud3;
Sprite *cloud4;
Sprite *cloud5;
Sprite *cloud6;
Sprite *cloud7;
Sprite *cloud8;
Sprite *healthBar0;
Sprite *healthBar20;
Sprite *healthBar40;
Sprite *healthBar60;
Sprite *healthBar80;
Sprite *healthBar100;
Sprite *gameName;
Sprite *playButton;
Sprite *multiplayerButton;
Sprite *helpButton;
Sprite *quitButton;
Sprite *resumeButton;
Sprite *againButton;
Sprite *menuButton;
Sprite *backButton;
Sprite *gameOver;
Sprite *gamePaused;
Sprite *orangeWon;
Sprite *purpleWon;
Sprite *barV[51];
Sprite *cannon_ball;
Sprite *helpPage1;
Sprite *helpPage2;
extern struct sprite_pos playButton_pos;
extern struct sprite_pos multiplayerButton_pos;
extern struct sprite_pos helpButton_pos;
extern struct sprite_pos quitButton_pos;
extern struct sprite_pos resumeButton_pos;
extern struct sprite_pos menuButton_pos;
extern struct sprite_pos againButton_pos;
extern struct sprite_pos backButton_pos;
extern int HEIGHT_BUTTON;
extern int WIDTH_BUTTON;
extern bool space_pressed;
extern bool prev_pressed_space;
MenuState prevState;


int timer_interrupts = 0;

void load_Vbar()
{
    char **velocity_maps[] = {
        velocity_bar_01, velocity_bar_02, velocity_bar_03, velocity_bar_04, velocity_bar_05, velocity_bar_06, velocity_bar_07, velocity_bar_08, velocity_bar_09, velocity_bar_10,
        velocity_bar_11, velocity_bar_12, velocity_bar_13, velocity_bar_14, velocity_bar_15, velocity_bar_16, velocity_bar_17, velocity_bar_18, velocity_bar_19, velocity_bar_20,
        velocity_bar_21, velocity_bar_22, velocity_bar_23, velocity_bar_24, velocity_bar_25, velocity_bar_26, velocity_bar_27, velocity_bar_28, velocity_bar_29, velocity_bar_30,
        velocity_bar_31, velocity_bar_32, velocity_bar_33, velocity_bar_34, velocity_bar_35, velocity_bar_36, velocity_bar_37, velocity_bar_38, velocity_bar_39, velocity_bar_40,
        velocity_bar_41, velocity_bar_42, velocity_bar_43, velocity_bar_44, velocity_bar_45, velocity_bar_46, velocity_bar_47, velocity_bar_48, velocity_bar_49, velocity_bar_50,
        velocity_bar_51};
    for (int i = 0; i < 51; i++)
    {
        barV[i] = create_sprite_xpm((xpm_map_t)velocity_maps[i]);
    }
}
void destroy_Vbar()
{
    for (int i = 0; i < 51; i++)
    {
        destroy_sprite(barV[i]);
    }
}

void setup_sprites()
{
    mouse = create_sprite_xpm((xpm_map_t)cursor_normal);
    mouse_click = create_sprite_xpm((xpm_map_t)cursor_click);
    towerO0 = create_sprite_xpm((xpm_map_t)Otower0);
    towerO1 = create_sprite_xpm((xpm_map_t)Otower1);
    towerO2 = create_sprite_xpm((xpm_map_t)Otower2);
    towerO3 = create_sprite_xpm((xpm_map_t)Otower3);
    towerO4 = create_sprite_xpm((xpm_map_t)Otower4);
    towerO5 = create_sprite_xpm((xpm_map_t)Otower5);
    towerO6 = create_sprite_xpm((xpm_map_t)Otower6);
    towerP0 = create_sprite_xpm((xpm_map_t)Ptower0);
    towerP1 = create_sprite_xpm((xpm_map_t)Ptower1);
    towerP2 = create_sprite_xpm((xpm_map_t)Ptower2);
    towerP3 = create_sprite_xpm((xpm_map_t)Ptower3);
    towerP4 = create_sprite_xpm((xpm_map_t)Ptower4);
    towerP5 = create_sprite_xpm((xpm_map_t)Ptower5);
    towerP6 = create_sprite_xpm((xpm_map_t)Ptower6);
    mountain1 = create_sprite_xpm((xpm_map_t)mountain1_xpm);
    mountain2 = create_sprite_xpm((xpm_map_t)mountain2_xpm);
    mountain3 = create_sprite_xpm((xpm_map_t)mountain3_xpm);
    mountain4 = create_sprite_xpm((xpm_map_t)mountain4_xpm);
    cloud1 = create_sprite_xpm((xpm_map_t)cloud1_xpm);
    cloud2 = create_sprite_xpm((xpm_map_t)cloud2_xpm);
    cloud3 = create_sprite_xpm((xpm_map_t)cloud1_xpm);
    cloud4 = create_sprite_xpm((xpm_map_t)cloud2_xpm);
    cloud5 = create_sprite_xpm((xpm_map_t)cloud1_xpm);
    cloud6 = create_sprite_xpm((xpm_map_t)cloud2_xpm);
    cloud7 = create_sprite_xpm((xpm_map_t)cloud1_xpm);
    cloud8 = create_sprite_xpm((xpm_map_t)cloud2_xpm);
    healthBar0 = create_sprite_xpm((xpm_map_t)health_bar_00_xpm);
    healthBar20 = create_sprite_xpm((xpm_map_t)health_bar_20_xpm);
    healthBar40 = create_sprite_xpm((xpm_map_t)health_bar_40_xpm);
    healthBar60 = create_sprite_xpm((xpm_map_t)health_bar_60_xpm);
    healthBar80 = create_sprite_xpm((xpm_map_t)health_bar_80_xpm);
    healthBar100 = create_sprite_xpm((xpm_map_t)health_bar_100_xpm);
    gameName = create_sprite_xpm((xpm_map_t)name_xpm);
    playButton = create_sprite_xpm((xpm_map_t)play_button_xpm);
    multiplayerButton = create_sprite_xpm((xpm_map_t)multiplayer_button_xpm);
    helpButton = create_sprite_xpm((xpm_map_t)help_button_xpm);
    quitButton = create_sprite_xpm((xpm_map_t)quit_button_xpm);
    resumeButton = create_sprite_xpm((xpm_map_t)resume_button_xpm);
    againButton = create_sprite_xpm((xpm_map_t)play_again_button_xpm);
    menuButton = create_sprite_xpm((xpm_map_t)menu_button_xpm);
    backButton = create_sprite_xpm((xpm_map_t)back_button_xpm);
    gameOver = create_sprite_xpm((xpm_map_t)game_over_xpm);
    gamePaused = create_sprite_xpm((xpm_map_t)game_paused_xpm);
    helpPage1 = create_sprite_xpm((xpm_map_t)help_page_xpm1);
    helpPage2 = create_sprite_xpm((xpm_map_t)help_page_xpm2);
    orangeWon = create_sprite_xpm((xpm_map_t)orange_won_xpm);
    purpleWon = create_sprite_xpm((xpm_map_t)purple_won_xpm);

    load_Vbar();
    cannon_ball = create_sprite_xpm((xpm_map_t)cannon_ball_xpm);
    cannon_ball->x_pos = 100;
    cannon_ball->y_pos = 100;
    cloud1->x_pos = 100;
    cloud1->y_pos = 100;
    cloud2->x_pos = 265;
    cloud2->y_pos = 165;
    cloud3->x_pos = 450;
    cloud3->y_pos = 216;
    cloud4->x_pos = 654;
    cloud4->y_pos = 34;
    cloud5->x_pos = 729;
    cloud5->y_pos = 256;
    cloud6->x_pos = 476;
    cloud6->y_pos = 40;
    cloud7->x_pos = 19;
    cloud7->y_pos = 216;
    cloud8->x_pos = 634;
    cloud8->y_pos = 178;
    towerO0->x_pos = 650;
    towerO0->y_pos = 470;
    towerP0->x_pos = 70;
    towerP0->y_pos = 470;
}

void destroy_sprites()
{
    destroy_sprite(mouse);
    destroy_sprite(mouse_click);
    destroy_sprite(towerO0);
    destroy_sprite(towerO1);
    destroy_sprite(towerO2);
    destroy_sprite(towerO3);
    destroy_sprite(towerO4);
    destroy_sprite(towerO5);
    destroy_sprite(towerO6);
    destroy_sprite(mountain1);
    destroy_sprite(mountain2);
    destroy_sprite(mountain3);
    destroy_sprite(mountain4);
    destroy_sprite(cloud1);
    destroy_sprite(cloud2);
    destroy_sprite(cloud3);
    destroy_sprite(cloud4);
    destroy_sprite(cloud5);
    destroy_sprite(cloud6);
    destroy_sprite(cloud7);
    destroy_sprite(cloud8);
    destroy_sprite(healthBar0);
    destroy_sprite(healthBar20);
    destroy_sprite(healthBar40);
    destroy_sprite(healthBar60);
    destroy_sprite(healthBar80);
    destroy_sprite(healthBar100);
    destroy_sprite(gameName);
    destroy_sprite(playButton);
    destroy_sprite(multiplayerButton);
    destroy_sprite(helpButton);
    destroy_sprite(quitButton);
    destroy_sprite(resumeButton);
    destroy_sprite(againButton);
    destroy_sprite(menuButton);
    destroy_sprite(backButton);
    destroy_sprite(gameOver);
    destroy_sprite(gamePaused);
    destroy_sprite(gameOver);
    destroy_sprite(gamePaused);
    destroy_sprite(orangeWon);
    destroy_sprite(purpleWon);
    destroy_Vbar();
    destroy_sprite(cannon_ball);
    destroy_sprite(helpPage1);
    destroy_sprite(helpPage2);
}

void update_timer_state()
{
    if (DOUBLE_BUFFER)
        swap_buffers();
    draw_new_frame();
    timer_interrupts++;
}

void update_rtc_state()
{
    if (timer_interrupts % GAME_FREQUENCY == 0)
        rtc_update_time();
}

void update_keyboard_state()
{
    space_pressed = false;
    (kbc_ih)();
    switch (scancode)
    {
    case Q_KEY:
        if (menuState == HELP)
        {
            menuState = START;
        }
        else if (menuState == START)
        {
            systemState = EXIT;
        }
        break;
    case P_KEY:
         if(menuState == START ){
            prevState = GAME;
            menuState = GAME;
         }
         else if(menuState == END ){
            menuState = prevState;
         }
       
        break;    
    case H_KEY:
        if(menuState == START){
            menuState = HELP;
        }
        break;    
    case M_KEY:
        if(menuState == START){
            prevState = MULTIPLAYER;
            menuState = MULTIPLAYER;
        }
        if(menuState == PAUSE || menuState == END){
            menuState = START;
        }
        break;
    case SPACE_KEY:
        if (menuState == GAME || menuState == MULTIPLAYER)
        {
            prev_pressed_space = true;
            space_pressed = true;
        }
        break;

    case ESC_KEY:
        if (menuState == GAME || menuState == MULTIPLAYER)
        {
            menuState = PAUSE;
        }
        else if (menuState == PAUSE)
        {
            menuState = GAME;
        }
        else if(menuState == HELP){
            menuState = START;
        }
        break;
    case R_KEY:
        if(menuState == PAUSE){
            menuState = prevState;
        }    
       break; 
    case B_KEY:
        if(menuState == HELP){
            menuState = START;
        }
        break;    
    default:
        break;
    }
}

void update_mouse_state()
{
    (mouse_ih)();
    mouse_sync_bytes();
    if (byte_index == 3)
    {
        mouse_sync_info();
        update_buttons_state();
        byte_index = 0;
    }
}
void update_start_state()
{

    if (playButton->pressed)
    {
        menuState = GAME;
        prevState = GAME;
        playButton->pressed = 0;
    }
    else if (multiplayerButton->pressed)
    {
        menuState = MULTIPLAYER;
        prevState = MULTIPLAYER;
        multiplayerButton->pressed = 0;
    }

    else if (quitButton->pressed)
    {
        systemState = EXIT;
        quitButton->pressed = 0;
    }
        else if (helpButton->pressed)
    {
        menuState = HELP;
        helpButton->pressed = 0;
    }
}

void update_game_state()
{
    if (resumeButton->pressed)
    {
        menuState = prevState;
        resumeButton->pressed = 0;
    }
    else if (menuButton->pressed)
    {
        menuState = START;
        menuButton->pressed = 0;
    }
}
void update_end_state()
{
    if (menuButton->pressed)
    {
        menuState = START;
        menuButton->pressed = 0;
    }
    else if (againButton->pressed)
    {
        menuState = prevState;
        againButton->pressed = 0;
    }
}
void update_help_state()
{
    if (backButton->pressed)
    {
        menuState = START;
        backButton->pressed = 0;
    }
}
void update_buttons_state()
{
    if (mouse_info.left_click)
    {
        switch (menuState)
        {
           case HELP:
            if (mouse_info.x > backButton_pos.x && mouse_info.y > backButton_pos.y && mouse_info.x < (backButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (backButton_pos.y + HEIGHT_BUTTON))
            {
                backButton->pressed = 1;
            }
            else
            {
                backButton->pressed = 0;
            }
            update_help_state();
            break;
        
        case START:
            if (mouse_info.x > playButton_pos.x && mouse_info.y > playButton_pos.y && mouse_info.x < (playButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (playButton_pos.y + HEIGHT_BUTTON))
            {
                playButton->pressed = 1;
            }
            else if(mouse_info.x > multiplayerButton_pos.x && mouse_info.y > multiplayerButton_pos.y && mouse_info.x < (multiplayerButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (multiplayerButton_pos.y + HEIGHT_BUTTON))
            {
                multiplayerButton->pressed = 1;
            }
            else if (mouse_info.x > quitButton_pos.x && mouse_info.y > quitButton_pos.y && mouse_info.x < (quitButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (quitButton_pos.y + HEIGHT_BUTTON))
            {
                quitButton->pressed = 1;
            }
            else if (mouse_info.x > helpButton_pos.x && mouse_info.y > helpButton_pos.y && mouse_info.x < (helpButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (helpButton_pos.y + HEIGHT_BUTTON))
            {
                helpButton->pressed = 1;
            }
            else
            {
                multiplayerButton->pressed = 0;
                playButton->pressed = 0;
                quitButton->pressed = 0;
                helpButton->pressed = 0;
            }
            update_start_state();
            break;
        case PAUSE:
            if (mouse_info.x > resumeButton_pos.x && mouse_info.y > resumeButton_pos.y && mouse_info.x < (resumeButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (resumeButton_pos.y + HEIGHT_BUTTON))
            {
                resumeButton->pressed = 1;
            }
            else if (mouse_info.x > menuButton_pos.x && mouse_info.y > menuButton_pos.y && mouse_info.x < (menuButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (menuButton_pos.y + HEIGHT_BUTTON))
            {
                menuButton->pressed = 1;
            }
            else
            {
                resumeButton->pressed = 0;
                menuButton->pressed = 0;
            }
            update_game_state();
            break;
        case GAME:
            break;
        case MULTIPLAYER:
            break;    
        case END:
            if (mouse_info.x > againButton_pos.x && mouse_info.y > againButton_pos.y && mouse_info.x < (againButton_pos.x + WIDTH_BUTTON) && mouse_info.y < (againButton_pos.y + HEIGHT_BUTTON))
            {
                againButton->pressed = 1;
            }
            else if (mouse_info.x > menuButton_pos.x && mouse_info.y > menuButton_pos.y && mouse_info.x < menuButton_pos.x + WIDTH_BUTTON && mouse_info.y < menuButton_pos.y + HEIGHT_BUTTON)
            {
                menuButton->pressed = 1;
            }
            else
            {
                againButton->pressed = 0;
                menuButton->pressed = 0;
            }
            update_end_state();
            break;
        }
    }
}
