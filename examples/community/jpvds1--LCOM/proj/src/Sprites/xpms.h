/**

@file xpms.h
@brief Header file containing declarations of functions and variables used to load all the .xpm files.

*/
#ifndef XPMS_H
#define XPMS_H

#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>

#include "xpms/others/nave.xpm"
#include "xpms/explosion/e1.xpm"
#include "xpms/explosion/e2.xpm"
#include "xpms/explosion/e3.xpm"
#include "xpms/explosion/e4.xpm"
#include "xpms/explosion/e5.xpm"
#include "xpms/explosion/e6.xpm"
#include "xpms/explosion/e7.xpm"
#include "xpms/enemy/enemy1.xpm"
#include "xpms/enemy/enemy2.xpm"
#include "xpms/enemy/enemy3.xpm"
#include "xpms/bullet_enemy/bullet_enemy1.xpm"
#include "xpms/bullet_enemy/bullet_enemy2.xpm"
#include "xpms/bullet_enemy/bullet_enemy3.xpm"
#include "xpms/life/sem_vida.xpm"
#include "xpms/life/vida.xpm"
#include "xpms/Numbers/zero.xpm"
#include "xpms/Numbers/one.xpm"
#include "xpms/Numbers/two.xpm"
#include "xpms/Numbers/three.xpm"
#include "xpms/Numbers/four.xpm"
#include "xpms/Numbers/five.xpm"
#include "xpms/Numbers/six.xpm"
#include "xpms/Numbers/seven.xpm"
#include "xpms/Numbers/eight.xpm"
#include "xpms/Numbers/nine.xpm"
#include "xpms/mouse_cursor.xpm"
#include "xpms/Menu/Alien_attacks_logo.xpm"
#include "xpms/Menu/endless.xpm"
#include "xpms/Menu/endless_selected.xpm"
#include "xpms/Menu/exit.xpm"
#include "xpms/Menu/exit_selected.xpm"
#include "xpms/Menu/instructions.xpm"
#include "xpms/Menu/instructions_selected.xpm"
#include "xpms/Menu/levels.xpm"
#include "xpms/Menu/levels_selected.xpm"
#include "xpms/Menu/inside_instructions.xpm"
#include "xpms/Menu/menu.xpm"
#include "xpms/Menu/menu_selected.xpm"
#include "xpms/chars/back_slash.xpm"
#include "xpms/chars/hifen.xpm"
#include "xpms/missil/missil1.xpm"
#include "xpms/missil/missil2.xpm"
#include "xpms/missil/missil3.xpm"
#include "xpms/missil/missil4.xpm"
#include "xpms/missil/missil5.xpm"
#include "xpms/missil/missil6.xpm"
#include "xpms/missil/missil7.xpm"
#include "xpms/missil/missil8.xpm"
#include "xpms/missil/missil9.xpm"
#include "xpms/missil/missil10.xpm"
#include "xpms/missil/missil11.xpm"
#include "xpms/missil/missil12.xpm"
#include "xpms/missil/missil13.xpm"
#include "xpms/missil/missil14.xpm"
#include "xpms/missil/missil15.xpm"
#include "xpms/missil/missil16.xpm"
#include "xpms/missil/missil17.xpm"
#include "xpms/missil/missil18.xpm"
#include "xpms/missil/missil19.xpm"
#include "xpms/missil/missil20.xpm"
#include "xpms/missil/missil21.xpm"
#include "xpms/missil/missil22.xpm"
#include "xpms/missil/missil23.xpm"
#include "xpms/missil/missil24.xpm"
#include "xpms/missil/nuke.xpm"


extern char* nave_xpm[];
extern uint16_t *user_sprite;
extern xpm_image_t user_img;

extern char* e1_xpm[];
extern uint16_t *explosion1_sprite;
extern xpm_image_t explosion1_img;
extern char* e2_xpm[];
extern uint16_t *explosion2_sprite;
extern xpm_image_t explosion2_img;
extern char* e3_xpm[];
extern uint16_t *explosion3_sprite;
extern xpm_image_t explosion3_img;
extern char* e4_xpm[];
extern uint16_t *explosion4_sprite;
extern xpm_image_t explosion4_img;
extern char* e5_xpm[];
extern uint16_t *explosion5_sprite;
extern xpm_image_t explosion5_img;
extern char* e6_xpm[];
extern uint16_t *explosion6_sprite;
extern xpm_image_t explosion6_img;
extern char* e7_xpm[];
extern uint16_t *explosion7_sprite;
extern xpm_image_t explosion7_img;

extern char * bullet_enemy1_xpm[];
extern uint16_t *eb1_sprite;
extern xpm_image_t eb1_img;
extern char * bullet_enemy2_xpm[];
extern uint16_t *eb2_sprite;
extern xpm_image_t eb2_img;
extern char * bullet_enemy3_xpm[];
extern uint16_t *eb3_sprite;
extern xpm_image_t eb3_img;

extern char * enemy1_xpm[];
extern uint16_t *enemy1_sprite;
extern xpm_image_t enemy1_img;
extern char * enemy2_xpm[];
extern uint16_t *enemy2_sprite;
extern xpm_image_t enemy2_img;
extern char * enemy3_xpm[];
extern uint16_t *enemy3_sprite;
extern xpm_image_t enemy3_img;

extern char * vida_xpm[];
extern uint16_t *vida_sprite;
extern xpm_image_t vida_img;
extern char * sem_vida_xpm[];
extern uint16_t *sem_vida_sprite;
extern xpm_image_t sem_vida_img;

