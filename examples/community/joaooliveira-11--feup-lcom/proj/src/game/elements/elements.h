#ifndef ELEMENTS_H_
#define ELEMENTS_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../../devices/util/KBC.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/graphics/graphics.h"
#include "sprite/sprite.h"
#include "../draw/draw.h"
#include "../context.h"

#include "xpms/game_elements/mouse.xpm"
#include "xpms/screens/background_final.xpm"
#include "xpms/screens/instructions.xpm"
#include "xpms/buttons/button_inst.xpm"
#include "xpms/buttons/button_multi.xpm"
#include "xpms/buttons/button_play.xpm"
#include "xpms/buttons/button_quit.xpm"
#include "xpms/buttons/button_back.xpm"
#include "xpms/screens/game_backgroung.xpm"
#include "xpms/players/fireboy.xpm"
#include "xpms/players/watergirl.xpm"
#include "xpms/game_elements/wall.xpm"
#include "xpms/game_elements/doors.xpm"
#include "xpms/screens/wonMenu.xpm"
#include "xpms/game_elements/barrier.xpm"
#include "xpms/game_elements/lever_left.xpm"
#include "xpms/game_elements/lever_right.xpm"
#include "xpms/game_elements/fire.xpm"
#include "xpms/game_elements/ice.xpm"
#include "xpms/screens/lostMenu.xpm"
#include "xpms/buttons/button_mainMenu.xpm"
#include "xpms/numbers/number0.xpm"
#include "xpms/numbers/number1.xpm"
#include "xpms/numbers/number2.xpm"
#include "xpms/numbers/number3.xpm"
#include "xpms/numbers/number4.xpm"
#include "xpms/numbers/number5.xpm"
#include "xpms/numbers/number6.xpm"
#include "xpms/numbers/number7.xpm"
#include "xpms/numbers/number8.xpm"
#include "xpms/numbers/number9.xpm"

static char **xpms_array[] = {
    mouse_xpm,
    button_play_xpm,
    button_quit_xpm,
    button_inst_xpm,
    button_multi_xpm,
    button_back_xpm,
    button_mainMenu_xpm,
    background_final_xpm,
    instructions_xpm,
    game_backgroung_xpm,
    wonMenu_xpm,
    lostMenu_xpm,
    fireboy_xpm,
    watergirl_xpm,
    wall_xpm,
    doors_xpm,
    barrier_xpm,
    lever_left_xpm,
    lever_right_xpm,
    fire_xpm,
    ice_xpm
};


static char **numbersArray[] = {
    number0_xpm,
    number1_xpm,
    number2_xpm,
    number3_xpm,
    number4_xpm,
    number5_xpm,
    number6_xpm,
    number7_xpm,
    number8_xpm,
    number9_xpm
};

///@brief Reads the game map from a file and initializes the game elements accordingly.
///@return 0 if successful, 1 otherwise.
int read_map();

///@brief Allocates memory and builds the mouse sprite.
void allocate_mouse();

///@brief Allocates memory and builds the button sprites.
void allocate_buttons();

///@brief Allocates memory and builds the screen sprites.
void allocate_screens();

///@brief Allocates memory and builds the player sprites.
void allocate_players();

///@brief Allocates memory and builds the game element sprites.
///@note Also reads the game map and initializes the game elements.
void allocate_game_elements();

///@brief Allocates memory and builds the number sprites.
void allocate_numbers();

///@brief Deletes the screen sprites.
void delete_screens();

///@brief Deletes the game element sprites and frees the memory allocated for game elements.
void delete_game_elements();

///@brief Deletes the player sprites.
void delete_players();

///@brief Deletes the button sprites.
void delete_buttons();

///@brief Deletes the number sprites.
void delete_numbers();

#endif
