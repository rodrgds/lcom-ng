#include <lcom/lcf.h>
#include "initiate.h"
#include "mainmenu.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "video.h"
#include "sprite.h"
#include "rtc.h"
#include "i8042.h"
#include "xpm.h"

//Sprites
Sprite *background;
Sprite *cursor;
Sprite *man;
Sprite *vida_;
Sprite *number;
Sprite *mini_number;
Sprite *play;
Sprite *singleplayer_but;
Sprite *instructions_but;
Sprite *back_but;
Sprite *pause_but;
Sprite *continue_but;
Sprite *exit_but;
Sprite *exit_mini_but;
Sprite *menu_mini_but;
Sprite *loading_;

//bit_no
uint8_t kbd_bit_no;
uint8_t mouse_bit_no;
uint8_t timer_bit_no;
uint8_t rtc_bit_no;

//images to array
xpm_image_t backgrounds[9];
xpm_image_t cursors[2];
xpm_image_t player[3];
xpm_image_t fruits_images[10];
xpm_image_t numbers[10];
xpm_image_t numbers_mini[11];
xpm_image_t buttons[17];
xpm_image_t loading[12];
xpm_image_t vidas;

//functions needed to iniciate program
int inicialize_minix() {

    if (vg_init(VBE_MODE) == NULL) {
        printf("Error in vg_init()\n");
        return 1;
    }
    if (timer_subscribe_int(&timer_bit_no) != OK) {
        printf("Error in timer_subscribe_int()\n");
        return 1;
    }
    if (kbd_subscribe_int(&kbd_bit_no) != OK) {
        printf("Error in kbd_subscribe_int()\n");
        return 1;
    }
    if (mouse_enable_data_reporting2(ENABLE) != OK) {
        printf("Error in mouse_enable_data_reporting2()\n");
        return 1;
    }
    if (mouse_subscribe_int(&mouse_bit_no) != OK) {
        printf("Error in mouse_subscribe_int()\n");
        return 1;
    }
    if (rtc_subscribe_int(&rtc_bit_no) != OK) {
        printf("Error in mouse_subscribe_int()\n");
        return 1;
    }
    if (enable_or_disable_rtc(true)) {
        printf("Error in mouse_subscribe_int()\n");
        return 1;
    }

    return 0;
}

//functions needed to exit program
int exit_minix() {
    if (mouse_enable_data_reporting2(DISABLE) != OK) {
        printf("Error in mouse_enable_data_reporting2()\n");
        return 1;
    }
    if (enable_or_disable_rtc(false)) {
        printf("Error in mouse_subscribe_int()\n");
        return 1;
    }
    if (timer_unsubscribe_int() != OK) {
        printf("Error in timer_unsubscribe_int()\n");
        return 1;
    }    
    if (mouse_unsubscribe_int() != OK) {
        printf("Error in mouse_unsubscribe_int()\n");
        return 1;
    }
    if (kbd_unsubscribe_int() != OK) {
        printf("Error in timer_unsubscribe_int()\n");
        return 1;
    }
    if (rtc_unsubscribe_int() != OK) {
        printf("Error in timer_unsubscribe_int()\n");
        return 1;
    }
  
    vg_exit();  
    return 0;
}