extern char * zero_xpm[];
extern uint16_t *zero_sprite;
extern xpm_image_t zero_img;
extern char * one_xpm[];
extern uint16_t *one_sprite;
extern xpm_image_t one_img;
extern char * two_xpm[];
extern uint16_t *two_sprite;
extern xpm_image_t two_img;
extern char * three_xpm[];
extern uint16_t *three_sprite;
extern xpm_image_t three_img;
extern char * four_xpm[];
extern uint16_t *four_sprite;
extern xpm_image_t four_img;
extern char * five_xpm[];
extern uint16_t *five_sprite;
extern xpm_image_t five_img;
extern char * six_xpm[];
extern uint16_t *six_sprite;
extern xpm_image_t six_img;
extern char * seven_xpm[];
extern uint16_t *seven_sprite;
extern xpm_image_t seven_img;
extern char * eight_xpm[];
extern uint16_t *eight_sprite;
extern xpm_image_t eight_img;
extern char * nine_xpm[];
extern uint16_t *nine_sprite;
extern xpm_image_t nine_img;

extern char * levels_xpm[];
extern uint16_t *levels_sprite;
extern xpm_image_t levels_img;
extern char * levels_sel_xpm[];
extern uint16_t *levels_sel_sprite;
extern xpm_image_t levels_sel_img;
extern char * instructions_xpm[];
extern uint16_t *instructions_sprite;
extern xpm_image_t instructions_img;
extern char * instructions_sel_xpm[];
extern uint16_t *instructions_sel_sprite;
extern xpm_image_t instructions_sel_img;
extern char * exit_xpm[];
extern uint16_t *exit_sprite;
extern xpm_image_t exit_img;
extern char * exit_sel_xpm[];
extern uint16_t *exit_sel_sprite;
extern xpm_image_t exit_sel_img;
extern char * endless_xpm[];
extern uint16_t *endless_sprite;
extern xpm_image_t endless_img;
extern char * endless_sel_xpm[];
extern uint16_t *endless_sel_sprite;
extern xpm_image_t endless_sel_img;
extern char * Alien_attacks_logo_xpm[];
extern uint16_t *logo_sprite;
extern xpm_image_t logo_img;
extern char * inside_instructions_xpm[];
extern uint16_t *inside_instructions_sprite;
extern xpm_image_t inside_instructions_img;

extern char * hifen_xpm[];
extern uint16_t *hifen_sprite;
extern xpm_image_t hifen_img;
extern char * back_slash_xpm[];
extern uint16_t *back_slash_sprite;
extern xpm_image_t back_slash_img;

extern char * missil1_xpm[];
extern char * missil2_xpm[];
extern char * missil3_xpm[];
extern char * missil4_xpm[];
extern char * missil5_xpm[];
extern char * missil6_xpm[];
extern char * missil7_xpm[];
extern char * missil8_xpm[];
extern char * missil9_xpm[];
extern char * missil10_xpm[];
extern char * missil11_xpm[];
extern char * missil12_xpm[];
extern char * missil13_xpm[];
extern char * missil14_xpm[];
extern char * missil15_xpm[];
extern char * missil16_xpm[];
extern char * missil17_xpm[];
extern char * missil18_xpm[];
extern char * missil19_xpm[];
extern char * missil20_xpm[];
extern char * missil21_xpm[];
extern char * missil22_xpm[];
extern char * missil23_xpm[];
extern char * missil24_xpm[];
extern uint16_t *missile1_sprite;
extern uint16_t *missile2_sprite;
extern uint16_t *missile3_sprite;
extern uint16_t *missile4_sprite;
extern uint16_t *missile5_sprite;
extern uint16_t *missile6_sprite;
extern uint16_t *missile7_sprite;
extern uint16_t *missile8_sprite;
extern uint16_t *missile9_sprite;
extern uint16_t *missile10_sprite;
extern uint16_t *missile11_sprite;
extern uint16_t *missile12_sprite;
extern uint16_t *missile13_sprite;
extern uint16_t *missile14_sprite;
extern uint16_t *missile15_sprite;
extern uint16_t *missile16_sprite;
extern uint16_t *missile17_sprite;
extern uint16_t *missile18_sprite;
extern uint16_t *missile19_sprite;
extern uint16_t *missile20_sprite;
extern uint16_t *missile21_sprite;
extern uint16_t *missile22_sprite;
extern uint16_t *missile23_sprite;
extern uint16_t *missile24_sprite;
extern xpm_image_t missile1_img;
extern xpm_image_t missile2_img;
extern xpm_image_t missile3_img;
extern xpm_image_t missile4_img;
extern xpm_image_t missile5_img;
extern xpm_image_t missile6_img;
extern xpm_image_t missile7_img;
extern xpm_image_t missile8_img;
extern xpm_image_t missile9_img;
extern xpm_image_t missile10_img;
extern xpm_image_t missile11_img;
extern xpm_image_t missile12_img;
extern xpm_image_t missile13_img;
extern xpm_image_t missile14_img;
extern xpm_image_t missile15_img;
extern xpm_image_t missile16_img;
extern xpm_image_t missile17_img;
extern xpm_image_t missile18_img;
extern xpm_image_t missile19_img;
extern xpm_image_t missile20_img;
extern xpm_image_t missile21_img;
extern xpm_image_t missile22_img;
extern xpm_image_t missile23_img;
extern xpm_image_t missile24_img;

extern char * menu_xpm[];
extern char * menu_selected_xpm[];
extern char * nuke_xpm[];
extern uint16_t *menu_sprite;
extern uint16_t *menu_selected_sprite;
extern uint16_t *nuke_sprite;
extern xpm_image_t menu_img;
extern xpm_image_t menu_selected_img;
extern xpm_image_t nuke_img;

/**
 * @brief loads all xpm files into their respective uint16_t* sprite and xpm_image_t img
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int load_xpm();

#endif //XPMS_H
