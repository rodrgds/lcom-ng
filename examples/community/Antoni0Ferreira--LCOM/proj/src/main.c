// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"

//#include "UI.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "rtc.h"

#include "game.h"

uint8_t irq_kbd_set;
uint8_t irq_timer_set;
uint8_t irq_mouse_set;
uint8_t irq_rtc_set;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {


    vg_init(PROJ_VG_MODE);

    uint8_t bit_keyboard_no, bit_timer_no, bit_mouse_no, bit_rtc_no;

    
    if (keyboard_subscribe_int(&bit_keyboard_no) != 0)
        return 1;
    if (rtc_subscribe_int(&bit_rtc_no) != 0) 
        return 1;
    if (timer_set_frequency(0, FRAME_RATE) != 0)
        return 1;
    if (timer_subscribe_int(&bit_timer_no) != 0)
        return 1;
    if (mouse_enable_data())
        return 1;
    if (mouse_subscribe_int(&bit_mouse_no))
        return 1;

    irq_kbd_set = BIT(bit_keyboard_no);
    irq_timer_set = BIT(bit_timer_no);
    irq_mouse_set = BIT(bit_mouse_no);
    irq_rtc_set = BIT(bit_rtc_no);


    
    while(true){
        xpm_image_t imgmenuBG;
        uint8_t *menubg = initialize_menu_background(&imgmenuBG);
        int ret = menu(menubg, imgmenuBG);
        if(ret == 2){
            return 0;
        }
        if(ret == 1){
            return 1;
        }

        xpm_image_t imgBG;
        uint8_t *bg = initialize_game_background(&imgBG);

        if (vg_draw_sprite(bg, 0, 0, imgBG) != 0)
            return 1;

        play(bg,imgBG);

        if(end() == 2){
            break;
        }
        
    }

    if (mouse_unsubscribe_int())
        return 1;
    if (mouse_disable_data())
        return 1;
    if (timer_unsubscribe_int() != 0)
        return 1;
    if (keyboard_unsubscribe_int() != 0)
        return 1;
    if (rtc_unsubscribe_int() != 0)
        return 1;
    if (vg_exit() != 0)
        return 1;

    return 0;
}