//loads all images
void load_xpms(){
  xpm_load(apple_xpm, XPM_8_8_8_8, &fruits_images[0]);
  xpm_load(carrot_xpm, XPM_8_8_8_8, &fruits_images[1]);
  xpm_load(cherries_xpm, XPM_8_8_8_8,&fruits_images[2]);
  xpm_load(donuts_xpm, XPM_8_8_8_8, &fruits_images[3]);
  xpm_load(greaps_xpm, XPM_8_8_8_8, &fruits_images[4]);
  xpm_load(hamburguer_xpm, XPM_8_8_8_8, &fruits_images[5]);
  xpm_load(hot_dog_xpm, XPM_8_8_8_8, &fruits_images[6]);
  xpm_load(nabo_xpm, XPM_8_8_8_8, &fruits_images[7]);
  xpm_load(pizza_xpm, XPM_8_8_8_8, &fruits_images[8]);
  xpm_load(potatoes_xpm, XPM_8_8_8_8, &fruits_images[9]);
  xpm_load(boneco1, XPM_8_8_8_8, &player[0]);
  xpm_load(boneco_esq, XPM_8_8_8_8, &player[1]);
  xpm_load(boneco_dir, XPM_8_8_8_8, &player[2]);
  xpm_load(colher,XPM_8_8_8_8, &cursors[0]);
  xpm_load(mira_xpm,XPM_8_8_8_8,&cursors[1]);
  xpm_load(loading0, XPM_8_8_8_8, &loading[0]);
  xpm_load(loading1, XPM_8_8_8_8, &loading[1]);
  xpm_load(loading2, XPM_8_8_8_8, &loading[2]);
  xpm_load(loading3, XPM_8_8_8_8, &loading[3]);
  xpm_load(loading4, XPM_8_8_8_8, &loading[4]);
  xpm_load(loading5, XPM_8_8_8_8, &loading[5]);
  xpm_load(loading6, XPM_8_8_8_8, &loading[6]);
  xpm_load(loading7, XPM_8_8_8_8, &loading[7]);
  xpm_load(loading8, XPM_8_8_8_8, &loading[8]);
  xpm_load(loading9, XPM_8_8_8_8, &loading[9]);
  xpm_load(loading10, XPM_8_8_8_8, &loading[10]);
  xpm_load(loading11, XPM_8_8_8_8, &loading[11]);
  xpm_load(entrada,XPM_8_8_8_8, &backgrounds[0]);
  xpm_load(fundo_menu,XPM_8_8_8_8, &backgrounds[1]);
  xpm_load(menu_instructions,XPM_8_8_8_8,&backgrounds[2]);
  xpm_load(cozinha_jogo,XPM_8_8_8_8,&backgrounds[3]);
  xpm_load(loading_background,XPM_8_8_8_8, &backgrounds[4]);
  xpm_load(cozinha_jogo_escuro, XPM_8_8_8_8, &backgrounds[5]);
  xpm_load(cozinha_jogo_escuro_loading, XPM_8_8_8_8, &backgrounds[6]);
  xpm_load(cozinha_jogo_escuro_instructions, XPM_8_8_8_8, &backgrounds[7]);
  xpm_load(fundo_game_over, XPM_8_8_8_8, &backgrounds[8]);
  xpm_load(botao_main,XPM_8_8_8_8, &buttons[0]);
  xpm_load(single_player, XPM_8_8_8_8, &buttons[1]);
  xpm_load(volta_back, XPM_8_8_8_8, &buttons[2]);
  xpm_load(instructions, XPM_8_8_8_8, &buttons[3]);
  xpm_load(exit_xpm, XPM_8_8_8_8, &buttons[4]);
  xpm_load(back_xpm, XPM_8_8_8_8, &buttons[5]);
  xpm_load(pause_xpm, XPM_8_8_8_8, &buttons[6]);
  xpm_load(continue_xpm, XPM_8_8_8_8, &buttons[7]);
  xpm_load(botao_exit_pequeno, XPM_8_8_8_8, &buttons[8]);
  xpm_load(botao_menu_pequeno, XPM_8_8_8_8, &buttons[9]);
  xpm_load(volta_exit, XPM_8_8_8_8, &buttons[10]);
  xpm_load(volta_menu, XPM_8_8_8_8, &buttons[11]);
  xpm_load(exit_big, XPM_8_8_8_8, &buttons[12]); 
  xpm_load(single_big, XPM_8_8_8_8, &buttons[13]); 
  xpm_load(instructions_big, XPM_8_8_8_8, &buttons[14]);   
  xpm_load(continue_big, XPM_8_8_8_8, &buttons[15]);
  xpm_load(volta_pause, XPM_8_8_8_8, &buttons[16]);          
  xpm_load(num_0,XPM_8_8_8_8, &numbers[0]);
  xpm_load(num_1,XPM_8_8_8_8, &numbers[1]);
  xpm_load(num_2,XPM_8_8_8_8, &numbers[2]);
  xpm_load(num_3,XPM_8_8_8_8, &numbers[3]);
  xpm_load(num_4,XPM_8_8_8_8, &numbers[4]);
  xpm_load(num_5,XPM_8_8_8_8, &numbers[5]);
  xpm_load(num_6,XPM_8_8_8_8, &numbers[6]);
  xpm_load(num_7,XPM_8_8_8_8, &numbers[7]);
  xpm_load(num_8,XPM_8_8_8_8, &numbers[8]);
  xpm_load(num_9,XPM_8_8_8_8, &numbers[9]);
  xpm_load(mini_0,XPM_8_8_8_8, &numbers_mini[0]);
  xpm_load(mini_1,XPM_8_8_8_8, &numbers_mini[1]);
  xpm_load(mini_2,XPM_8_8_8_8, &numbers_mini[2]);
  xpm_load(mini_3,XPM_8_8_8_8, &numbers_mini[3]);
  xpm_load(mini_4,XPM_8_8_8_8, &numbers_mini[4]);
  xpm_load(mini_5,XPM_8_8_8_8, &numbers_mini[5]);
  xpm_load(mini_6,XPM_8_8_8_8, &numbers_mini[6]);
  xpm_load(mini_7,XPM_8_8_8_8, &numbers_mini[7]);
  xpm_load(mini_8,XPM_8_8_8_8, &numbers_mini[8]);
  xpm_load(mini_9,XPM_8_8_8_8, &numbers_mini[9]);
  xpm_load(traco,XPM_8_8_8_8, &numbers_mini[10]);
  xpm_load(vida,XPM_8_8_8_8, &vidas);
}

