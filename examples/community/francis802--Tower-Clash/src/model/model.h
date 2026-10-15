/**
 * @file model.h
 * 
 * @brief This file contains the definition of the SystemState and MenuState enum and the
 * functions to update the game state.
*/

#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/video/graphics.h"
#include "controller/rtc/rtc.h"

#include "xpm_800x600/cursores/cursor_normal.XPM"
#include "xpm_800x600/cursores/cursor_click.XPM"
#include "xpm_800x600/montanhas/montanha1.XPM"
#include "xpm_800x600/montanhas/montanha2.XPM"
#include "xpm_800x600/montanhas/montanha3.XPM"
#include "xpm_800x600/montanhas/montanha4.XPM"
#include "xpm_800x600/nuvens/nuvem_1.XPM"
#include "xpm_800x600/nuvens/nuvem_2.XPM"
#include "xpm_800x600/botoes/name.XPM"
#include "xpm_800x600/botoes/play_button.XPM"
#include "xpm_800x600/botoes/multiplayer_button.XPM"
#include "xpm_800x600/botoes/help_button.XPM"
#include "xpm_800x600/botoes/quit_button.XPM"
#include "xpm_800x600/botoes/resume_button.XPM"
#include "xpm_800x600/botoes/play_again_button.XPM"
#include "xpm_800x600/botoes/menu_button.XPM"
#include "xpm_800x600/botoes/game_over.XPM"
#include "xpm_800x600/botoes/game_paused.XPM"
#include "xpm_800x600/botoes/back_button.XPM"
#include "xpm_800x600/botoes/purple_won.XPM"
#include "xpm_800x600/botoes/orange_won.XPM"
#include "xpm_800x600/torres/roxa_0.XPM"
#include "xpm_800x600/torres/roxa_15.XPM"
#include "xpm_800x600/torres/roxa_30.XPM"
#include "xpm_800x600/torres/roxa_45.XPM"
#include "xpm_800x600/torres/roxa_60.XPM"
#include "xpm_800x600/torres/roxa_75.XPM"
#include "xpm_800x600/torres/roxa_90.XPM"
#include "xpm_800x600/torres/laranja_0.XPM"
#include "xpm_800x600/torres/laranja_15.XPM"
#include "xpm_800x600/torres/laranja_30.XPM"
#include "xpm_800x600/torres/laranja_45.XPM"
#include "xpm_800x600/torres/laranja_60.XPM"
#include "xpm_800x600/torres/laranja_75.XPM"
#include "xpm_800x600/torres/laranja_90.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_01.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_02.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_03.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_04.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_05.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_06.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_07.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_08.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_09.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_10.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_11.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_12.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_13.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_14.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_15.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_16.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_17.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_18.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_19.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_20.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_21.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_22.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_23.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_24.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_25.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_26.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_27.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_28.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_29.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_30.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_31.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_32.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_33.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_34.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_35.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_36.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_37.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_38.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_39.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_40.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_41.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_42.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_43.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_44.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_45.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_46.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_47.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_48.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_49.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_50.XPM"
#include "xpm_800x600/barra_velocidade/barra_velocidade_51.XPM"
#include "xpm_800x600/health_bar/health_bar_00.XPM"
#include "xpm_800x600/health_bar/health_bar_20.XPM"
#include "xpm_800x600/health_bar/health_bar_40.XPM"
#include "xpm_800x600/health_bar/health_bar_60.XPM"
#include "xpm_800x600/health_bar/health_bar_80.XPM"
#include "xpm_800x600/health_bar/health_bar_100.XPM"

#include "xpm_800x600/cannon_ball.XPM"
#include "xpm_800x600/help_page1.XPM"
#include "xpm_800x600/help_page2.XPM"

#include "view/view.h"
#include "model/sprite.h"
#include "config.h"


/**
 * @brief Enum that represents the game states
*/
typedef enum
{
    RUNNING,
    EXIT,
} SystemState;

/**
 * @brief Enum that represents the menu states
*/
typedef enum
{
    START,
    GAME,
    MULTIPLAYER,
    PAUSE,
    HELP,
    END
} MenuState;

/**
 * @brief This function draws a new frame by calling a drawing function from the view.h file.
 * If the double buffer is enabled, it copies the buffer to the VRAM and then draws the
 * new frame.
*/
void update_timer_state();

/**
 * @brief This function updates the game and/or menu state when some specific keys are pressed while
 * the game is in a specific menu and game states.
*/
void update_keyboard_state();

/**
 * @brief This function does the necessary work to process and store the information from the 3 bytes of the mouse packet and
 * then updates not only the game's buttons state, but also the game's own state.
*/
void update_mouse_state();

/**
 * @brief This function updates the game's buttons state by checking if the mouse is over a button and if the left button is pressed.
*/
void update_buttons_state();

/**
 * @brief This function updates the current time by fetching the information from the RTC after a definable timer 0 interrupts.
*/
void update_rtc_state();

/**
 * @brief This function creates all the sprites by calling the create_sprite function from the sprite.h file with
 * the respective XPM location and the sprite's initial position.
*/
void setup_sprites();

/**
 * @brief This function destroys all the sprites by calling the destroy_sprite function from the sprite.h file.
*/
void destroy_sprites();

#endif