//creates all sprites
void create_sprites() {
    mini_number = create_sprite(&numbers_mini[0], 0,0,0,0);
    number = create_sprite(&numbers[0],7,12,0,0);
    vida_ = create_sprite(&vidas,0,12,0,0);
    man = create_sprite(&player[0],455,500,0,0); 
    pause_but = create_sprite(&buttons[6], 955, 800, 0, 0);
    background = create_sprite(&backgrounds[0],0,0,0,0);
    cursor = create_sprite(&cursors[0],50,50,0,0);
    loading_ = create_sprite(&loading[0], 530, 345, 0, 0);
    play = create_sprite(&buttons[0], 543, 581, 0, 0);  
    singleplayer_but = create_sprite(&buttons[1], 455, 292, 0, 0);
    instructions_but = create_sprite(&buttons[3], 455, 375, 0, 0);
    exit_but = create_sprite(&buttons[4], 455, 458, 0, 0);
    continue_but = create_sprite(&buttons[7], 455, 272, 0, 0);
    exit_mini_but = create_sprite(&buttons[8], 580, 476, 0, 0);
    menu_mini_but = create_sprite(&buttons[9], 397, 476, 0, 0);
    back_but = create_sprite(&buttons[5],580,548,0,0);
    
}

//destoys all sprites
void destroy_all_sprites() {
    destroy_sprite(background);
    destroy_sprite(cursor);
    destroy_sprite(man);
    destroy_sprite(vida_);
    destroy_sprite(number);
    destroy_sprite(mini_number);
    destroy_sprite(play);
    destroy_sprite(singleplayer_but);
    destroy_sprite(instructions_but);
    destroy_sprite(back_but);
    destroy_sprite(pause_but);
    destroy_sprite(continue_but);
    destroy_sprite(exit_but);
    destroy_sprite(exit_mini_but);
    destroy_sprite(menu_mini_but);
    destroy_sprite(loading_);
}
